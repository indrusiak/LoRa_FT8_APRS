/* Copyright (C) 2026 Leandro Soares Indrusiak (G5LSI)
 *
 * This file is part of the combined LoRa APRS Tracker + FT8/LoRa firmware,
 * a derivative of LoRa APRS Tracker (C) Ricardo Guzman - CA2RXU.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include <WiFi.h>
#include <logger.h>
#include "mode_manager.h"
#include "configuration.h"
#include "web_utils.h"
#include "lora_utils.h"
#include "display.h"

extern Configuration    Config;
extern logging::Logger  logger;

#define FT8_AP_SSID     "FT8LoRa-AP"
#define FT8_SF          11     // FT8 uses SF11; APRS uses SF12 -- different SF = real PHY separation

namespace MODE_Manager {

    static OpMode mode = MODE_TRACKER;
    static bool   pendingTracker = false;
    static bool   pendingTx      = false;
    static String pendingTxMsg;

    OpMode current() { return mode; }
    bool   isFT8()   { return mode == MODE_FT8; }

    void enterFT8() {
        if (mode == MODE_FT8) return;
        mode = MODE_FT8;
        logger.log(logging::LoggerLevel::LOGGER_LEVEL_INFO, "Mode", "Entering FT8/LoRa mode");

        // AsyncWebServer is event-driven, so unlike the blocking config portal we
        // bring the AP + server up and return to the main loop.
        // Unique per-board SSID (last 4 hex of the MAC) so two units don't
        // present the same AP name during testing.
        uint32_t mac = (uint32_t)ESP.getEfuseMac();
        String ssid  = String(FT8_AP_SSID) + "-" + String(mac & 0xFFFF, HEX);
        WiFi.mode(WIFI_AP);
        WiFi.softAP(ssid.c_str(), Config.wifiAP.password);
        WEB_Utils::setupFT8();

        // Same freq/BW/CR as APRS, but SF10 vs APRS's SF12 -- different spreading
        // factors are quasi-orthogonal, so the two nets genuinely don't hear each
        // other (unlike the sync word). startReceive() re-arms RX for FT8 frames.
        LoRa_Utils::setSpreadingFactor(FT8_SF);
        // Header renders at text-size 2 -- keep it very short ("FT8" fits easily).
        displayShow("FT8", "AP:" + ssid, "IP: 192.168.4.1", "", "3x-press: APRS", "");
    }

    void enterTracker() {
        if (mode == MODE_TRACKER) return;
        mode = MODE_TRACKER;
        logger.log(logging::LoggerLevel::LOGGER_LEVEL_INFO, "Mode", "Returning to APRS tracker mode");

        WEB_Utils::stopFT8();
        WiFi.softAPdisconnect(true);
        WiFi.mode(WIFI_OFF);

        LoRa_Utils::restoreAprsSpreadingFactor();   // back to APRS SF (SF12)
        displayShow("", "", "  APRS Tracker", "  WiFi off", "", "", 1500);
    }

    void toggle() {
        if (mode == MODE_TRACKER) enterFT8();
        else                      enterTracker();
    }

    // Called from the FT8 web page's "return" button. Resetting the web server
    // from inside its own request callback is unsafe, so defer to loop().
    void requestTracker() { pendingTracker = true; }

    // Called from the FT8 web page's TX button. The radio SPI is serviced by the
    // main loop, so we NEVER transmit from the async HTTP task -- defer to loop().
    void requestTx(const String& msg) { pendingTxMsg = msg; pendingTx = true; }

    void setup() {
        mode = MODE_TRACKER;     // always boot as a tracker (WiFi already off)
    }

    void loop() {
        if (pendingTracker) { pendingTracker = false; enterTracker(); }
        if (pendingTx) {
            pendingTx = false;
            LoRa_Utils::sendRawFT8(pendingTxMsg);
            displayShow("FT8 TX", pendingTxMsg, "", 0);   // local confirmation, wait=0
        }
    }

}
