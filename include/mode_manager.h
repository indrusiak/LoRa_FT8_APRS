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

#ifndef MODE_MANAGER_H_
#define MODE_MANAGER_H_

#include <Arduino.h>

// Two mutually-exclusive operating modes sharing one radio:
//   MODE_TRACKER : stock CA2RXU APRS tracker, WiFi off (low power, for the hike)
//   MODE_FT8     : SoftAP + web UI up, APRS beaconing suspended (for the summit)
namespace MODE_Manager {

    enum OpMode { MODE_TRACKER, MODE_FT8 };

    void   setup();          // call once in setup(); always boots as TRACKER
    void   loop();           // call each main-loop pass (FT8 service hook; no-op for now)

    OpMode current();
    bool   isFT8();          // used by sendNewPacket() to suspend APRS TX in FT8 mode

    void   toggle();         // wired to the button gesture (runs in loop ctx)
    void   requestTracker(); // safe to call from an HTTP handler; acts next loop()
    void   requestTx(const String& msg); // safe from an HTTP handler; TX happens in loop()
    void   enterFT8();
    void   enterTracker();

}

#endif
