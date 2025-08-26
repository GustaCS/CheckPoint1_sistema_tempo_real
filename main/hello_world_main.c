#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "freertos/semphr.h"

SemaphoreHandle_t semTask1;
SemaphoreHandle_t semTask2;
SemaphoreHandle_t semTask3;

void vTask1(void *pvParameters)
{
    while (1) {
        xSemaphoreTake(semTask1, portMAX_DELAY); // Espera o semáforo
        printf("Tarefa 1 executou : Gustavo Correa\n");
        vTaskDelay(pdMS_TO_TICKS(1000));        // Delay 1s
        xSemaphoreGive(semTask2);               // Libera semáforo da próxima task
    }
}

void vTask2(void *pvParameters)
{
    while (1) {
        xSemaphoreTake(semTask2, portMAX_DELAY);
        printf("Tarefa 2 executou : Gustavo Correa\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
        xSemaphoreGive(semTask3);
    }
}

void vTask3(void *pvParameters)
{
    while (1) {
        xSemaphoreTake(semTask3, portMAX_DELAY);
        printf("Tarefa 3 executou : Gustavo Correa\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
        xSemaphoreGive(semTask1); // Volta para a Task1
    }
}

void app_main(void)
{
    //3 tasks que passam o meu nome com semaforo
    
    // Criando semáforos binários
    semTask1 = xSemaphoreCreateBinary();
    semTask2 = xSemaphoreCreateBinary();
    semTask3 = xSemaphoreCreateBinary();

    // Inicialmente só libera a Task1 para começar o ciclo
    xSemaphoreGive(semTask1);

    // Criação das tasks
    xTaskCreate(vTask1, "Task1", 2048, NULL, 5, NULL);
    xTaskCreate(vTask2, "Task2", 2048, NULL, 5, NULL);
    xTaskCreate(vTask3, "Task3", 2048, NULL, 5, NULL);
}
