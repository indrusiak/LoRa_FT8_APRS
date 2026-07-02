# G5LSI LoRa/FT8 + CA2RXU APRS Tracker

This firmware merges the functionality of [CA2RXU APRS Tracker](https://github.com/richonguzman/LoRa_APRS_Tracker/) and [G5LSI FT8/LoRa](https://github.com/indrusiak/ft8-lora).

It leaves all the APRS Tracker functionality completely untouched, and only enters the FT8 mode when it detects a triple-button press during tracking mode. At that point, it stops sending out APRS packets, enables WiFi to serve the FT8/LoRa interface to a web browser, and allows QSOs to take place just as the standalone FT8/LoRa. Another triple-button press, it shuts down FT8/LoRa and goes back to APRS Tracker. 

It has been tested on T-Beam v1.2 and Heltec Tracker boards.
