/* Copyright (C) 2026 Leandro Soares Indrusiak (G5LSI)
 *
 * Part of the combined LoRa APRS Tracker + FT8/LoRa firmware (GPL-3.0),
 * a derivative of LoRa APRS Tracker (C) Ricardo Guzman - CA2RXU.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version. See <https://www.gnu.org/licenses/>.
 */

#ifndef FT8_WEB_H_
#define FT8_WEB_H_

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

// FT8/LoRa web console: serves the QSO UI (web_ui.h) and its /api/* endpoints on
// CA2RXU's AsyncWebServer, backed by the reusable rx_store / qso_log. The browser
// holds the QSO state machine (ft8.js); the device just moves bytes + logs.
namespace FT8_Web {
    void registerRoutes(AsyncWebServer& server); // page + /api/* + /log.adi
    void pushRx(const String& text, int rssi);   // called from the FT8 RX gate in msg_utils
}

#endif
