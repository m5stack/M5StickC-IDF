/**
 * m5stickc.c - ESP-IDF component to work with M5
 *
 * (C) 2019 - Pablo Bacho <pablo@pablobacho.com>
 * This code is licensed under the MIT License.
 */

#include "m5stickc.h"

static const char *TAG = "m5stickc";

esp_event_loop_handle_t m5_event_loop;

esp_err_t m5_init(m5stickc_config_t * config) {
    esp_err_t e;
    uint8_t error_count = 0;

    m5event_init();

    ESP_LOGI(TAG, "Setting up I2C");
	e = InitI2CWire(&wire0);
    if(e == ESP_OK) {
        if(e == ESP_OK) {
            // Init power management
            e = m5power_init(&config->power);
            if(e == ESP_OK) {
                ESP_LOGD(TAG, "Power manager initialized");
            } else {
                ESP_LOGE(TAG, "Error initializing power manager");
                ++error_count;
            }
        } else {
            ESP_LOGE(TAG, "Error during I2C driver installation: %s", esp_err_to_name(e));
            ++error_count;
        }
    } else {
        ESP_LOGE(TAG, "Error setting up I2C: %s", esp_err_to_name(e));
        ++error_count;
    }
    // Init button
    e = m5button_init();
    if(e == ESP_OK) {
        ESP_LOGD(TAG, "Button initialized");
    } else {
        ESP_LOGE(TAG, "Error initializing button");
        ++error_count;
    }

	MPU6886Init();

    // Init display
    e = m5display_init();
    if(e == ESP_OK) {
        ESP_LOGD(TAG, "Display initialized");
    } else {
        ESP_LOGE(TAG, "Error initializing display");
        ++error_count;
    }

    if(error_count == 0) {
        ESP_LOGD(TAG, "M5StickC initialized successfully");
        return ESP_OK;
    } else {
        ESP_LOGE(TAG, "%d errors found while initializing M5StickC", error_count);
        return ESP_FAIL;
    }
}