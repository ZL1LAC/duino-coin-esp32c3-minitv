// LilyGO T-Beam v1.1 — AXP192 battery voltage / presence (I2C 0x34)
#ifndef TBEAM_BATTERY_H
#define TBEAM_BATTERY_H

#if defined(LILYGO_T_BEAM_V11)

#include <Arduino.h>
#include "TBeamDisplay.h"

#define AXP192_REG_POWER_STATUS   0x00
#define AXP192_REG_CHARGE_STATUS  0x01
#define AXP192_REG_ADC_ENABLE     0x82
#define AXP192_REG_ADC_ENABLE2    0x83
#define AXP192_REG_BAT_VOLTAGE    0x78

// LiPo present: AXP192 bit set and plausible cell voltage (USB-only reads ~0 V).
#define TBEAM_BATTERY_MIN_V       3.05f

static bool tbeam_battery_ready = false;
static bool tbeam_battery_present = false;
static int tbeam_battery_pct = -1;
static bool tbeam_battery_charging = false;
static uint32_t tbeam_battery_last_ms = 0;

#if defined(SERIAL_PRINTING)
static bool tbeam_battery_dbg_last = false;
#endif

inline uint16_t tbeam_axp_read12(uint8_t reg) {
  uint8_t hi = 0;
  uint8_t lo = 0;
  if (!tbeam_axp_read(reg, &hi) || !tbeam_axp_read(reg + 1, &lo)) {
    return 0;
  }
  return ((uint16_t)hi << 4) | (lo & 0x0F);
}

inline float tbeam_battery_voltage_v() {
  return tbeam_axp_read12(AXP192_REG_BAT_VOLTAGE) * 0.0011f;
}

inline bool tbeam_axp_vbus_in() {
  uint8_t powerStatus = 0;
  if (!tbeam_axp_read(AXP192_REG_POWER_STATUS, &powerStatus)) {
    return false;
  }
  // Bit 5: VBUS present (USB / 5 V input)
  return (powerStatus & 0x20) != 0;
}

inline float tbeam_vbus_voltage_v() {
  return tbeam_axp_read12(0x5A) * 0.0017f;
}

inline int tbeam_voltage_to_pct(float volts) {
  if (volts < 3.248088f) {
    return 0;
  }
  int pct = (int)((volts - 3.120712f) * 100.0f);
  if (pct > 100) {
    pct = 100;
  }
  return pct;
}

inline void tbeam_battery_update() {
  if (!tbeam_battery_ready) {
    return;
  }
  uint32_t now = millis();
  if (tbeam_battery_last_ms != 0 && (now - tbeam_battery_last_ms) < 3000) {
    return;
  }
  tbeam_battery_last_ms = now;

  uint8_t chargeStatus = 0;
  if (!tbeam_axp_read(AXP192_REG_CHARGE_STATUS, &chargeStatus)) {
    return;
  }

  bool hwPresent = (chargeStatus & 0x20) != 0;
  float volts = tbeam_battery_voltage_v();
  bool vbus = tbeam_axp_vbus_in();
  float vbusV = vbus ? tbeam_vbus_voltage_v() : 0.0f;

  bool absent = !hwPresent || volts < TBEAM_BATTERY_MIN_V;
  // Empty holder on USB: bat ADC can track VBUS (~4.0–4.2 V) even with no cell
  if (!absent && vbus && volts >= 3.9f && vbusV >= 3.5f) {
    float delta = volts - vbusV;
    if (delta < 0.0f) {
      delta = -delta;
    }
    if (delta < 0.25f) {
      absent = true;
    }
  }

  if (absent) {
    tbeam_battery_present = false;
    tbeam_battery_pct = -1;
    tbeam_battery_charging = false;
  } else {
    tbeam_battery_present = true;
    tbeam_battery_pct = tbeam_voltage_to_pct(volts);
    tbeam_battery_charging = (chargeStatus & 0x40) != 0;
  }

  #if defined(SERIAL_PRINTING)
    if (tbeam_battery_present != tbeam_battery_dbg_last) {
      Serial.printf("Battery: cell=%s hw=%d %.2fV usb=%d pct=%d chg=%d\n",
                    tbeam_battery_present ? "yes" : "no",
                    hwPresent ? 1 : 0, volts, vbus ? 1 : 0,
                    tbeam_battery_pct, tbeam_battery_charging ? 1 : 0);
      Serial.flush();
      tbeam_battery_dbg_last = tbeam_battery_present;
    }
  #endif
}

inline void tbeam_battery_init() {
  tbeam_axp_write(AXP192_REG_ADC_ENABLE, 0xFF);
  tbeam_axp_write(AXP192_REG_ADC_ENABLE2, 0xFF);
  delay(20);

  uint8_t chipId = 0;
  tbeam_battery_ready = tbeam_axp_read(0x03, &chipId);
  #if defined(SERIAL_PRINTING)
    if (tbeam_battery_ready) {
      Serial.println("Battery: AXP192 ADC enabled");
    } else {
      Serial.println("Battery: AXP192 not responding");
    }
    Serial.flush();
  #endif

  tbeam_battery_present = false;
  tbeam_battery_pct = -1;
  tbeam_battery_charging = false;

  if (tbeam_battery_ready) {
    tbeam_battery_last_ms = 0;
    tbeam_battery_update();
  }
}

// Always safe to call; present=false => draw battery outline with X.
inline void tbeam_battery_get(bool &present, int &pct, bool &charging) {
  present = false;
  pct = 0;
  charging = false;
  if (!tbeam_battery_ready) {
    return;
  }
  tbeam_battery_update();
  present = tbeam_battery_present;
  pct = (tbeam_battery_pct < 0) ? 0 : tbeam_battery_pct;
  charging = tbeam_battery_charging;
}

#endif
#endif
