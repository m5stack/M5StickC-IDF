/**
 * m5event.h
 *
 * (C) 2019 - Pablo Bacho <pablo@pablobacho.com>
 * This code is licensed under the MIT License.
 */

#ifndef _EVENT_H_
#define _EVENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_log.h"
#include "esp_event.h"

extern esp_event_loop_handle_t m5_event_loop;   /*!< Event loop for M5 device-specific events */

esp_err_t m5event_init();

#ifdef __cplusplus
}
#endif

#endif // _M5EVENT_H_
