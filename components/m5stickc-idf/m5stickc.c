/**
 * m5stickc.h
 *
 * (C) 2019 - Hades2001 <hades2001@foxmail.com>
 * This code is licensed under the MIT License.
 */

#include "m5stickc.h"

static const char *TAG = "M5StickC";

esp_event_loop_handle_t event_loop;
spi_lobo_device_handle_t m5_display_spi;

uint8_t axp192_init_list[28] = {
    'A', 'X', 'P',
    0x06, 0, 11,
    0x10, 0xff, //1
    0x28, 0xCC, //2
    0x82, 0xff, //3
    0x33, 0xC1, //4
    0xB8, 0x80, //5
    0x12, 0x4D, //6
    0x36, 0x0C, //7
    0x90, 0x02, //8
    0x30, 0xec, //9
    0x39, 0xFC, //10
    0x35, 0xA2, //11
};

esp_err_t EventInit()
{
    esp_event_loop_args_t loop_args = {
        .queue_size = 5,
        .task_name = "event_loop",
        .task_priority = 10,
        .task_stack_size = 2048,
        .task_core_id = 0};

    esp_err_t e = esp_event_loop_create(&loop_args, &event_loop);
    if (e == ESP_OK)
    {
        ESP_LOGD(TAG, "Event loop created");
        return ESP_OK;
    }
    else
    {
        ESP_LOGE(TAG, "Error creating event loop: %s", esp_err_to_name(e));
        return ESP_FAIL;
    }
}


esp_err_t M5DisplayInit()
{
    esp_err_t e;

    tft_disp_type = M5DISPLAY_TYPE;
    _width = 80;   // smaller dimension
    _height = 160; // larger dimension
    max_rdclock = 25000000;

    TFT_PinsInit();

    spi_lobo_bus_config_t buscfg = {};
    buscfg.miso_io_num = PIN_NUM_MISO; // set SPI MISO pin
    buscfg.mosi_io_num = PIN_NUM_MOSI; // set SPI MOSI pin
    buscfg.sclk_io_num = PIN_NUM_CLK;  // set SPI CLK pin
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;
    buscfg.max_transfer_sz = 6 * 1024;

    spi_lobo_device_interface_config_t devcfg = {};
    devcfg.clock_speed_hz = 25000000;		 // Initial clock out at 8 MHz
    devcfg.mode = 0;						 // SPI mode 0
    devcfg.spics_io_num = -1;				 // we will use external CS pin
    devcfg.spics_ext_io_num = PIN_NUM_CS;	// external CS pin
    devcfg.flags = LB_SPI_DEVICE_HALFDUPLEX; // ALWAYS SET  to HALF DUPLEX MODE!! for display spi

    e = spi_lobo_bus_add_device(TFT_HSPI_HOST, &buscfg, &devcfg, &m5_display_spi);
    if (e != ESP_OK)
    {
        ESP_LOGE(TAG, "Error adding display to SPI bus: %s", esp_err_to_name(e));
        return ESP_FAIL;
    }

    disp_spi = m5_display_spi;

    // ==== Test select/deselect ====
    spi_lobo_device_select(m5_display_spi, 1);
    spi_lobo_device_deselect(m5_display_spi);

    // Pin reset
    gpio_set_level((gpio_num_t)PIN_NUM_RST, 1);
    vTaskDelay(5 / portTICK_PERIOD_MS);
    gpio_set_level((gpio_num_t)PIN_NUM_RST, 0);
    vTaskDelay(20 / portTICK_PERIOD_MS);
    gpio_set_level((gpio_num_t)PIN_NUM_RST, 1);

    TFT_display_init();
    TFT_invertDisplay(INVERT_ON);

    // ==== Set SPI clock used for display operations ====
    spi_lobo_set_speed(m5_display_spi, DEFAULT_SPI_CLOCK);

    font_rotate = 0;
    text_wrap = 0;
    font_transparent = 0;
    font_forceFixed = 0;
    gray_scale = 0;
    TFT_setGammaCurve(DEFAULT_GAMMA_CURVE);
    TFT_setRotation(LANDSCAPE);
    TFT_setFont(DEFAULT_FONT, NULL);
    TFT_resetclipwin();

    return ESP_OK;
}

esp_err_t M5Init()
{
    esp_err_t e;
    uint8_t error_count = 0;

    EventInit();

    ESP_LOGI(TAG, "Setting up I2C");
    e = InitI2CWire(&wire0);
    if (e == ESP_OK)
    {
        AxpInitFromList(&wire0, axp192_init_list);
    }
    else
    {
        ESP_LOGE(TAG, "Error setting up I2C: %s", esp_err_to_name(e));
        ++error_count;
    }
    // Init button
    e = ButtonInit();
    if (e == ESP_OK)
    {
        ESP_LOGD(TAG, "Button initialized");
    }
    else
    {
        ESP_LOGE(TAG, "Error initializing button");
        ++error_count;
    }

    MPU6886Init();

    // Init display
    e = M5DisplayInit();
    if (e == ESP_OK)
    {
        ESP_LOGD(TAG, "Display initialized");
    }
    else
    {
        ESP_LOGE(TAG, "Error initializing display");
        ++error_count;
    }

    if (error_count == 0)
    {
        ESP_LOGD(TAG, "M5StickC initialized successfully");
        return ESP_OK;
    }
    else
    {
        ESP_LOGE(TAG, "%d errors found while initializing M5StickC", error_count);
        return ESP_FAIL;
    }
}