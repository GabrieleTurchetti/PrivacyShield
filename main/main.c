#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "audio_hal.h"

static const char *TAG = "MAIN_APP";

QueueHandle_t audio_ai_queue;

void app_main(void) {
    ESP_LOGI(TAG, "Booting Privacy Shield System...");

    audio_ai_queue = xQueueCreate(10, 512 * sizeof(int16_t));

    if (audio_ai_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create audio queue!");
        return;
    }

    // Initialize Hardware Components
    audio_hal_mic_init();

    // Run the microphone reading logic on Core 1
    xTaskCreatePinnedToCore(audio_hal_mic_read_task, "Mic_Task", 8192, NULL, 5, NULL, 1);

    ESP_LOGI(TAG, "System running.");
}