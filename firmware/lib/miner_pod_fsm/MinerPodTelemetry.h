#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "MinerPodState.h"

// OTF-1 (telemetry-protocol) fleet integration for the mining pod.
//
// Header-only, integer-only formatting so it builds unchanged on the Uno,
// the ESP32 and the native test environment (no float printf needed).
//
// The pod keeps no clock, so the caller passes the RFC 3339 timestamp (from
// its radio/RTC/ground link) and the per-source sequence counter.

// Reason strings are exactly the OTF-1 `ack.reason` enum values.
// Returns nullptr when the pod can take a new task (it is Idle).
inline const char* minerPodAckReason(MinerPodState s) {
  if (s == Fault) return "safe-mode";
  if (s != Idle) return "busy";
  return nullptr;
}

// Writes an OTF-1 `status` packet as JSON into buf.
//   zone: an OTF-1 location.zone value, e.g. "NEA" or "belt".
//   link: true = coordinator contact, false = running on local defaults.
// Returns the number of bytes written (excluding the NUL), or 0 if the
// packet did not fit in cap (buf is then left NUL-terminated and empty).
inline size_t minerPodStatusJson(char* buf, size_t cap, const char* nodeId, uint32_t seq,
                                 const char* timestamp, MinerPodState state,
                                 const char* zone, bool link) {
  if (buf == nullptr || cap == 0) return 0;
  // A Fault means local fault response has taken over: report "safe".
  const char* mode = (state == Fault) ? "safe" : "nominal";
  const int n = snprintf(
      buf, cap,
      "{\"version\":\"OTF-1\",\"kind\":\"status\",\"timestamp\":\"%s\","
      "\"source\":{\"id\":\"%s\",\"role\":\"node\"},\"seq\":%lu,"
      "\"node\":{\"type\":\"miner\",\"mode\":\"%s\",\"phase\":\"%s\","
      "\"link\":\"%s\",\"capabilities\":[\"process\",\"refine\"]},"
      "\"location\":{\"zone\":\"%s\"}}",
      timestamp, nodeId, (unsigned long)seq, mode, minerPodStateName(state),
      link ? "connected" : "autonomous", zone);
  if (n < 0 || (size_t)n >= cap) {
    buf[0] = '\0';
    return 0;
  }
  return (size_t)n;
}
