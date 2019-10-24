/* M5StackC - Example

   See README.md file to get detailed usage of this example.
*/
#include <stdio.h>
#include "esp_system.h"
#include "esp_err.h"
#include "esp_log.h"
#include "m5stickc.h"
#include "sdkconfig.h"
#include "wire.h"
#include "AXP192.h"
#include "driver/i2s.h"

static uint8_t strbuff[50];
static bool button_a_flag = false;

static void AXP192Test(void)
{
    float temp;
    float bat_voltage;
    float bat_current;
    float vin_voltage;
    float vin_current;

    while (button_a_flag == false)
    {
        temp = AXP192GetTempInAXP192(&wire0);
        sprintf((char *)strbuff, "AXP192-temp:%.2fC", temp);
        TFT_print((char *)strbuff, 5, 10);
        bat_voltage = AXP192GetBatVoltage(&wire0);
        sprintf((char *)strbuff, "bat_voltage:%.2fV", bat_voltage);
        TFT_print((char *)strbuff, 5, 20);
        bat_current = AXP192GetBatCurrent(&wire0);
        sprintf((char *)strbuff, "bat_current:%.2fmA", bat_current);
        TFT_print((char *)strbuff, 5, 30);
        vin_voltage = AXP192GetVinVoltage(&wire0);
        sprintf((char *)strbuff, "vin_voltage:%.2fV", vin_voltage);
        TFT_print((char *)strbuff, 5, 40);
        vin_current = AXP192GetVinCurrent(&wire0);
        sprintf((char *)strbuff, "vin_current:%.2fmA", vin_current);
        TFT_print((char *)strbuff, 5, 50);

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
    button_a_flag = false;
    vTaskDelay(200 / portTICK_PERIOD_MS);
    TFT_fillScreen(TFT_BLACK);
    TFT_print(">>>M5 StickC<<<", CENTER, 0);
}

static void MPU6886Test(void)
{
    float accX = 0;
    float accY = 0;
    float accZ = 0;
    float gyroX = 0;
    float gyroY = 0;
    float gyroZ = 0;
    float temp = 0;

    while (button_a_flag == false)
    {
        MPU6886getGyroData(&gyroX, &gyroY, &gyroZ);
        MPU6886getAccelData(&accX, &accY, &accZ);
        MPU6886getTempData(&temp);

        printf("%.2f   %.2f   %.2f      ", gyroX, gyroY, gyroZ);
        printf("o/s\n");
        printf("%.2f   %.2f   %.2f      ", accX * 1000, accY * 1000, accZ * 1000);
        printf("mg\n");
        printf("temperature : %.2f C\n", temp);

        TFT_print(" |", 5, 20);
        TFT_print("Gyro", 30, 10);
        TFT_print("| Accel", 80, 10);
        TFT_print("X|", 5, 20);
        TFT_print("Y|", 5, 30);
        TFT_print("Z|", 5, 40);

        sprintf((char *)strbuff, "%.2f", gyroX);
        TFT_print((char *)strbuff, 30, 20);
        sprintf((char *)strbuff, "%.2f", gyroY);
        TFT_print((char *)strbuff, 30, 30);
        sprintf((char *)strbuff, "%.2f", gyroZ);
        TFT_print((char *)strbuff, 30, 40);

        sprintf((char *)strbuff, "| %.2f", accX * 1000);
        TFT_print((char *)strbuff, 80, 20);
        sprintf((char *)strbuff, "| %.2f", accY * 1000);
        TFT_print((char *)strbuff, 80, 30);
        sprintf((char *)strbuff, "| %.2f", accZ * 1000);
        TFT_print((char *)strbuff, 80, 40);

        sprintf((char *)strbuff, "temperature:%.2fC\n", temp);
        TFT_print((char *)strbuff, 5, 50);

        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
    
    vTaskDelay(200 / portTICK_PERIOD_MS);
    button_a_flag = false;
    TFT_fillScreen(TFT_BLACK);
    TFT_print(">>>M5 StickC<<<", CENTER, 0);
}

#define PIN_CLK 0
#define PIN_DATA 34
#define READ_LEN (2 * 200)

uint8_t i2s_read_buff[READ_LEN] = {0};
int16_t adc_data_buff[200] = {};
int16_t *padc_16bit_buffer = NULL;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void MicRecordTest()
{
    uint16_t posX = 0, yData, lastY = 0;
    uint16_t count_n = 0, count_i = 0;
    int32_t sumData;
    size_t bytesread;

    TFT_fillScreen(TFT_BLACK);

    TFT_drawLine(0, 73, 110, 73, TFT_WHITE);
    TFT_drawLine(5, 5, 5, 75, TFT_WHITE);
    for (count_n = 0; count_n < 10; count_n++)
    {
        TFT_drawLine(15 + count_n * 10, 72, 15 + count_n * 10, 75, TFT_WHITE);

    }

    for (count_n = 0; count_n < 200; count_n++)
    {
        adc_data_buff[count_n] = 0;
    }

    while (button_a_flag == false)
    {
        i2s_read(I2S_NUM_0, (char *)i2s_read_buff, READ_LEN, &bytesread, (100 / portTICK_RATE_MS));
        padc_16bit_buffer = (int16_t *)i2s_read_buff;

        posX += 10;
        if (posX >= 200)
        {
            posX = 0;
        }

        for (count_n = 0; count_n < 10; count_n++)
        {
            sumData = 0;
            for (count_i = 0; count_i < 20; count_i++)
            {
                sumData = sumData + padc_16bit_buffer[count_i + count_n * 20] * 5;
            }

            if (posX + 100 + count_n >= 200)
            {
                adc_data_buff[posX + 100 + count_n - 200] = (sumData / 20);
            }
            else
            {
                adc_data_buff[posX + 100 + count_n] = (sumData / 20);
            }
        }

        TFT_fillRect(6, 0, 106, 70, TFT_BLACK);
        for (count_n = 0; count_n < 100; count_n++)
        {
            if ((posX + count_n) < 200)
            {
                yData = map(adc_data_buff[posX + count_n], INT16_MIN, INT16_MAX, 10, 70);
            }
            else
            {
                yData = map(adc_data_buff[posX + count_n - 200], INT16_MIN, INT16_MAX, 10, 70);
            }
            if (lastY == yData)
            {
                TFT_drawPixel(105 - count_n, yData, TFT_WHITE, 1);
            }
            else
            {
                TFT_drawLine(105 - count_n, lastY, 105 - count_n + 1, yData, TFT_WHITE);
            }

            lastY = yData;
        }
        vTaskDelay(50 / portTICK_RATE_MS);
    }

    vTaskDelay(200 / portTICK_PERIOD_MS);
    button_a_flag = false;
    TFT_fillScreen(TFT_BLACK);
    TFT_print(">>>M5 StickC<<<", CENTER, 0);
}

static void stickc_test_task(void *arg)
{
    while (1)
    {
        MPU6886Test();
        AXP192Test();
        MicRecordTest();
    }
}

void buttonEvent(void *handler_arg, esp_event_base_t base, int32_t id, void *event_data)
{
    if ((base == button_a.esp_event_base) && (id == BUTTON_PRESSED_EVENT))
    {
        TFT_print("button A", CENTER, 60);
        button_a_flag = true;
    }

    if ((base == button_b.esp_event_base) && (id == BUTTON_PRESSED_EVENT))
    {
        TFT_print("button B", CENTER, 60);
    }
}

void i2sInit()
{
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, // is fixed at 12bit, stereo, MSB
        .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 2,
        .dma_buf_len = 128,
    };

    i2s_pin_config_t pin_config;
    pin_config.bck_io_num = I2S_PIN_NO_CHANGE;
    pin_config.ws_io_num = PIN_CLK;
    pin_config.data_out_num = I2S_PIN_NO_CHANGE;
    pin_config.data_in_num = PIN_DATA;

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
    i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
}

void app_main(void)
{
    M5Init();
    i2sInit();

    esp_event_handler_register_with(event_loop, BUTTON_A_EVENT_BASE, BUTTON_PRESSED_EVENT, buttonEvent, NULL);
    esp_event_handler_register_with(event_loop, BUTTON_B_EVENT_BASE, BUTTON_PRESSED_EVENT, buttonEvent, NULL);

    font_rotate = 0;
    text_wrap = 0;
    font_transparent = 0;
    font_forceFixed = 0;
    gray_scale = 0;
    TFT_setGammaCurve(DEFAULT_GAMMA_CURVE);
    TFT_setRotation(LANDSCAPE);
    TFT_setFont(SMALL_FONT, NULL);
    TFT_resetclipwin();

    TFT_print(">>>M5 StickC<<<", CENTER, 0);
    vTaskDelay(500 / portTICK_PERIOD_MS);

    xTaskCreate(stickc_test_task, "stickc_test_task", 1024 * 2, (void *)0, 10, NULL);
}
