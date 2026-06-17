# Duino-Coin — ESP32-C3 display miners

Fork of [Duino-Coin](https://github.com/revoxhere/duino-coin) with live mining UI for ESP32-C3 boards with small TFT screens.

Based on Duino-Coin **ESP_Code 4.3**.

## Supported boards

| Board | Display | Resolution | Guide |
|-------|---------|------------|--------|
| Spotpear ESP32-C3 1.44" Mini TV | ST7735 | 128×128 | [README_ESP32C3_MiniTV.md](README_ESP32C3_MiniTV.md) |
| ESP32-2424S012 round (2424S012C) | GC9A01 + CST816D touch | 240×240 | [README_ESP32C3_Round128.md](README_ESP32C3_Round128.md) |

Both use the **same mining layout** (designed in [Lopaka](https://lopaka.app)). See [DISPLAYS.md](DISPLAYS.md) to switch boards.

## Quick start

1. Clone this repo.
2. Copy `Settings.h.example` → `Settings.h` and set your Duino-Coin username, WiFi, etc.
3. Enable **one** display in `Settings.h`: `DISPLAY_ST7735` or `DISPLAY_GC9A01`.
4. Follow the README for your board (TFT_eSPI setup + patches).
5. Open `ESP_Code.ino`, select **ESP32C3 Dev Module**, upload.

**Arduino settings:** USB CDC On Boot **Enabled**, partition **Huge APP**, upload **115200**, Serial Monitor **115200**.

## What's in this repo

| Path | Purpose |
|------|---------|
| `DisplayHal.h` | Mining UI for ST7735 and GC9A01 (scaled Lopaka layout) |
| `CST816D.cpp` / `CST816D.h` | Touch driver (factory sequence) for round board |
| `ST7735_setup.h` / `GC9A01_setup.h` | TFT_eSPI pin reference |
| `patches/TFT_eSPI/` | User setup files + ESP32-C3 SPI crash fix |
| `Lopaka/` | Importable layout code for [Lopaka](https://lopaka.app) |
| `DISPLAYS.md` | Board comparison and touch gestures |

## Mining screen

```
┌──────────────────────────────┐
│ ▂▄▆█  10ms   darkhunter~     │
├──────────────────────────────┤
│  63.4              kH/s      │
├──────────────────────────────┤
│  6400          diff          │
│  0.2           sh/s          │
├──────────────────────────────┤
│ ✓ 110/110         (100%)     │
├──────────────────────────────┤
│ 192.168.x.x        0h9m24s   │
└──────────────────────────────┘
```

## Touch (round board only)

Tap = rotate · Swipe up/down = brightness · BOOT (GPIO9) = rotate

## Upstream

Mines on the [Duino-Coin](https://duinocoin.com) network. Official project: [revoxhere/duino-coin](https://github.com/revoxhere/duino-coin).

## License

Same as upstream Duino-Coin — see [LICENSE](LICENSE).
