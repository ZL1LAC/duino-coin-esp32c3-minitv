# Testbench

Two stages: **generic hardware test** first, then the **full miner** when the board looks good.

## 1. Generic test (new boards / bring-up)

Minimal sketch — color bars, board name, serial ticks. No WiFi, no mining.

1. `copy testbench\<board-id>\TestConfig.h.example testbench\<board-id>\TestConfig.h`
2. `.\scripts\sync-testbench-test.ps1 <board-id>`
3. For TFT_eSPI boards: `.\scripts\setup-tft-espi.ps1 <board-id>` (skip for OLED / e-paper)
4. Open **`testbench/GenericTest/GenericTest.ino`** · upload · Serial **115200**

See [testbench/GenericTest/README.md](GenericTest/README.md).

## 2. Full miner (when display works)

1. Edit `testbench/<board-id>/Settings.h` (from `Settings.h.example`).
2. `.\scripts\sync-testbench-settings.ps1 <board-id>`
3. Open **`ESP_Code/ESP_Code.ino`** · upload.

When mining works, set `firmware.enabled: true` in [boards/registry.json](../boards/registry.json).

## Boards

| Board | Generic test | Full miner settings | TFT setup |
|-------|--------------|---------------------|-----------|
| Spotpear Mini TV | [TestConfig.h.example](esp32c3-minitv/TestConfig.h.example) | [Settings.h.example](esp32c3-minitv/Settings.h.example) | [tft_setup.h](esp32c3-minitv/tft_setup.h) |
| ESP32-2424S012 round | [TestConfig.h.example](esp32c3-round128/TestConfig.h.example) | [Settings.h.example](esp32c3-round128/Settings.h.example) | [tft_setup.h](esp32c3-round128/tft_setup.h) |
| LilyGO T-Deck | [TestConfig.h.example](lilygo-t-deck/TestConfig.h.example) | [Settings.h.example](lilygo-t-deck/Settings.h.example) | [tft_setup.h](lilygo-t-deck/tft_setup.h) |
| LilyGO T-Deck Pro | [TestConfig.h.example](lilygo-t-deck-pro/TestConfig.h.example) | [Settings.h.example](lilygo-t-deck-pro/Settings.h.example) | — (GxEPD2 e-paper) |
| Heltec WiFi LoRa 32 V2 | [TestConfig.h.example](heltec-wifi-lora-32-v2/TestConfig.h.example) | [Settings.h.example](heltec-wifi-lora-32-v2/Settings.h.example) | — (U8g2 OLED) |

IDs match [boards/registry.json](../boards/registry.json).

## Layout

```
testbench/
├── README.md
├── GenericTest/              GenericTest.ino + BoardInit.h (open for bring-up)
├── _template/
├── esp32c3-minitv/
├── esp32c3-round128/
├── lilygo-t-deck/
├── lilygo-t-deck-pro/
└── heltec-wifi-lora-32-v2/
```

Each board folder has `Settings.h.example`, optional `TestConfig.h.example`, optional `tft_setup.h` (TFT_eSPI boards only), and `README.md`.

## Add another board

1. `bash scripts/new-board.sh <id> "<name>"`
2. Add `testbench/<id>/` with `TestConfig.h.example` + `Settings.h.example`
3. Copy `tft_setup.h` from `boards/<id>/` when using TFT_eSPI
4. Add a row to the table above
