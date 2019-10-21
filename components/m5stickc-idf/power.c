/**
 * m5power.c
 *
 * (C) 2019 - Pablo Bacho <pablo@pablobacho.com>
 * This code is licensed under the MIT License.
 */

#include "power.h"

static const char * TAG = "m5power";

uint8_t InitStickC[28] = {
	'A','X','P',
	0x06,0,11,
	0x10,0xff, //1
	0x28,0xCC, //2
	0x82,0xff, //3
	0x33,0xC1, //4
	0xB8,0x80, //5
	0x12,0x4D, //6
	0x36,0x0C, //7
	0x90,0x02, //8
	0x30,0xec, //9
	0x39,0xFC, //10
	0x35,0xA2, //11
};

esp_err_t m5power_init(m5power_config_t * config) {

	AxpinitFromList(&wire0, InitStickC );
	return ESP_OK;
}

esp_err_t m5power_register_read(uint8_t register_address, uint8_t * register_content)
{
    *register_content = AxpRead8bit(&wire0, register_address);
    return ESP_OK;
}

esp_err_t m5power_register_write(uint8_t register_address, uint8_t register_content)
{
    AxpWriteByte(&wire0, register_address, register_content);
    return ESP_OK;
}

esp_err_t m5power_register_set_bits(uint8_t register_address, uint8_t bits_to_set)
{
    esp_err_t e;
    uint8_t register_content;

    e = m5power_register_read(register_address, &register_content);
    if(e != ESP_OK) {
        return ESP_FAIL;
    }

    register_content |= bits_to_set;

    e = m5power_register_write(register_address, register_content);
    if(e != ESP_OK) {
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t m5power_register_clear_bits(uint8_t register_address, uint8_t bits_to_clear)
{
    esp_err_t e;
    uint8_t register_content;

    e = m5power_register_read(register_address, &register_content);
    if(e != ESP_OK) {
        return ESP_FAIL;
    }

    register_content &= ~bits_to_clear;

    e = m5power_register_write(register_address, register_content);
    if(e != ESP_OK) {
        return ESP_FAIL;
    }

    return ESP_OK;
}