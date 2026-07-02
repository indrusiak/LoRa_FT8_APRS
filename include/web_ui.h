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

#pragma once
#include <Arduino.h>

// Auto-generated from test/page_app.html + test/ft8.js (spliced verbatim).
// Do not hand-edit; see README.md ("Regenerating web_ui.h").

static const char INDEX_HTML[] PROGMEM = R"FT8PAGE(
<!DOCTYPE html>
<!--
  Copyright 2026 Leandro Soares Indrusiak (G5LSI)

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
-->
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>FT8/LoRa by G5LSI v0.1</title>
<style>
  *{box-sizing:border-box}
  body{font-family:ui-monospace,"Courier New",monospace;margin:0;background:#0e1116;color:#cdd6e0;font-size:13px}
  header{position:sticky;top:0;background:#11161d;border-bottom:1px solid #222b36;padding:8px 12px;z-index:5}
  .row{display:flex;gap:10px;flex-wrap:wrap;align-items:center}
  .pill{background:#1b2330;border:1px solid #2a3646;border-radius:5px;padding:3px 8px}
  .pill b{color:#5cc8ff}
  .pill.tx b{color:#ffd166}
  .wrap{padding:10px 12px;max-width:980px;margin:0 auto}
  select,input,button{font-family:inherit;font-size:13px;background:#0c1219;color:#cdd6e0;border:1px solid #2a3646;border-radius:5px;padding:6px 8px}
  button{cursor:pointer;background:#17202b}
  button:hover{background:#202c3a}
  button.go{background:#1d6fb8;border-color:#1d6fb8;color:#fff}
  button.tx{background:#a8431f;border-color:#a8431f;color:#fff;font-weight:bold}
  button.warn{background:#7a3b15;border-color:#7a3b15;color:#fff}
  h2{font-size:13px;color:#7d8aa0;font-weight:normal;margin:14px 0 6px;text-transform:uppercase;letter-spacing:.06em}
  .cfg{display:grid;grid-template-columns:repeat(auto-fill,minmax(130px,1fr));gap:8px;align-items:end}
  .cfg label{display:flex;flex-direction:column;font-size:11px;color:#7d8aa0;gap:3px}
  /* WSJT-X style scrolling RX window */
  #rxwin{height:46vh;min-height:220px;overflow-y:auto;background:#0a0f15;border:1px solid #1b232e;border-radius:6px;padding:4px 0}
  .line{display:grid;grid-template-columns:74px 44px 64px 1fr;gap:8px;padding:2px 10px;white-space:nowrap;border-bottom:1px solid #0e151d}
  .line .t{color:#5f6e82}
  .line .db{text-align:right;color:#8fb0c8}
  .line .rs{text-align:right;color:#43525f}
  .line .m{overflow:hidden;text-overflow:ellipsis}
  .line.rx.cq{cursor:pointer}
  .line.rx.cq .m{color:#7fe0a6}
  .line.rx.cq:hover{background:#10241a}
  .line.rx.me{cursor:pointer;background:#10202f}
  .line.rx.me .m{color:#9fd0ff}
  .line.rx.me:hover{background:#16314a}
  .line.rx.other .m{color:#b7c4d4}
  .line.tx{background:#241409}
  .line.tx .m{color:#ffba73;font-weight:bold}
  .line.tx .t{color:#7a5a36}
  .txbar{display:flex;gap:8px;align-items:center;margin-top:10px}
  #txField{flex:1;min-width:160px;font-size:15px;letter-spacing:.04em}
  .qbtns{display:flex;gap:6px;flex-wrap:wrap;margin-top:8px}
  .muted{color:#6b7889}
  .hint{color:#6b7889;font-size:11px;margin-top:4px}
  table{width:100%;border-collapse:collapse;margin-top:6px}
  th,td{text-align:left;padding:4px 8px;border-bottom:1px solid #1b232e;white-space:nowrap;font-size:12px}
  th{color:#7d8aa0;font-weight:normal}
  #status-state.tx{color:#ffd166}
  .title{font-weight:bold;color:#5cc8ff;font-size:15px;margin-bottom:6px;letter-spacing:.3px}
  #rxwin .line.sel{outline:1px solid #ffd166;outline-offset:-1px;background:#2a2410}
</style>
</head>
<body>
<header>
  <div class="title">FT8/LoRa by G5LSI v0.1</div>
  <div class="row">
    <span class="pill">me <b id="s-call">-</b></span>
    <span class="pill">grid <b id="s-grid">-</b></span>
    <span class="pill"><b id="s-freq">-</b> MHz</span>
    <span class="pill">SF <b id="s-sf">-</b></span>
    <span class="pill">BW <b id="s-bw">-</b></span>
    <span class="pill">rssi <b id="s-rssi">-</b></span>
    <span class="pill" id="s-statepill"><b id="s-state">RX</b></span>
    <span class="pill">RX <b id="s-rx">0</b></span>
    <span class="pill">TX <b id="s-tx">0</b></span>
    <span class="pill">QSOs <b id="s-qsos">0</b></span>
  </div>
</header>

<div class="wrap">

  <h2>Station &amp; radio</h2>
  <div class="cfg">
    <label>Callsign<input id="c-call" maxlength="10" placeholder="G5LSI"></label>
    <label>Grid<input id="c-grid" maxlength="8" placeholder="IO93"></label>
    <label>Freq MHz<input id="c-freq" type="number" step="0.0001" value="439.9125"></label>
    <label>SF<select id="c-sf"><option>7</option><option>8</option><option>9</option><option>10</option><option>11</option><option selected>12</option></select></label>
    <label>BW kHz<input id="c-bw" type="number" step="1" value="125"></label>
    <label>&nbsp;<button class="go" id="applyBtn">Apply</button></label>
  </div>
  <div class="hint">Default 439.9125 MHz (UK-coordinated 70cm low-power LoRa channel, 125 kHz max BW), SF12, BW 125 kHz. Coding rate 4/5, sync 0x14 and CRC are fixed for the net &mdash; every board must match freq/BW/SF/sync to hear each other.</div>

  <h2>Decoded traffic</h2>
  <div id="rxwin"></div>
  <div class="hint">Times are UTC. <b style="color:#8fb0c8">dB</b> is an FT8-style report derived from LoRa RSSI (&minus;23&hellip;+23). The last message <b style="color:#9fd0ff">addressed to you</b> is highlighted automatically; click any line to select it instead. <b>CQ</b> loads a CQ call; <b>Answer</b> generates the reply to the highlighted line. Replies to your calls also auto-fill the TX field.</div>

  <div class="txbar">
    <input id="txField" placeholder="message to transmit, e.g. CQ G5LSI IO93" spellcheck="false" autocapitalize="characters">
    <button class="tx" id="txBtn">TX</button>
  </div>
  <div class="qbtns">
    <button id="ansBtn">Answer</button>
    <button id="cqBtn">CQ</button>
    <button id="dxBtn" class="muted">Reset QSO</button>
    <span class="muted" id="txMsg"></span>
  </div>
  <div class="hint">TX sends immediately and the text stays in the field, so pressing TX again retransmits it. Nothing is sent automatically &mdash; auto-fill only proposes the next message.</div>

  <h2>QSO log</h2>
  <div class="row">
    <button class="go" id="adifBtn">Export ADIF</button>
    <button class="warn" id="logClrBtn">Clear log</button>
    <span class="muted" id="logMsg"></span>
  </div>
  <table>
    <thead><tr><th>UTC date</th><th>time</th><th>call</th><th>grid</th><th>sent</th><th>rcvd</th><th>MHz</th></tr></thead>
    <tbody id="logRows"></tbody>
  </table>

</div>

<script>
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

// ft8.js - the FT8-over-LoRa protocol brain.
//
// This is pure logic, no DOM and no radio. It is used verbatim inside the web
// page AND by the Node test in ../test, exactly like decoders.js was in the
// sniffer. Three jobs:
//
//   1. parseMsg(text)      - classify one FT8 message string into a typed object
//   2. rssiToReport(dbm)   - map a LoRa RSSI (dBm) to an FT8 report in [-23,+23]
//   3. a small QSO engine  - given my call/grid and the running context, decide
//                            what reply belongs in the TX field, and notice when
//                            a QSO has completed (so it can be logged).
//
// What "FT8 over LoRa" means here: we keep the FT8 *message grammar* and the
// *QSO sequence* (CQ -> call -> report -> roger -> RR73/73, Maidenhead grids,
// signed dB reports) but the bytes ride inside an ordinary LoRa packet as ASCII,
// and the report we hand out is synthesised from LoRa RSSI rather than recovered
// from an FT8 soft-decode. None of the 8-FSK / 15 s slot / LDPC machinery of
// real FT8 is involved; the SX1262 cannot make that waveform.

(function (root) {
  'use strict';

  // ---- field tests ---------------------------------------------------------
  // Maidenhead: 2 letters A-R, 2 digits, optional 2 letters (sub-square).
  const GRID_RE = /^[A-R]{2}[0-9]{2}([A-X]{2})?$/i;
  // A callsign is loosely "has a digit, alnum/slash, 3-10 chars" - good enough to
  // separate calls from grids/reports/keywords without a full ITU validator.
  const CALL_RE = /^[A-Z0-9/]{3,10}$/i;

  const isGrid   = (t) => GRID_RE.test(t);
  const isReport = (t) => /^[-+]\d{1,2}$/.test(t);          // -12, +05, -1
  const isRReport= (t) => /^R[-+]\d{1,2}$/i.test(t);        // R-12, R+05
  // A call has a letter and a digit, fits the charset, and is NOT grid-shaped
  // (a bare "IO82" is a grid, never a callsign).
  const isCall    = (t) => CALL_RE.test(t) && /\d/.test(t) && /[A-Z]/i.test(t) && !isGrid(t);

  // ---- RSSI -> FT8 report --------------------------------------------------
  // LoRa RSSI runs from roughly the noise floor (~-120 dBm on a quiet 70 cm
  // channel with this modem) up to ~-28 dBm for a strong nearby transmitter.
  // Map that span linearly onto the FT8 report range,
  // [-23, +23] (~2 dB of RSSI per report unit), and clamp the ends.
  const RSSI_LO = -120;   // -> -23
  const RSSI_HI = -28;    // -> +23
  function rssiToReport(dbm) {
    if (dbm === null || dbm === undefined || isNaN(dbm)) return 0;
    let r = Math.round(((dbm - RSSI_LO) / (RSSI_HI - RSSI_LO)) * 46) - 23;
    if (r < -23) r = -23;
    if (r > 23) r = 23;
    return r;
  }
  // Format a report the FT8 way: sign always shown, magnitude zero-padded to 2.
  function fmtReport(r) {
    const s = r < 0 ? '-' : '+';
    return s + String(Math.abs(r)).padStart(2, '0');
  }

  // ---- message parser ------------------------------------------------------
  // Returns one of:
  //   {kind:'CQ',      call, grid?}                      e.g. "CQ G5LSI IO82"
  //   {kind:'grid',    to, from, grid}                   e.g. "G5LSI M7NSE IO93"
  //   {kind:'report',  to, from, report}                 e.g. "M7NSE G5LSI -12"
  //   {kind:'Rreport', to, from, report}                 e.g. "G5LSI M7NSE R-08"
  //   {kind:'RR73'|'RRR'|'73', to, from}                 e.g. "M7NSE G5LSI RR73"
  //   {kind:'other',   to?, from?, text}                 anything else
  function parseMsg(text) {
    const toks = String(text || '').trim().toUpperCase().split(/\s+/).filter(Boolean);
    if (!toks.length) return { kind: 'other', text: '' };

    if (toks[0] === 'CQ') {
      // Forms: "CQ CALL GRID", "CQ DX CALL GRID", "CQ POTA CALL GRID", "CQ CALL".
      // Find the call (token containing a digit) and an optional trailing grid.
      let grid = null, call = null;
      if (toks.length >= 2 && isGrid(toks[toks.length - 1])) grid = toks[toks.length - 1];
      for (let i = 1; i < toks.length; i++) {
        if (toks[i] === grid) continue;
        if (isCall(toks[i])) { call = toks[i]; break; }
      }
      return { kind: 'CQ', call, grid };
    }

    // Directed: "<to> <from> <rest...>"
    const to = toks[0], from = toks[1];
    const rest = toks.slice(2);
    const base = { to, from };
    if (!rest.length) return Object.assign({ kind: 'call' }, base);

    const r0 = rest[0];
    if (r0 === 'RR73') return Object.assign({ kind: 'RR73' }, base);
    if (r0 === 'RRR')  return Object.assign({ kind: 'RRR' }, base);
    if (r0 === '73')   return Object.assign({ kind: '73' }, base);
    if (isGrid(r0))    return Object.assign({ kind: 'grid', grid: r0 }, base);
    if (isRReport(r0)) return Object.assign({ kind: 'Rreport', report: parseInt(r0.slice(1), 10) }, base);
    if (isReport(r0))  return Object.assign({ kind: 'report', report: parseInt(r0, 10) }, base);
    return Object.assign({ kind: 'other', text: rest.join(' ') }, base);
  }

  // Convenience: who is this message addressed to? (null for CQ / unparseable)
  function addressedTo(text) {
    const m = parseMsg(text);
    return (m.kind === 'CQ' || m.kind === 'other' && !m.to) ? null : (m.to || null);
  }

  // ---- QSO engine ----------------------------------------------------------
  // A QSO context is a plain object; make a fresh one with newQso().
  function newQso() {
    return {
      partner: null,        // their callsign once known
      theirGrid: null,
      myReportToThem: null,  // dB int I sent (or will send) them
      theirReportToMe: null,  // dB int they sent me
      iCalledCQ: false,     // true if this QSO started from my CQ
      state: 'idle',        // idle|cq|called|reported|rogered|complete
      logged: false,
    };
  }

  // Build the reply to a CQ I clicked on: call them with my grid.
  // Returns { tx, qso } (a fresh QSO context that I am initiating).
  function answerCQ(cqMsg, myCall, myGrid) {
    const m = (typeof cqMsg === 'string') ? parseMsg(cqMsg) : cqMsg;
    if (!m || m.kind !== 'CQ' || !m.call) return null;
    const qso = newQso();
    qso.partner = m.call;
    qso.theirGrid = m.grid || null;
    qso.iCalledCQ = false;
    qso.state = 'called';
    return { tx: `${m.call} ${myCall} ${myGrid}`.toUpperCase(), qso };
  }

  // My own CQ. Resets the context to "waiting for an answer".
  function callCQ(myCall, myGrid) {
    const qso = newQso();
    qso.iCalledCQ = true;
    qso.state = 'cq';
    return { tx: `CQ ${myCall} ${myGrid}`.toUpperCase(), qso };
  }

  // Update the context from a message *I* just transmitted, so that an incoming
  // reply can be recognised as belonging to this QSO. Mutates and returns qso.
  function onTx(qso, text, myCall) {
    const m = parseMsg(text);
    myCall = (myCall || '').toUpperCase();
    if (m.kind === 'CQ') {
      qso.iCalledCQ = true; qso.partner = null; qso.state = 'cq'; qso.logged = false;
      qso.myReportToThem = qso.theirReportToMe = null; qso.theirGrid = null;
      return qso;
    }
    if (m.from === myCall && m.to) {
      // A directed message from me to "m.to".
      if (qso.partner && qso.partner !== m.to) {
        // New partner -> start a fresh QSO context around it.
        const fresh = newQso(); Object.assign(qso, fresh);
      }
      qso.partner = m.to;
      if (m.kind === 'grid')        { qso.state = 'called'; }
      else if (m.kind === 'report') { qso.myReportToThem = m.report; qso.state = 'reported'; }
      else if (m.kind === 'Rreport'){ qso.myReportToThem = m.report; qso.state = 'rogered'; }
      else if (m.kind === 'RR73' || m.kind === 'RRR') { qso.state = 'sentRR73'; }
      else if (m.kind === '73')     { qso.state = 'complete'; }
    }
    return qso;
  }

  // A message arrived. Decide whether it belongs to my QSO and, if so, what to
  // put in the TX field next. Mutates qso. Returns:
  //   { fill, complete, record }
  //     fill     - suggested TX string, or null (nothing to auto-fill)
  //     complete - true when this RX completed the QSO
  //     record   - a QSO log record when complete (else null)
  // myReportFromRssi is the report I computed from THIS rx's RSSI (their signal).
  function onRx(qso, text, myCall, myGrid, myReportFromRssi, opts) {
    opts = opts || {};
    const m = parseMsg(text);
    myCall = (myCall || '').toUpperCase();

    // Only react to traffic addressed to me.
    if (m.kind === 'CQ' || !m.to || m.to !== myCall) return { fill: null, complete: false, record: null };

    const from = m.from;
    // Adopt a partner if I'm CQing (anyone may answer) or I have none yet.
    if (!qso.partner) {
      if (qso.iCalledCQ || qso.state === 'idle' || qso.state === 'cq') qso.partner = from;
    }
    // If mid-QSO with someone else, ignore a third party (teaching-simple).
    if (qso.partner && qso.partner !== from) return { fill: null, complete: false, record: null };
    qso.partner = qso.partner || from;

    const R = (myReportFromRssi === undefined || myReportFromRssi === null) ? 0 : myReportFromRssi;
    let fill = null, complete = false, record = null;

    switch (m.kind) {
      case 'grid':                       // they answered my CQ (or called me) w/ grid
        qso.theirGrid = m.grid;
        qso.myReportToThem = R;
        qso.state = 'reported';
        fill = `${from} ${myCall} ${fmtReport(R)}`;
        break;
      case 'report':                     // they reported me, no roger yet
        qso.theirReportToMe = m.report;
        qso.myReportToThem = R;
        qso.state = 'rogered';
        fill = `${from} ${myCall} R${fmtReport(R)}`;
        break;
      case 'Rreport':                    // they rogered + reported -> I close with RR73
        qso.theirReportToMe = m.report;
        qso.state = 'sentRR73';
        fill = `${from} ${myCall} RR73`;
        complete = true;                 // both reports known; this closes it
        break;
      case 'RRR':
      case 'RR73':                       // they are closing -> I answer 73
        qso.state = 'complete';
        fill = `${from} ${myCall} 73`;
        complete = true;
        break;
      case '73':                         // bare 73 -> QSO done, nothing required
        qso.state = 'complete';
        complete = true;
        break;
      default:
        return { fill: null, complete: false, record: null };
    }

    if (complete && !qso.logged) {
      qso.logged = true;
      record = makeRecord(qso, myCall, myGrid, opts);
    } else if (complete) {
      complete = false;                  // already logged; don't double-count
    }
    return { fill, complete, record };
  }

  // Stateless: "what should I put in the TX field in response to THIS message?"
  // Used by click-to-respond. Does not touch any QSO context or the log; the
  // stateful onRx() remains the authority for the running QSO + completion.
  function suggestReply(text, myCall, myGrid, report) {
    const m = parseMsg(text);
    myCall = (myCall || '').toUpperCase();
    myGrid = (myGrid || '').toUpperCase();
    const R = (report === undefined || report === null) ? 0 : report;
    if (m.kind === 'CQ') return m.call ? `${m.call} ${myCall} ${myGrid}` : null;
    if (m.to !== myCall) return null;
    const from = m.from;
    switch (m.kind) {
      case 'grid':    return `${from} ${myCall} ${fmtReport(R)}`;
      case 'report':  return `${from} ${myCall} R${fmtReport(R)}`;
      case 'Rreport': return `${from} ${myCall} RR73`;
      case 'RRR':
      case 'RR73':    return `${from} ${myCall} 73`;
      default:        return null;     // bare 73 / call / other -> nothing to send
    }
  }

  // Assemble a log record from the finished context. dateTime defaults to now.
  function makeRecord(qso, myCall, myGrid, opts) {
    opts = opts || {};
    const d = opts.date ? new Date(opts.date) : new Date();
    const pad = (n, w) => String(n).padStart(w, '0');
    const qso_date = pad(d.getUTCFullYear(), 4) + pad(d.getUTCMonth() + 1, 2) + pad(d.getUTCDate(), 2);
    const time_on  = pad(d.getUTCHours(), 2) + pad(d.getUTCMinutes(), 2) + pad(d.getUTCSeconds(), 2);
    return {
      call: qso.partner || '',
      gridsquare: qso.theirGrid || '',
      rst_sent: qso.myReportToThem !== null ? fmtReport(qso.myReportToThem) : '',
      rst_rcvd: qso.theirReportToMe !== null ? fmtReport(qso.theirReportToMe) : '',
      qso_date, time_on,
      freq: opts.freq || null,
      mycall: (myCall || '').toUpperCase(),
      mygrid: (myGrid || '').toUpperCase(),
    };
  }

  const api = {
    GRID_RE, CALL_RE, isGrid, isReport, isRReport, isCall,
    RSSI_LO, RSSI_HI, rssiToReport, fmtReport,
    parseMsg, addressedTo,
    newQso, answerCQ, callCQ, onTx, onRx, suggestReply, makeRecord,
  };

  if (typeof module !== 'undefined' && module.exports) module.exports = api;
  else root.FT8 = api;
})(typeof self !== 'undefined' ? self : this);

</script>

<script>
const $ = (id) => document.getElementById(id);
let lastRxId = 0;
let qso = FT8.newQso();
let myCall = 'G5LSI', myGrid = 'IO93', myFreq = 439.9125;
let autoScroll = true;
let selected = null;   // {el, text, rssi} highlighted line the Answer button replies to

// form-urlencoded POST helper (the ESP32 WebServer parses these into named
// args reliably; a JSON body via arg("plain") does not work across core versions)
function postForm(url, obj){
  const body = new URLSearchParams();
  for (const k in obj) body.set(k, obj[k]);
  return fetch(url, {method:'POST', headers:{'Content-Type':'application/x-www-form-urlencoded'}, body});
}

function pad2(n){ return String(n).padStart(2,'0'); }
function utcHMS(d){ d=d||new Date(); return pad2(d.getUTCHours())+':'+pad2(d.getUTCMinutes())+':'+pad2(d.getUTCSeconds()); }
function esc(s){ return (s+'').replace(/[&<>]/g,c=>({'&':'&amp;','<':'&lt;','>':'&gt;'}[c])); }

// ---- RX window ----
const rxwin = $('rxwin');
rxwin.addEventListener('scroll',()=>{ autoScroll = (rxwin.scrollHeight - rxwin.scrollTop - rxwin.clientHeight) < 24; });

function classFor(text){
  const m = FT8.parseMsg(text);
  if (m.kind==='CQ') return 'cq';
  if (m.to === myCall) return 'me';
  return 'other';
}
function appendRx(text, rssi){
  const report = FT8.rssiToReport(rssi);
  const cls = classFor(text);
  const div = document.createElement('div');
  div.className = 'line rx '+cls;
  div.innerHTML = '<span class="t">'+utcHMS()+'</span>'+
    '<span class="db">'+FT8.fmtReport(report)+'</span>'+
    '<span class="rs">'+rssi+'</span>'+
    '<span class="m">'+esc(text)+'</span>';
  // Every received line can be selected; the Answer button replies to whatever
  // is currently highlighted.
  div.onclick = ()=>{ selectLine(div, text, rssi); };
  rxwin.appendChild(div);
  // Default highlight follows the last message addressed to our callsign.
  if (cls==='me') selectLine(div, text, rssi);
  trimWin();
  if (autoScroll) rxwin.scrollTop = rxwin.scrollHeight;
}
function appendTx(text){
  const div = document.createElement('div');
  div.className = 'line tx';
  div.innerHTML = '<span class="t">'+utcHMS()+'</span><span class="db"></span><span class="rs">TX</span><span class="m">'+esc(text)+'</span>';
  rxwin.appendChild(div);
  trimWin();
  if (autoScroll) rxwin.scrollTop = rxwin.scrollHeight;
}
function trimWin(){
  while (rxwin.children.length > 400){
    if (rxwin.firstChild === (selected && selected.el)) selected = null;
    rxwin.removeChild(rxwin.firstChild);
  }
}

// ---- selection (drives the Answer button) ----
function selectLine(el, text, rssi){
  if (selected && selected.el) selected.el.classList.remove('sel');
  el.classList.add('sel');
  selected = { el, text, rssi };
}

function setTx(s){ $('txField').value = (s||'').toUpperCase(); }

// ---- polling ----
async function pollStatus(){
  try{
    const s = await (await fetch('/api/status')).json();
    myCall = s.callsign; myGrid = s.grid; myFreq = s.freq;
    $('s-call').textContent = s.callsign;
    $('s-grid').textContent = s.grid;
    $('s-freq').textContent = s.freq.toFixed(4);
    $('s-sf').textContent = s.sf;
    $('s-bw').textContent = s.bw;
    $('s-rssi').textContent = s.rssi + ' dBm';
    $('s-state').textContent = s.state;
    $('s-statepill').className = 'pill' + (s.state==='TX'?' tx':'');
    $('s-rx').textContent = s.rx;
    $('s-tx').textContent = s.tx;
    $('s-qsos').textContent = s.qsos;
    // populate the form once on first load
    if (!$('c-call').dataset.init){
      $('c-call').value = (s.callsign && s.callsign!=='NOCALL') ? s.callsign : '';
      $('c-grid').value = s.grid;
      $('c-freq').value = s.freq.toFixed(4);
      $('c-sf').value = s.sf;
      $('c-bw').value = s.bw;
      $('c-call').dataset.init = '1';
    }
  }catch(e){}
}

async function pollRx(){
  try{
    const list = await (await fetch('/api/rx?since='+lastRxId)).json();
    for (const m of list){
      if (m.id > lastRxId) lastRxId = m.id;
      appendRx(m.text, m.rssi);
      handleRx(m.text, m.rssi);
    }
  }catch(e){}
}

async function handleRx(text, rssi){
  const report = FT8.rssiToReport(rssi);
  const res = FT8.onRx(qso, text, myCall, myGrid, report, { freq: myFreq });
  if (res.fill) setTx(res.fill);
  if (res.complete && res.record){
    try{
      await postForm('/api/log', res.record);
      loadLog();
      $('logMsg').textContent = 'logged '+res.record.call;
    }catch(e){}
  }
}

// ---- actions ----
async function applyCfg(){
  const body = {
    callsign: ($('c-call').value||'G5LSI').trim().toUpperCase(),
    grid: ($('c-grid').value||'IO93').trim().toUpperCase(),
    freq: parseFloat($('c-freq').value)||439.9125,
    sf: parseInt($('c-sf').value,10)||12,
    bw: parseFloat($('c-bw').value)||125,
  };
  await postForm('/api/config', body);
  myCall = body.callsign; myGrid = body.grid; myFreq = body.freq;
  pollStatus();
}

async function doTx(){
  const text = $('txField').value.trim().toUpperCase();
  if (!text){ $('txMsg').textContent='nothing to send'; return; }
  $('txField').value = text;          // normalise, but keep it (allows re-TX)
  $('txMsg').textContent = 'sending...';
  try{
    const r = await (await postForm('/api/tx', {text})).json();
    $('txMsg').textContent = r.ok ? 'sent' : ('failed: '+(r.error||r.status));
    if (r.ok){ appendTx(text); FT8.onTx(qso, text, myCall); }
  }catch(e){ $('txMsg').textContent='request failed'; }
}

function doCQ(){
  const c = FT8.callCQ(myCall, myGrid);
  qso = c.qso;
  setTx(c.tx);
}
function doAnswer(){
  if (!selected){ $('txMsg').textContent='no message selected to answer'; return; }
  const { text, rssi } = selected;
  const m = FT8.parseMsg(text);
  if (m.kind==='CQ'){
    const a = FT8.answerCQ(text, myCall, myGrid);
    if (a){ qso = a.qso; setTx(a.tx); $('txMsg').textContent='answering '+(m.call||'CQ'); }
    else $('txMsg').textContent='cannot answer that';
    return;
  }
  // a message addressed to us (grid / report / R-report / RR73 ...): reply in kind
  const r = FT8.suggestReply(text, myCall, myGrid, FT8.rssiToReport(rssi));
  if (r){ setTx(r); $('txMsg').textContent='answering '+(m.from||''); }
  else $('txMsg').textContent='nothing to answer for that line';
}
function resetQso(){ qso = FT8.newQso(); $('txMsg').textContent='QSO reset'; }

async function loadLog(){
  try{
    const list = await (await fetch('/api/log')).json();
    const tb = $('logRows'); tb.innerHTML='';
    for (let i=list.length-1;i>=0;i--){
      const q = list[i];
      const tr = document.createElement('tr');
      const dt = q.qso_date ? (q.qso_date.slice(0,4)+'-'+q.qso_date.slice(4,6)+'-'+q.qso_date.slice(6,8)) : '';
      const tm = q.time_on ? (q.time_on.slice(0,2)+':'+q.time_on.slice(2,4)+':'+q.time_on.slice(4,6)) : '';
      tr.innerHTML = '<td>'+dt+'</td><td>'+tm+'</td><td>'+esc(q.call)+'</td><td>'+esc(q.gridsquare)+'</td>'+
        '<td>'+esc(q.rst_sent)+'</td><td>'+esc(q.rst_rcvd)+'</td><td>'+(q.freq?q.freq.toFixed(4):'')+'</td>';
      tb.appendChild(tr);
    }
  }catch(e){}
}

$('applyBtn').onclick = applyCfg;
$('txBtn').onclick = doTx;
$('txField').addEventListener('keydown',(e)=>{ if(e.key==='Enter') doTx(); });
$('cqBtn').onclick = doCQ;
$('ansBtn').onclick = doAnswer;
$('dxBtn').onclick = resetQso;
$('adifBtn').onclick = ()=>{ window.open('/log.adi','_blank'); };
$('logClrBtn').onclick = async()=>{ await fetch('/api/log/clear',{method:'POST'}); loadLog(); $('logMsg').textContent='log cleared'; };

pollStatus(); pollRx(); loadLog();
setInterval(pollStatus, 1000);
setInterval(pollRx, 700);
</script>
</body>
</html>

)FT8PAGE";
