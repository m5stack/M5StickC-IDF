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

#define PIN_CLK  0
#define PIN_DATA 34
#define READ_LEN (2 * 200)

uint8_t BUFFER[READ_LEN] = {0};
int16_t oldy[200] = {};
int16_t *adcBuffer = NULL;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void MicRecordTest()
{   
	uint16_t posX = 0, yData, LastY = 0,LastYs = 0, posX_b = 0;
	uint16_t Count_n = 0, Count_i = 0, Bigest = 0;
	int32_t SumData;
	size_t bytesread;

	TFT_fillScreen(TFT_BLACK);

	TFT_drawLine( 0, 73, 110, 73, TFT_WHITE);
	TFT_drawLine( 5, 5, 5, 75, TFT_WHITE);
	for( Count_n = 0; Count_n < 10; Count_n++ )
		TFT_drawLine( 15 + Count_n * 10 , 72, 15 + Count_n * 10, 75, TFT_WHITE);

	for( Count_n = 0; Count_n < 200; Count_n++ )
		oldy[Count_n] = 0;

	while ( ButtonAFlag == false )
    {
		i2s_read(I2S_NUM_0, (char*) BUFFER, READ_LEN, &bytesread, (100 / portTICK_RATE_MS));
		adcBuffer = (int16_t *)BUFFER;

		posX += 10;
		if( posX >= 200 )
		posX = 0;

		for( Count_n = 0; Count_n < 10; Count_n++ )
		{
			SumData = 0;
			for( Count_i = 0; Count_i < 20; Count_i++ )
			{
				SumData = SumData + adcBuffer[ Count_i + Count_n * 20 ] * 3 ;
			}

			if( posX + 100 + Count_n >= 200 )
			{
				oldy[ posX + 100 + Count_n - 200 ] = (SumData / 20) ;
			}
			else
			{
				oldy[ posX + 100 + Count_n ] = (SumData / 20);
			}
		}

		TFT_fillRect(6, 0, 106, 70, TFT_BLACK);
		for( Count_n = 0; Count_n < 100; Count_n++ )
		{
			if(( posX + Count_n )< 200 )
			{
				yData = map(oldy[ posX + Count_n ],INT16_MIN ,INT16_MAX ,10,70);
			}
			else
			{
				yData = map(oldy[ posX + Count_n -200 ],INT16_MIN ,INT16_MAX ,10,70);
			}
			if( LastY == yData )
				TFT_drawPixel( 105- Count_n , yData, TFT_WHITE, 1);
			else 
				TFT_drawLine( 105- Count_n , LastY, 105- Count_n + 1, yData, TFT_WHITE);
			LastY = yData;
		}
		vTaskDelay(50 / portTICK_RATE_MS);
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
		MicRecordTest();
    }
}

void buttonEvent(void *handler_arg, esp_event_base_t base, int32_t id, void *event_data)
{
    if ((base == button_a.esp_event_base) && (id == BUTTON_PRESSED_EVENT))
    {
        TFT_print("button A", CENTER, 60);
        ButtonAFlag = true;
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
    .sample_rate =  44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, // is fixed at 12bit, stereo, MSB
    .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 2,
    .dma_buf_len = 128,
   };

   i2s_pin_config_t pin_config;
   pin_config.bck_io_num   = I2S_PIN_NO_CHANGE;
   pin_config.ws_io_num    = PIN_CLK;
   pin_config.data_out_num = I2S_PIN_NO_CHANGE;
   pin_config.data_in_num  = PIN_DATA;
	
   i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
   i2s_set_pin(I2S_NUM_0, &pin_config);
   i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
}

void app_main(void)
{
    m5_init();
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
