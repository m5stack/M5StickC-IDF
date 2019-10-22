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

static uint8_t strbuff[50];
static bool ButtonAFlag = false;

static void LCDtest(void)
{
    TFT_fillScreen(TFT_BLACK);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    TFT_fillScreen(TFT_RED);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    TFT_fillScreen(TFT_BLUE);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    TFT_fillScreen(TFT_GREEN);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    TFT_fillScreen(TFT_CYAN);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    TFT_fillScreen(TFT_YELLOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    TFT_fillScreen(TFT_WHITE);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    TFT_fillScreen(TFT_BLACK);
    vTaskDelay(500 / portTICK_PERIOD_MS);
}

static void AXP192Test(void)
{
    float Temp;
    float BatVoltage;
    float BatCurrent;
    float VinVoltage;
    float VinCurrent;
    float VbusVoltage;

    while( ButtonAFlag == false )
    {
        Temp = AXP192GetTempInAXP192( &wire0 );
        sprintf( ( char* )strbuff, "AXP192-Temp:%.2fC", Temp);
        TFT_print( ( char* )strbuff, 5, 10);
        BatVoltage = AXP192GetBatVoltage( &wire0 );
        sprintf( ( char* )strbuff, "BatVoltage:%.2fV", BatVoltage);
        TFT_print( ( char* )strbuff, 5, 20);
        BatCurrent = AXP192GetBatCurrent( &wire0 );
        sprintf( ( char* )strbuff, "BatCurrent:%.2fmA", BatCurrent);
        TFT_print(( char* )strbuff, 5, 30);
        VinVoltage = AXP192GetVinVoltage( &wire0 );
        sprintf( ( char* )strbuff, "VinVoltage:%.2fV", VinVoltage);
        TFT_print(( char* )strbuff, 5, 40);
        VinCurrent = AXP192GetVinCurrent( &wire0 );
        sprintf( ( char* )strbuff, "VinCurrent:%.2fmA", VinCurrent);
        TFT_print(( char* )strbuff, 5, 50);

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
    ButtonAFlag  = false;
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

    while ( ButtonAFlag == false )
    {
        MPU6886getGyroData(&gyroX, &gyroY, &gyroZ);
        MPU6886getAccelData(&accX, &accY, &accZ);
        MPU6886getTempData(&temp);

        printf("%.2f   %.2f   %.2f      ", gyroX, gyroY, gyroZ);
        printf("o/s\n");
        printf("%.2f   %.2f   %.2f      ", accX * 1000, accY * 1000, accZ * 1000);
        printf("mg\n");
        printf("Temperature : %.2f C\n", temp);

        TFT_print(" |", 5, 20);
        TFT_print("Gyro", 30, 10);
        TFT_print("| Accel", 80, 10);
        TFT_print("X|", 5, 20);
        TFT_print("Y|", 5, 30);
        TFT_print("Z|", 5, 40);

        sprintf( ( char* )strbuff, "%.2f", gyroX);
        TFT_print(( char* )strbuff, 30, 20);
        sprintf( ( char* )strbuff, "%.2f", gyroY);
        TFT_print(( char* )strbuff, 30, 30);
        sprintf( ( char* )strbuff, "%.2f", gyroZ);
        TFT_print(( char* )strbuff, 30, 40);

        sprintf( ( char* )strbuff, "| %.2f", accX * 1000);
        TFT_print(( char* )strbuff, 80, 20);
        sprintf( ( char* )strbuff, "| %.2f", accY * 1000);
        TFT_print(( char* )strbuff, 80, 30);
        sprintf( ( char* )strbuff, "| %.2f", accZ * 1000);
        TFT_print(( char* )strbuff, 80, 40);

        sprintf( ( char* )strbuff, "Temperature:%.2fC\n", temp);
        TFT_print(( char* )strbuff, 5, 50);

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
    vTaskDelay(200 / portTICK_PERIOD_MS);
    ButtonAFlag  = false;
    TFT_fillScreen(TFT_BLACK);
    TFT_print(">>>M5 StickC<<<", CENTER, 0);
}

static void stickc_test_task(void *arg)
{

    while (1)
    {
        MPU6886Test();
        AXP192Test();
    }
}

void buttonEvent(void *handler_arg, esp_event_base_t base, int32_t id, void *event_data)
{
    if ((base == m5button_a.esp_event_base) && (id == M5BUTTON_BUTTON_CLICK_EVENT))
    {
        TFT_print("button A", CENTER, 60);
        ButtonAFlag = true;
    }

    if ((base == m5button_b.esp_event_base) && (id == M5BUTTON_BUTTON_CLICK_EVENT))
    {
        TFT_print("button B", CENTER, 60);
    }
}

void app_main(void)
{
    m5stickc_config_t m5config = M5STICKC_CONFIG_DEFAULT();
    m5config.power.lcd_backlight_level = 3; // Set starting backlight level

    m5_init(&m5config);

    esp_event_handler_register_with(m5_event_loop, M5BUTTON_A_EVENT_BASE, M5BUTTON_BUTTON_CLICK_EVENT, buttonEvent, NULL);
    esp_event_handler_register_with(m5_event_loop, M5BUTTON_B_EVENT_BASE, M5BUTTON_BUTTON_CLICK_EVENT, buttonEvent, NULL);

    font_rotate = 0;
    text_wrap = 0;
    font_transparent = 0;
    font_forceFixed = 0;
    gray_scale = 0;
    TFT_setGammaCurve(DEFAULT_GAMMA_CURVE);
    TFT_setRotation(LANDSCAPE);
    TFT_setFont(SMALL_FONT, NULL);
    TFT_resetclipwin();

    LCDtest();

    TFT_print(">>>M5 StickC<<<", CENTER, 0);
    vTaskDelay(500 / portTICK_PERIOD_MS);

    xTaskCreate(stickc_test_task, "stickc_test_task", 1024 * 2, (void *)0, 10, NULL);
}
