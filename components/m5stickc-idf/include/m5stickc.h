/**
 * m5stickc.c
 *
 * (C) 2019 - Hades2001 <hades2001@foxmail.com>
 * This code is licensed under the MIT License.
 */

#ifndef _STICKC_H_
#define _STICKC_H_

#include "esp_system.h"
#include "esp_err.h"
#include "esp_log.h"

#include "button.h"
#include "util/include/wire.h"
#include "util/include/MPU6886.h"
#include "util/include/AXP192.h"
#include "util/include/spi_master_lobo.h"
#include "util/include/tftspi.h"
#include "util/include/tft.h"

extern esp_event_loop_handle_t event_loop;   /*!< Event loop for M5 device-specific events */
extern spi_lobo_device_handle_t m5_display_spi;    /*!< SPI device handle */

#define M5DISPLAY_TYPE DISP_TYPE_ST7735S  /*!< Display type for display driver */
#define M5DISPLAY_WIDTH 160               /*!< Display width in pixels after rotation */
#define M5DISPLAY_HEIGHT 80               /*!< Display height in pixels after rotation */



#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief   Initialize M5StickC
 *
 *          Initializes power management, display and buttons.
 *
 * @return  ESP_OK success
 *          ESP_FAIL errors found
 */
esp_err_t M5Init();
esp_err_t EventInit();

#ifdef __cplusplus
}
#endif

#endif // _M5STICKC_H_
