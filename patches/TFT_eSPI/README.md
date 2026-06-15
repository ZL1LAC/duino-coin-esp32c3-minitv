# TFT_eSPI patches for Spotpear ESP32-C3 (Arduino ESP32 3.x)

## 1. Board setup file

Copy `Setup_Spotpear_ESP32C3_144.h` into:

```
Arduino/libraries/TFT_eSPI/User_Setups/
```

Edit `User_Setup_Select.h` and include it (comment out other setups):

```cpp
#include <User_Setups/Setup_Spotpear_ESP32C3_144.h>
```

## 2. ESP32-C3 SPI crash fix (required on core 3.x)

Without this patch, `tft.init()` crashes with `Store access fault` / `MTVAL: 0x10`.

In `Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32_C3.h`, after the includes, add:

```cpp
#if CONFIG_IDF_TARGET_ESP32C3
  #ifdef REG_SPI_BASE
    #undef REG_SPI_BASE
  #endif
  #define REG_SPI_BASE(i) DR_REG_SPI2_BASE
#endif
```

Also ensure `#include "soc/soc.h"` is present if `DR_REG_SPI2_BASE` is undefined.

## 3. Firmware init

`DisplayHal.h` calls `tft.init(INITR_GREENTAB)` for correct 128×128 alignment on this panel.
