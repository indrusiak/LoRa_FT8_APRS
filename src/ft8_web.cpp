/* Copyright (C) 2026 Leandro Soares Indrusiak (G5LSI)  -- GPL-3.0
 * Part of the combined LoRa APRS Tracker + FT8/LoRa firmware,
 * a derivative of LoRa APRS Tracker (C) Ricardo Guzman - CA2RXU.
 * See <https://www.gnu.org/licenses/>.
 */

#include "ft8_web.h"
#include <TinyGPSPlus.h>
#include <math.h>
#include "configuration.h"
#include "mode_manager.h"
#include "display.h"
#include "rx_store.h"
#include "qso_log.h"
#include "web_ui.h"

extern Configuration Config;
extern TinyGPSPlus    gps;
extern Beacon         *currentBeacon;   // active beacon profile (holds the callsign)

namespace FT8_Web {

// FT8/LoRa net parameters (fixed by FT8 mode: 439.9125 MHz, SF11, BW125).
static const float FT8_FREQ = 439.9125f;
static const int   FT8_SF   = 11;
static const int   FT8_BW   = 125;

static RxStore  rxs;
static QsoLog   qsos;
static String   ft8Callsign;       // override; defaults to Config.callsign
static String   ft8GridOverride;   // override; defaults to GPS-derived grid
static String   lastTx;
static uint32_t lastTxMs = 0;
static uint32_t txCount  = 0;
static int      lastRssi = 0;

// ---- helpers --------------------------------------------------------------
static void jsonEsc(String& out, const char* s) {
    for (const char* p = s; *p; p++) {
        char c = *p;
        if (c == '"' || c == '\\') { out += '\\'; out += c; }
        else if (c == '\n') out += "\\n";
        else if (c == '\r') { /* drop */ }
        else out += c;
    }
}

// 6-character Maidenhead locator from the current GPS fix ("" if no fix).
static String maidenhead() {
    if (!gps.location.isValid()) return "";
    double lon = gps.location.lng() + 180.0;
    double lat = gps.location.lat() + 90.0;
    char g[7];
    g[0] = 'A' + (int)(lon / 20);
    g[1] = 'A' + (int)(lat / 10);
    g[2] = '0' + (int)(fmod(lon, 20) / 2);
    g[3] = '0' + (int)(fmod(lat, 10));
    g[4] = 'A' + (int)(fmod(lon, 2)  * 12.0);
    g[5] = 'A' + (int)(fmod(lat, 1)  * 24.0);
    g[6] = 0;
    return String(g);
}

static String callsign() { return ft8Callsign.length() ? ft8Callsign : currentBeacon->callsign; }
static String grid()     { return ft8GridOverride.length() ? ft8GridOverride : maidenhead(); }

// ---- RX ingest (called from the FT8 gate in msg_utils, loop context) ------
void pushRx(const String& text, int rssi) {
    lastRssi = rssi;
    rxs.add((const uint8_t*)text.c_str(), (uint16_t)text.length(), (int16_t)rssi, 0.0f);
    Serial.printf("FT8 Rx: %s  (RSSI %d)\n", text.c_str(), rssi);
    displayShow("FT8 RX", text, "RSSI " + String(rssi), 0);
}

// ---- handlers -------------------------------------------------------------
static void handleRoot(AsyncWebServerRequest* req) {
    req->send(200, "text/html", INDEX_HTML);        // flash is memory-mapped on ESP32
}

static void handleExit(AsyncWebServerRequest* req) {
    req->send(200, "text/plain", "ok");
    MODE_Manager::requestTracker();                 // deferred teardown in loop()
}

static void handleStatus(AsyncWebServerRequest* req) {
    bool txing = (lastTxMs != 0) && (millis() - lastTxMs < 1200);
    String j; j.reserve(384);
    j += "{\"callsign\":\""; jsonEsc(j, callsign().c_str()); j += "\"";
    j += ",\"grid\":\"";     jsonEsc(j, grid().c_str());     j += "\"";
    j += ",\"freq\":"; j += String(FT8_FREQ, 4);
    j += ",\"bw\":";   j += FT8_BW;
    j += ",\"sf\":";   j += FT8_SF;
    j += ",\"rssi\":"; j += lastRssi;
    j += ",\"state\":\""; j += (txing ? "TX" : "RX"); j += "\"";
    j += ",\"rx\":";    j += rxs.count();
    j += ",\"tx\":";    j += txCount;
    j += ",\"err\":0";
    j += ",\"count\":"; j += rxs.count();
    j += ",\"cap\":";   j += rxs.capacity();
    j += ",\"qsos\":";  j += qsos.count();
    j += "}";
    req->send(200, "application/json", j);
}

static void handleRx(AsyncWebServerRequest* req) {
    uint32_t since = 0;
    if (req->hasParam("since"))
        since = strtoul(req->getParam("since")->value().c_str(), nullptr, 10);
    String out = "[";
    bool first = true;
    rxs.for_each_since(since, 80, [&](const RxMsg& m) {
        if (!first) out += ",";
        first = false;
        out += "{\"id\":"; out += m.id;
        out += ",\"t\":";  out += m.t_ms;
        out += ",\"rssi\":"; out += m.rssi;
        out += ",\"snr\":";  out += String(m.snr_cdb / 100.0f, 1);
        out += ",\"text\":\""; jsonEsc(out, m.text); out += "\"}";
    });
    out += "]";
    req->send(200, "application/json", out);
}

static void handleConfig(AsyncWebServerRequest* req) {
    if (req->hasParam("callsign", true)) { ft8Callsign     = req->getParam("callsign", true)->value(); ft8Callsign.toUpperCase(); }
    if (req->hasParam("grid", true))     { ft8GridOverride = req->getParam("grid", true)->value();     ft8GridOverride.toUpperCase(); }
    // freq/sf are fixed by FT8 mode; console changes to them are intentionally ignored.
    req->send(200, "application/json", "{\"ok\":true}");
}

static void handleTx(AsyncWebServerRequest* req) {
    if (!req->hasParam("text", true)) { req->send(400, "application/json", "{\"ok\":false,\"error\":\"no text\"}"); return; }
    String text = req->getParam("text", true)->value();
    if (!text.length())               { req->send(400, "application/json", "{\"ok\":false,\"error\":\"empty\"}");   return; }
    MODE_Manager::requestTx(text);      // deferred to loop() -> LoRa_Utils::sendRawFT8
    lastTx = text; lastTxMs = millis(); txCount++;
    req->send(200, "application/json", "{\"ok\":true,\"status\":0}");
}

static void handleLogPost(AsyncWebServerRequest* req) {
    auto arg = [&](const char* n) -> String { return req->hasParam(n, true) ? req->getParam(n, true)->value() : String(""); };
    auto cp  = [](char* dst, size_t n, const String& src) { strncpy(dst, src.c_str(), n - 1); dst[n - 1] = '\0'; };
    QsoRecord r{};
    cp(r.call,     sizeof(r.call),     arg("call"));
    cp(r.gridsq,   sizeof(r.gridsq),   arg("gridsquare"));
    cp(r.rst_sent, sizeof(r.rst_sent), arg("rst_sent"));
    cp(r.rst_rcvd, sizeof(r.rst_rcvd), arg("rst_rcvd"));
    cp(r.qso_date, sizeof(r.qso_date), arg("qso_date"));
    cp(r.time_on,  sizeof(r.time_on),  arg("time_on"));
    cp(r.mycall,   sizeof(r.mycall),   req->hasParam("mycall", true) ? arg("mycall") : callsign());
    cp(r.mygrid,   sizeof(r.mygrid),   req->hasParam("mygrid", true) ? arg("mygrid") : grid());
    r.freq_mhz = req->hasParam("freq", true) ? arg("freq").toFloat() : FT8_FREQ;
    qsos.add(r);
    req->send(200, "application/json", "{\"ok\":true,\"qsos\":" + String(qsos.count()) + "}");
}

static void handleLogGet(AsyncWebServerRequest* req) {
    String out = "[";
    bool first = true;
    qsos.for_each([&](const QsoRecord& r) {
        if (!r.used) return;
        if (!first) out += ",";
        first = false;
        out += "{\"call\":\"";        jsonEsc(out, r.call);     out += "\"";
        out += ",\"gridsquare\":\"";  jsonEsc(out, r.gridsq);   out += "\"";
        out += ",\"rst_sent\":\"";    jsonEsc(out, r.rst_sent); out += "\"";
        out += ",\"rst_rcvd\":\"";    jsonEsc(out, r.rst_rcvd); out += "\"";
        out += ",\"qso_date\":\"";    jsonEsc(out, r.qso_date); out += "\"";
        out += ",\"time_on\":\"";     jsonEsc(out, r.time_on);  out += "\"";
        out += ",\"freq\":";          out += String(r.freq_mhz, 4); out += "}";
    });
    out += "]";
    req->send(200, "application/json", out);
}

static void handleLogClear(AsyncWebServerRequest* req) {
    qsos.clear();
    req->send(200, "application/json", "{\"ok\":true}");
}

static void handleLogAdif(AsyncWebServerRequest* req) {
    String o;
    o += "ADIF export from FT8-over-LoRa\n";
    o += "<ADIF_VER:5>3.1.4\n<PROGRAMID:12>FT8-over-LoRa\n<EOH>\n\n";
    qsos.for_each([&](const QsoRecord& r) {
        if (!r.used) return;
        adif_field(o, "CALL", r.call);
        adif_field(o, "GRIDSQUARE", r.gridsq);
        adif_field(o, "MODE", "DATA");
        adif_field(o, "RST_SENT", r.rst_sent);
        adif_field(o, "RST_RCVD", r.rst_rcvd);
        adif_field(o, "QSO_DATE", r.qso_date);
        adif_field(o, "TIME_ON", r.time_on);
        if (r.freq_mhz > 0) adif_field(o, "FREQ", String(r.freq_mhz, 4));
        const char* band = QsoLog::band_for(r.freq_mhz);
        if (band && *band) adif_field(o, "BAND", band);
        adif_field(o, "STATION_CALLSIGN", r.mycall);
        adif_field(o, "OPERATOR", r.mycall);
        adif_field(o, "MY_GRIDSQUARE", r.mygrid);
        adif_field(o, "COMMENT", "FT8 protocol over LoRa");
        o += "<EOR>\n";
    });
    AsyncWebServerResponse* resp = req->beginResponse(200, "text/plain", o);
    resp->addHeader("Content-Disposition", "inline; filename=\"ft8lora_log.adi\"");
    req->send(resp);
}

void registerRoutes(AsyncWebServer& server) {
    server.on("/",              HTTP_GET,  handleRoot);
    server.on("/exit-ft8",      HTTP_POST, handleExit);
    server.on("/api/status",    HTTP_GET,  handleStatus);
    server.on("/api/rx",        HTTP_GET,  handleRx);
    server.on("/api/config",    HTTP_POST, handleConfig);
    server.on("/api/tx",        HTTP_POST, handleTx);
    server.on("/api/log",       HTTP_POST, handleLogPost);
    server.on("/api/log",       HTTP_GET,  handleLogGet);
    server.on("/api/log/clear", HTTP_POST, handleLogClear);
    server.on("/log.adi",       HTTP_GET,  handleLogAdif);
}

} // namespace FT8_Web
