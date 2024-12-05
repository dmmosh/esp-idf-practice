#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"

#include "driver/uart.h" // terminal drivers
#include "driver/gpio.h" // gpio pin drivers
#include "driver/spi_master.h"
#include "freertos/task.h" // freertos tasks
//#include "esp_chip_info.h"  // chip info
//#include "esp_flash.h" 
//#include "esp_system.h"

// PIO MENUCONFIG
// pio run -t menuconfig

// PIO RUN
// pio run --target uploaad

#define LED GPIO_NUM_2
#define ON 1
#define OFF 0
#define BACKLIGHT GPIO_NUM_32   






void led(void* args){
  int i = 1000; 
  while(1){
    gpio_set_level(LED, ON);
    vTaskDelay(i/portTICK_PERIOD_MS);
    gpio_set_level(LED, OFF);
    vTaskDelay(i/portTICK_PERIOD_MS);
    i= (int)((float)i /1.2);
    if(i == 0){
      i = 1000;
    }
    //Serial.printf("%i\n", i);
  }
}

void vTaskMemoryUsage(void *pvParameters)
{
    while(1){
        TaskHandle_t xTaskHandle = xTaskGetCurrentTaskHandle();
        UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(xTaskHandle);
        UBaseType_t uxTaskStackSize = configMINIMAL_STACK_SIZE; // Or your stack size

        printf("mem: %i | %i\n", uxHighWaterMark, uxTaskStackSize);
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}


extern "C" void app_main(void)
{
    uart_set_baudrate(UART_NUM_0, 115200); // bitrate
    printf("Hello world!\n");
    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT); // whether the gpio will function as input or output
    gpio_set_direction(BACKLIGHT, GPIO_MODE_OUTPUT);
    gpio_set_level(BACKLIGHT, 255);

    xTaskCreate(led, "led blink", 2048, NULL, 1, NULL);

    while(1){

        vTaskDelay(5/portTICK_PERIOD_MS);
    }
    //xTaskCreate(vTaskMemoryUsage, "memory check", 2048, NULL, 1, NULL);

    fflush(stdout);
    esp_restart();
}