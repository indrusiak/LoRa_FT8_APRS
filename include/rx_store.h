#pragma once
// Copyright 2026 Leandro Soares Indrusiak (G5LSI)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// rx_store.h - a small ring of received FT8-over-LoRa messages.
//
// Each entry is the decoded ASCII text of a LoRa packet plus the metadata the UI
// needs: a monotonic id (the browser polls "since" this), a capture time, RSSI
// and SNR. The browser turns RSSI into an FT8 report and runs the QSO logic;
// the device just stores and serves.
//
#include <Arduino.h>
#include <stdint.h>
#include <string.h>

#ifndef RX_MSG_MAX
#define RX_MSG_MAX 40       // an FT8 message is short; this is generous
#endif
#ifndef RX_RING_SIZE
#define RX_RING_SIZE 120
#endif

struct RxMsg {
  uint32_t id;              // monotonic, 0 = empty
  uint32_t t_ms;           // millis() at capture
  int16_t  rssi;           // dBm
  int16_t  snr_cdb;        // SNR centi-dB
  char     text[RX_MSG_MAX + 1];
};

class RxStore {
 public:
  void clear() { head_ = 0; count_ = 0; next_id_ = 1; memset(ring_, 0, sizeof(ring_)); }

  uint32_t add(const uint8_t* d, uint16_t len, int16_t rssi, float snr) {
    if (len > RX_MSG_MAX) len = RX_MSG_MAX;
    RxMsg& m = ring_[head_];
    m.id = next_id_++;
    m.t_ms = millis();
    m.rssi = rssi;
    m.snr_cdb = (int16_t)(snr * 100.0f);
    // Copy as text, keeping only printable ASCII so the UI never breaks on noise.
    uint16_t j = 0;
    for (uint16_t i = 0; i < len; i++) {
      char c = (char)d[i];
      m.text[j++] = (c >= 0x20 && c < 0x7f) ? c : '.';
    }
    m.text[j] = '\0';
    head_ = (head_ + 1) % RX_RING_SIZE;
    if (count_ < RX_RING_SIZE) count_++;
    return m.id;
  }

  template <typename F>
  void for_each_since(uint32_t since, int limit, F fn) const {
    int n = (count_ < RX_RING_SIZE) ? count_ : RX_RING_SIZE;
    int start = (head_ - n + RX_RING_SIZE) % RX_RING_SIZE;
    for (int k = 0; k < n && limit > 0; k++) {
      const RxMsg& m = ring_[(start + k) % RX_RING_SIZE];
      if (m.id > since) { fn(m); limit--; }
    }
  }

  uint32_t latest_id() const { return next_id_ - 1; }
  int count() const { return count_; }
  int capacity() const { return RX_RING_SIZE; }

 private:
  RxMsg ring_[RX_RING_SIZE];
  int head_ = 0, count_ = 0;
  uint32_t next_id_ = 1;
};
