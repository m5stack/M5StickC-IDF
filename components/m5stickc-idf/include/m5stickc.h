/**
 * m5stickc.h - ESP-IDF component to work with M5StickC
 *
 * Include this header file to use the component.
 *
 * (C) 2019 - Pablo Bacho <pablo@pablobacho.com>
 * This code is licensed under the MIT License.
 */

#ifndef _STICKC_H_
#define _STICKC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_system.h"
#include "esp_err.h"
#include "esp_log.h"

#include "power.h"
#include "button.h"
#include "display.h"
#include "event.h"
#include "util/include/wire.h"
#include "util/include/MPU6886.h"

typedef struct {
    m5power_config_t power;
} m5stickc_config_t;

#define M5STICKC_CONFIG_DEFAULT() { \
    .power = M5POWER_CONFIG_DEFAULT() \
    };

/**
 * @brief   Initialize M5StickC
 *
 *          Initializes power management, display and buttons.
 *
 * @return  ESP_OK success
 *          ESP_FAIL errors found
 */
esp_err_t m5_init(m5stickc_config_t * config);

#ifdef __cplusplus
}
#endif

#endif // _M5STICKC_H_
