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

static void stickc_test_task(void *arg)
{
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
	TFT_print(">>>M5 StickC<<<", CENTER, 0);
	vTaskDelay(500 /portTICK_PERIOD_MS);

	xTaskCreate(stickc_test_task, "stickc_test_task", 1024 * 2, (void *)0, 10, NULL);
}
