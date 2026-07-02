# G5LSI LoRa/FT8 + CA2RXU APRS Tracker

This firmware merges the functionality of [CA2RXU APRS Tracker](https://github.com/richonguzman/LoRa_APRS_Tracker/) and [G5LSI FT8/LoRa](https://github.com/indrusiak/ft8-lora).

It leaves all the APRS Tracker functionality completely untouched, and only enters the FT8 mode when it detects a triple-press of the user button during tracking mode. At that point, it stops sending out APRS packets, enables WiFi to serve the FT8/LoRa interface to a web browser, and allows QSOs to take place just as the standalone FT8/LoRa. Another triple-button press, it shuts down FT8/LoRa and goes back to APRS Tracker. 

It has been tested on T-Beam v1.2 and Heltec Tracker boards.

> This README covers **only the FT8 extension**. For everything about the base
> tracker — supported boards, configuration, the web config portal, GPS, power,
> digipeating, etc. — see the upstream project:
> **https://github.com/richonguzman/LoRa_APRS_Tracker**

## What it adds

- A second operating mode, **FT8/LoRa**, toggled from the tracker by a
  **triple-press** of the board button.
- **APRS mode** (default at boot): stock CA2RXU tracker, Wi-Fi off — low power
  for the hike.
- **FT8 mode**: raises a Wi-Fi access point **`FT8LoRa-XXXX`** and serves a
  WSJT-X-style QSO console at `http://192.168.4.1/`; APRS beaconing is suspended
  so the radio is dedicated to FT8.
- Triple-press again (or the console's *Return to APRS tracker* button) drops the
  AP and resumes tracking. A power-cycle always boots back into APRS mode.

## The FT8/LoRa console

- Callsign comes from your tracker's beacon config; **grid square auto-fills from
  the GPS fix**.
- Scrolling receive window, click-to-answer, **CQ** / **Answer** buttons,
  immediate TX. Completed QSOs are logged and exportable to **ADIF** (`/log.adi`).
- "FT8-over-LoRa" means the FT8 *message protocol and QSO workflow*
  (CQ → grid → report → RR73/73) carried as text in ordinary LoRa frames — **not**
  the FT8 waveform. Signal reports are derived from LoRa RSSI.

## Radio / coexistence

- FT8 shares the UK LoRa-APRS channel **439.9125 MHz, BW 125 kHz**, but transmits
  at **SF11** while APRS uses **SF12**. Different spreading factors are
  quasi-orthogonal, so FT8 and APRS don't hear each other (and FT8 traffic isn't
  gated to APRS-IS). All boards in one FT8 net must share frequency / BW / SF.
- Use a **70 cm (430–440 MHz) band** radio variant — not an 868/915 one.

## Build & flash

Build for your board's CA2RXU variant and upload **both** the firmware and the
filesystem:

```
pio run -e <your_variant> -t upload      # e.g. ttgo-t-beam-v1_2, heltec_wireless_tracker
pio run -e <your_variant> -t uploadfs    # required — writes the config filesystem
```

On first boot (callsign unset) the tracker's config portal comes up: set your
callsign and set the LoRa frequency to **439.9125 MHz**, then it reboots into
tracking. Board selection, configuration and flashing details are all in the
upstream project.

## License

**GPL-3.0**, as a derivative of CA2RXU's GPL-3.0 *LoRa APRS Tracker*. Bundled
third-party libraries (RadioLib, ArduinoJson, TFT_eSPI, the OLED driver) retain
their own (MIT) licenses.

## Credits

- Base tracker: **Ricardo Guzmán, CA2RXU** —
  https://github.com/richonguzman/LoRa_APRS_Tracker
- FT8/LoRa extension: **Leandro Soares Indrusiak, G5LSI**
