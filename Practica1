#include <stdio.h>
#include <string.h>
#include <driver/gpio.h>
#include "esp_adc/adc_oneshot.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "esp_system.h"

#ifndef configUSE_IDLE_HOOK
#define configUSE_IDLE_HOOK 1
#endif

//VARIABLES GLOBALES
volatile bool g_ledRapido = true;
volatile bool g_botonPres = false;
volatile bool g_sensorActivo = false;
TaskHandle_t xHandleLedRapido = NULL;
TaskHandle_t xHandleLedLento = NULL;
TaskHandle_t xHandleSensor = NULL;
TaskHandle_t xHandleMonitor = NULL;
volatile int contador = 0;

#define ADC_CHANNEL ADC_CHANNEL_0
adc_oneshot_unit_handle_t adc1_handle;

void TaskLedRapido(void *pvParameters) {
    gpio_reset_pin(32);
    gpio_set_direction(32, GPIO_MODE_OUTPUT); 

    while(1){
        contador = 0;
        if(g_botonPres == true){
            vTaskResume(xHandleLedLento);
            vTaskResume(xHandleSensor);
            vTaskSuspend(NULL);
        
        }
    gpio_set_level(32, 1);
    printf("[LED_R]  ");
    vTaskDelay(100 / portTICK_PERIOD_MS); 
    gpio_set_level(32, 0);
    vTaskDelay(100 / portTICK_PERIOD_MS); 
    }
}

void TaskLedLento(void *pvParameters) {
    vTaskSuspend(NULL); 
    while(1){
        if(contador >= 5 || g_ledRapido == true){
            if(contador >= 5){
                printf("\n[LED_L] Timeout 5s -> regresando a modo RAPIDO\n");
            }
            g_ledRapido = true;
            g_botonPres = false;
            contador = 0;
            vTaskResume(xHandleLedRapido);  
            vTaskSuspend(NULL);    
        }
            gpio_set_level(32, 1);
            printf("[LED_L]  ");
            vTaskDelay(500 / portTICK_PERIOD_MS); 
            gpio_set_level(32, 0);
            vTaskDelay(500 / portTICK_PERIOD_MS);
            contador++;
    }
}

void TaskMonitor(void *pvParameters) {
    gpio_reset_pin(0);
    gpio_set_direction(0, GPIO_MODE_INPUT);
    UBaseType_t uxTaskGetStackHighWaterMark( TaskHandle_t xTask );
    while(1)
    {
        if(gpio_get_level(0) == 0){
            if (g_ledRapido == true){
                printf("(usuario presiona boton) ");
                printf("BOTON PRESIONADO ");
                printf("Boton detectado -> activando modo LENTO\n");
                g_botonPres = true;
                g_ledRapido = false;
                g_sensorActivo = true;
        } else {
            printf("\n(usuario presiona boton) BOTON PRESIONADO \n");
                printf("Boton detectado -> interrumpiendo y regresando a modo RAPIDO\n");
                g_ledRapido = true;
                g_botonPres = false;
                g_sensorActivo = false;
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }

        uint32_t free_heap = esp_get_free_heap_size();
        int StackRapido = uxTaskGetStackHighWaterMark(xHandleLedRapido);
        int StackLento = uxTaskGetStackHighWaterMark(xHandleLedLento);
        int StackSensor = uxTaskGetStackHighWaterMark(xHandleSensor);

        printf("[MON] Heap: %lu  Stack R: %u  Stack L: %u  Stack S: %u words\n", 
               (unsigned long)free_heap, (unsigned int)StackRapido, (unsigned int)StackLento, (unsigned int)StackSensor);
               vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void TaskSensor(void *pvParameters) {
    vTaskSuspend(NULL); 

    // Implementación del ADC
    int adc_raw;
    float voltaje;
    adc_oneshot_unit_init_cfg_t adc1_init_cfg = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };

    adc_oneshot_new_unit(&adc1_init_cfg, &adc1_handle);

    adc_oneshot_chan_cfg_t adc1_cfg = {
        .bitwidth = ADC_BITWIDTH_DEFAULT, 
        .atten = ADC_ATTEN_DB_12,
    };
    adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL, &adc1_cfg);
 
    while(1)
    {   
        if(contador >= 5 || g_sensorActivo == false){
            g_sensorActivo = false;   
            vTaskSuspend(NULL);    
        }
        voltaje = ((float)adc_raw /4095)*3.3;
        adc_oneshot_read(adc1_handle, ADC_CHANNEL, &adc_raw);
        
        printf("[SENS] ADC raw: %d  v: %.2f\n", adc_raw, voltaje);
        vTaskDelay(300 / portTICK_PERIOD_MS);
    }
}

void vApplicationIdleHook(void) {
    static bool mensajeImpreso = false;
    
    if (g_ledRapido == false && contador >= 4) {
        if (!mensajeImpreso) {
            printf("\n[IDLE] CPU libre esperando evento de boton\n");
            mensajeImpreso = true;
        }
    } else {
        mensajeImpreso = false;
    }
}
void app_main(void)
{
    xTaskCreate(TaskLedRapido, "LED_R", 2048, NULL, 1, &xHandleLedRapido);
    xTaskCreate(TaskLedLento, "LED_L", 2048, NULL, 2, &xHandleLedLento);
    xTaskCreate(TaskSensor, "SENS", 2048, NULL, 3, &xHandleSensor);
    xTaskCreate(TaskMonitor, "MON", 3072, NULL, 4, &xHandleMonitor);

}
