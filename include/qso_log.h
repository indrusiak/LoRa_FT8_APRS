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
// qso_log.h - a ring of completed QSOs + ADIF export.
//
// The browser owns the FT8 QSO state machine and decides when a contact is
// complete; it then POSTs the finished record here (with a real UTC date/time
// from the operator's device clock, since the board is an isolated AP with no
// NTP). The device keeps the log across browser reloads and renders ADIF on
// request. ADIF is emitted as the canonical <FIELD:len>value form with a header.
//
#include <Arduino.h>
#include <stdint.h>
#include <string.h>

#ifndef QSO_RING_SIZE
#define QSO_RING_SIZE 100
#endif

struct QsoRecord {
  char call[16];
  char gridsq[10];
  char rst_sent[8];
  char rst_rcvd[8];
  char qso_date[10];   // YYYYMMDD
  char time_on[8];     // HHMMSS
  char mycall[16];
  char mygrid[10];
  float freq_mhz;
  bool used;
};

class QsoLog {
 public:
  void clear() { head_ = 0; count_ = 0; memset(ring_, 0, sizeof(ring_)); }

  void add(const QsoRecord& r) {
    ring_[head_] = r;
    ring_[head_].used = true;
    head_ = (head_ + 1) % QSO_RING_SIZE;
    if (count_ < QSO_RING_SIZE) count_++;
  }

  int count() const { return count_; }

  template <typename F>
  void for_each(F fn) const {
    int n = (count_ < QSO_RING_SIZE) ? count_ : QSO_RING_SIZE;
    int start = (head_ - n + QSO_RING_SIZE) % QSO_RING_SIZE;
    for (int k = 0; k < n; k++) fn(ring_[(start + k) % QSO_RING_SIZE]);
  }

  // Derive an ADIF band string from a frequency in MHz (the bands this board can
  // plausibly use). Falls back to empty so BAND is simply omitted if unknown.
  static const char* band_for(float f) {
    if (f >= 430.0f && f <= 440.0f) return "70cm";
    if (f >= 144.0f && f <= 148.0f) return "2m";
    if (f >= 868.0f && f <= 870.0f) return ""; // 868 is ISM, no ham band; left blank-ish
    if (f >= 902.0f && f <= 928.0f) return "33cm";
    return "";
  }

 private:
  QsoRecord ring_[QSO_RING_SIZE];
  int head_ = 0, count_ = 0;
};

// ---- ADIF rendering helpers (free functions so main.cpp can stream them) ----
static inline void adif_field(String& out, const char* name, const char* val) {
  if (!val || !*val) return;
  out += "<"; out += name; out += ":"; out += String((unsigned)strlen(val)); out += ">"; out += val;
}
static inline void adif_field(String& out, const char* name, const String& val) {
  adif_field(out, name, val.c_str());
}
