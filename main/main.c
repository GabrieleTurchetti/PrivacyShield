#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "global_config.h"
#include "mesh_core.h"
#include "esp_mac.h"
#include "audio_hal.h"

static const char *TAG = "MAIN_APP";

QueueHandle_t audio_ai_queue;

/* -------------------------------------------------------------------------- */
/*  Hello task — broadcast our presence every 5 seconds                       */
/* -------------------------------------------------------------------------- */

static void hello_task(void *arg) {
    TickType_t last_wake = xTaskGetTickCount();
    while (1) {
        mesh_send_hello();
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(10000));
    }
}

/* -------------------------------------------------------------------------- */
/*  Prune task — clean up timed-out neighbors every 2 seconds                 */
/* -------------------------------------------------------------------------- */

static void prune_task(void *arg) {
    while (1) {
        mesh_discovery_prune();
        int count = mesh_discovery_count();
        if (count > 0) {
            ESP_LOGI(TAG, "%d neighbor(s) online", count);
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

/* -------------------------------------------------------------------------- */
/*  Packet received callback — handle incoming mesh packets                   */
/* -------------------------------------------------------------------------- */

static void on_mesh_packet(const uint8_t *src_mac, const void *data, size_t len) {
    const mesh_header_t *hdr = (const mesh_header_t *)data;

    switch (hdr->type) {
    case MESH_PKT_HELLO:
        ESP_LOGI(TAG, "HELLO from node %u (" MACSTR ")",
                 hdr->src_id, MAC2STR(src_mac));
        break;

    case MESH_PKT_STATUS:
        if (len >= sizeof(mesh_status_pkt_t)) {
            const mesh_status_pkt_t *status = (const mesh_status_pkt_t *)data;
            ESP_LOGI(TAG, "STATUS from node %u: masking=%s vol=%u batt=%u%%",
                     status->header.src_id,
                     status->masking_active ? "ON" : "OFF",
                     status->volume, status->battery_pct);
        }
        break;

    case MESH_PKT_COMMAND:
        if (len >= sizeof(mesh_command_pkt_t)) {
            const mesh_command_pkt_t *cmd = (const mesh_command_pkt_t *)data;
            ESP_LOGI(TAG, "COMMAND from node %u: cmd=%u val=%u",
                     cmd->header.src_id, cmd->command, cmd->value);
            /* Future: act on mute/unmute/volume commands here */
        }
        break;

    default:
        ESP_LOGD(TAG, "Unknown packet type 0x%02X from node %u",
                 hdr->type, hdr->src_id);
        break;
    }
}

/* -------------------------------------------------------------------------- */
/*  Entry point                                                               */
/* -------------------------------------------------------------------------- */

void app_main(void) {
    ESP_LOGI(TAG, "======================================");
    ESP_LOGI(TAG, "  Privacy Shield — Node %u", DEFAULT_NODE_ID);
    ESP_LOGI(TAG, "======================================");

    /* ---- Mesh (ESP-NOW) ---- */
    ESP_ERROR_CHECK(mesh_init(DEFAULT_NODE_ID));
    mesh_register_recv_callback(on_mesh_packet);
    xTaskCreate(hello_task, "hello", 2048, NULL, 1, NULL);
    xTaskCreate(prune_task, "prune", 2048, NULL, 1, NULL);

    ESP_LOGI(TAG, "Booting Privacy Shield System...");

    audio_ai_queue = xQueueCreate(10, 512 * sizeof(int16_t));

    if (audio_ai_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create audio queue!");
        return;
    }

    /* ---- Audio (I2S Microphone) ---- */
    audio_hal_mic_init();

    // Run the microphone reading logic on Core 1
    xTaskCreatePinnedToCore(audio_hal_mic_read_task, "Mic_Task", 8192, NULL, 5, NULL, 1);

    ESP_LOGI(TAG, "System ready.");
}