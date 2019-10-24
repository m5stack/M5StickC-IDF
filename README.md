# M5stickC-IDF

> ESP-IDF component to work with M5StickC  
> inf: This item is only available for ESP IDF versions below 3.3  
> Compiler Environment:esp-idf-v3.2.3  
> TFT lib base [loboris TFT library ](https://github.com/loboris/ESP32_TFT_library)

---
## File Tree
```
.
├── components
│   └── m5stickc-idf
│       ├── include
│       │   ├── button.h
│       │   ├── i2smic.h
│       │   └── m5stickc.h
│       ├── util
│       │   ├── font
│       │   │   ├── DefaultFont.c
│       │   │   ├── DejaVuSans18.c
│       │   │   ├── DejaVuSans24.c
│       │   │   ├── SmallFont.c
│       │   │   ├── Ubuntu16.c
│       │   │   ├── comic24.c
│       │   │   ├── def_small.c
│       │   │   ├── default_tiny_9pt.c
│       │   │   ├── minya24.c
│       │   │   └── tooney32.c
│       │   ├── include
│       │   │   ├── AXP192.h
│       │   │   ├── MPU6886.h
│       │   │   ├── axp192_reg.h
│       │   │   ├── spi_master_lobo.h
│       │   │   ├── stmpe610.h
│       │   │   ├── tft.h
│       │   │   ├── tftspi.h
│       │   │   └── wire.h
│       │   ├── AXP192.c
│       │   ├── MPU6886.c
│       │   ├── i2smic.c
│       │   ├── spi_master_lobo.c
│       │   ├── tft.c
│       │   ├── tftspi.c
│       │   └── wire.c
│       ├── CMakeLists.txt
│       ├── LICENSE
│       ├── button.c
│       ├── component.mk
│       └── m5stickc.c
├── main
│   ├── CMakeLists.txt
│   ├── Kconfig.projbuild
│   ├── component.mk
│   └── main.c
├── CMakeLists.txt
├── Makefile
└── sdkconfig
```

## Versions
* Version 0.0.1 2019-10-21@Hades
	* Creat this project
	* Add LCD Device Library
	* Add Wire(I2C) Library
	* Add AXP192 Library
	* Add MPU6886 Library
---
* Version 0.0.2 2019-10-22@Hades
	* Edit AXP192 Library 
	```C
	float AXP192GetVinVoltage(wire_t *wire)
	```

	* Edit main-Examples 
		* Add Demo for LCD
		* Add Demo for AXP192
		* Add Demo for MPU6886
		* Add Demo for Button
---
* Version 1.0.1 2019-10-23@Hades

	* remove power.c & power.h
	* remove event.c & event.h
	* remove display.c & display.h

	* Edit I2S micophone Examles
	```C
	void MicRecordTest()
	```
	* edit m5stick.c 
