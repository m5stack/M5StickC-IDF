/* i2c - Example

   For other examples please check:
   https://github.com/espressif/esp-idf/tree/master/examples

   See README.md file to get detailed usage of this example.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "esp_system.h"
#include "esp_err.h"
#include "esp_log.h"

#include "m5stickc.h"

#include "sdkconfig.h"
#include "wire.h"
#include "AXP192.h"

//static const char *TAG = "i2c-example";

#define DATA_LENGTH 512                  /*!< Data buffer length of test buffer */
#define RW_TEST_LENGTH 128               /*!< Data length for r/w test, [0,DATA_LENGTH] */
#define DELAY_TIME_BETWEEN_ITEMS_MS 1000 /*!< delay time between different test items */

static void disp_buf(uint8_t *buf, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        printf("%02x ", buf[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

int data = 0;

static void i2c_test_task(void *arg)
{
    uint32_t task_idx = (uint32_t) arg;
    uint8_t *data_rd = (uint8_t *)malloc(DATA_LENGTH);
	char *strbuff = (char *)malloc(64);
	uint16_t VbatData = 0;

	float accX = 0;
	float accY = 0;
	float accZ = 0;

	float gyroX = 0;
	float gyroY = 0;
	float gyroZ = 0;

	float temp = 0;
    while (1) {
		
		MPU6886getGyroData(&gyroX,&gyroY,&gyroZ);
		MPU6886getAccelData(&accX,&accY,&accZ);
		MPU6886getTempData(&temp);

		printf("%.2f   %.2f   %.2f      ", gyroX, gyroY,gyroZ);
		printf("o/s\n");
		printf("%.2f   %.2f   %.2f      ",accX * 1000,accY * 1000, accZ * 1000);
		printf("mg\n");
		printf("Temperature : %.2f C\n", temp);

		TFT_print(" |", 5, 20);
		TFT_print("Gyro", 30, 10);
		TFT_print("| Accel", 80, 10);
		TFT_print("X|", 5, 20);
		TFT_print("Y|", 5, 30);
		TFT_print("Z|", 5, 40);

		sprintf(strbuff, "%.2f", gyroX);
		TFT_print(strbuff, 30, 20);
		sprintf(strbuff, "%.2f", gyroY);
		TFT_print(strbuff, 30, 30);
		sprintf(strbuff, "%.2f", gyroZ);
		TFT_print(strbuff, 30, 40);

		sprintf(strbuff, "| %.2f", accX * 1000);
		TFT_print(strbuff, 80, 20);
		sprintf(strbuff, "| %.2f", accY * 1000);
		TFT_print(strbuff, 80, 30);
		sprintf(strbuff, "| %.2f", accZ * 1000);
		TFT_print(strbuff, 80, 40);

		sprintf(strbuff, "Temperature:%.2fC\n", temp);
		TFT_print(strbuff, 5, 50);

		vTaskDelay(300 /portTICK_PERIOD_MS);

		//TFT_print("Backlight test", CENTER, (M5DISPLAY_HEIGHT-24)/2 +12);
    }
}


void app_main(void)
{
	m5stickc_config_t m5config = M5STICKC_CONFIG_DEFAULT();
    m5config.power.lcd_backlight_level = 3; // Set starting backlight level

	m5_init(&m5config);

	font_rotate = 0;
    text_wrap = 0;
    font_transparent = 0;
    font_forceFixed = 0;
    gray_scale = 0;
    TFT_setGammaCurve(DEFAULT_GAMMA_CURVE);
    TFT_setRotation(LANDSCAPE);
    TFT_setFont(SMALL_FONT, NULL);
    TFT_resetclipwin();
    TFT_fillScreen(TFT_BLACK);
	TFT_print(">>>FUCK STC<<<", CENTER, 0);
	vTaskDelay(500 /portTICK_PERIOD_MS);

    xTaskCreate(i2c_test_task, "i2c_test_task_0", 1024 * 2, (void *)0, 10, NULL);
}
