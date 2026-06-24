// LilyGO T-Beam v1.1 — AXP192 PMU + SSD1306 OLED on I2C (SDA=21, SCL=22)
// Minimal AXP192 bring-up via Wire (no external AXP library required).
#ifndef TBEAM_DISPLAY_H
#define TBEAM_DISPLAY_H

#include <Arduino.h>
#include <Wire.h>

#ifndef TBEAM_I2C_SDA
  #define TBEAM_I2C_SDA 21
#endif
#ifndef TBEAM_I2C_SCL
  #define TBEAM_I2C_SCL 22
#endif

#ifndef AXP192_I2C_ADDR
  #define AXP192_I2C_ADDR 0x34
#endif

static inline bool tbeam_axp_write(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(AXP192_I2C_ADDR);
  Wire.write(reg);
  Wire.write(val);
  return Wire.endTransmission() == 0;
}

static inline bool tbeam_axp_read(uint8_t reg, uint8_t *val) {
  Wire.beginTransmission(AXP192_I2C_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) {
    return false;
  }
  if (Wire.requestFrom((uint16_t)AXP192_I2C_ADDR, (uint8_t)1) != 1) {
    return false;
  }
  *val = Wire.read();
  return true;
}

static inline uint8_t tbeam_axp_mv_to_reg(uint16_t mv) {
  if (mv < 700) {
    mv = 700;
  }
  if (mv > 3500) {
    mv = 3500;
  }
  return (uint8_t)((mv - 700) / 25);
}

inline bool tbeam_axp_init() {
  Wire.begin(TBEAM_I2C_SDA, TBEAM_I2C_SCL);
  delay(20);

  uint8_t chipId = 0;
  if (!tbeam_axp_read(0x03, &chipId)) {
    return false;
  }

  // DCDC1 -> OLED header 3V3, DCDC3 -> ESP32 3V3 (T-Beam v1.1 AXP192 wiring)
  if (!tbeam_axp_write(0x26, tbeam_axp_mv_to_reg(3300))) {
    return false;
  }
  if (!tbeam_axp_write(0x27, tbeam_axp_mv_to_reg(3300))) {
    return false;
  }

  uint8_t pwr = 0;
  if (!tbeam_axp_read(0x12, &pwr)) {
    return false;
  }
  pwr |= (1 << 0);  // DCDC1 on
  pwr |= (1 << 2);  // DCDC3 on
  if (!tbeam_axp_write(0x12, pwr)) {
    return false;
  }

  uint8_t led = 0;
  if (tbeam_axp_read(0x32, &led)) {
    led = (led | 0x40) & 0xFC;  // battery monitor on, charge LED off
    tbeam_axp_write(0x32, led);
  }

  // ADC for battery voltage reads (TBeamBattery.h)
  tbeam_axp_write(0x82, 0xFF);

  return true;
}

inline void tbeam_board_early_init() {
  tbeam_axp_init();
  delay(50);
}

template<typename U8G2>
inline void tbeam_display_init(U8G2 &u8g2) {
  tbeam_board_early_init();
  u8g2.begin();
  u8g2.setBusClock(400000);
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  u8g2.sendBuffer();
}

template<typename U8G2>
inline void tbeam_display_reinit_core0(U8G2 &u8g2) {
  // Wire/u8g2 must be used from one core on ESP32 — hand off after setup drew boot screen.
  Wire.end();
  delay(20);
  Wire.begin(TBEAM_I2C_SDA, TBEAM_I2C_SCL);
  u8g2.begin();
  u8g2.setBusClock(400000);
}

#endif
