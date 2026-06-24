# Flash pre-built firmware

How to flash **merged** release binaries from [GitHub Releases](https://github.com/ZL1LAC/duino-coin-boards/releases) or a local `firmware/dist/` build.

Pick the file for your hardware:

| Board | Binary |
|-------|--------|
| Spotpear ESP32-C3 1.44" Mini TV (ST7735) | `esp32c3-minitv-firmware.bin` | esp32c3 |
| ESP32-2424S012 round (GC9A01) | `esp32c3-round128-firmware.bin` | esp32c3 |
| LilyGO T-Deck (ST7789) | `lilygo-t-deck-firmware.bin` | esp32s3 |
| Heltec WiFi LoRa 32 (V2) (SSD1306) | `heltec-wifi-lora-32-v2-firmware.bin` | esp32 |
| LilyGO T-Deck Pro (e-paper) | `lilygo-t-deck-pro-firmware.bin` | esp32s3 |
| LilyGO T-Beam v1.1 (SSD1306) | `lilygo-t-beam-v11-firmware.bin` | esp32 |

Release builds enable **captive portal** setup — see [firmware/README.md](../firmware/README.md). Use the **esptool chip** column (`--chip esp32c3`, `esp32`, or `esp32s3`).

---

## Option A — esptool (recommended)

Install Python 3, then:

```bash
pip install esptool
```

**Windows (PowerShell):**

```powershell
python -m esptool --chip esp32c3 --port COM14 --baud 115200 write_flash 0x0 esp32c3-minitv-firmware.bin
```

**macOS / Linux:**

```bash
python3 -m esptool --chip esp32c3 --port /dev/ttyUSB0 --baud 115200 write_flash 0x0 esp32c3-minitv-firmware.bin
```

Replace the port and filename for your board.

### If upload fails

1. Close Serial Monitor / anything else using the COM port.
2. Use **115200** baud.
3. **Manual boot mode:** hold **BOOT**, tap **RESET**, release **BOOT**, run the command again.
4. On boards with two COM ports, try the one that appears when you plug in USB.

---

## Option B — ESP Web Flasher

1. Open [esptool-js](https://espressif.github.io/esptool-js/) or [ESP Launchpad](https://espressif.github.io/esp-launchpad/).
2. Connect the board (Chrome / Edge, WebSerial).
3. Chip: **ESP32-C3**.
4. Flash address **0x0**, select the `*-merged-flash.bin` file.
5. Start flash.

---

## Option C — Arduino IDE (source build)

If you need custom credentials compiled in (no captive portal):

1. Copy `devices/<id>/Settings.h.example` → `devices/<id>/Settings.h` and edit WiFi + username.
2. `.\scripts\sync-device.ps1 <id> miner` (or `miner-portal` to match release behaviour).
3. Follow the board README for TFT_eSPI setup.
4. Open `ESP_Code/ESP_Code.ino` and upload.

---

## After flashing

| Step | What to expect |
|------|----------------|
| Power on | Boot splash → mining UI or “Waiting for WiFi…” |
| Release build | WiFi AP **`Duino-Coin`** — configure via captive portal |
| Serial Monitor | **115200** baud, USB CDC On Boot **Enabled** |
| Mining | Hashrate varies by MCU (e.g. ~16–18 kH/s on ESP32-C3 TFT boards) |

Board-specific troubleshooting: [esp32c3-minitv](../devices/esp32c3-minitv/) · [esp32c3-round128](../devices/esp32c3-round128/)

---

## Build artifacts layout

Local builds (`scripts/build-firmware.sh`) write:

```
firmware/dist/<board>/
  <board>-merged-flash.bin   ← flash this
  manifest.json
  FLASH.txt
  ESP_Code.ino.bin           ← individual partitions (advanced)
  ...
```

The merged file includes bootloader, partition table, and application — one shot at **0x0**.