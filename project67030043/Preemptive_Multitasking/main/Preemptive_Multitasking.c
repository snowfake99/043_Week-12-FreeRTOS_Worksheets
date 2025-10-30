#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

// Preemptive multitasking using FreeRTOS
static const char *PREEMPT_TAG = "PREEMPTIVE";
static volatile bool preempt_emergency = false;
static uint64_t preempt_start_time = 0;
static uint32_t preempt_max_response = 0;

void preemptive_task1(void *pvParameters)
{
    uint32_t count = 0;
    while (1) {
        ESP_LOGI(PREEMPT_TAG, "Preempt Task1: %d", count++);
        
        gpio_set_level(LED1_PIN, 1);
        
        // Simulate work WITHOUT yielding
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 50000; j++) {
                volatile int dummy = j * 2;
            }
            // No voluntary yielding - RTOS will preempt if needed
        }
        
        gpio_set_level(LED1_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void preemptive_task2(void *pvParameters)
{
    uint32_t count = 0;
    while (1) {
        ESP_LOGI(PREEMPT_TAG, "Preempt Task2: %d", count++);
        
        gpio_set_level(LED2_PIN, 1);
        
        // Simulate LONGER work without yielding
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 30000; j++) {
                volatile int dummy = j + i;
            }
            // RTOS can preempt this task anytime
        }
        
        gpio_set_level(LED2_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(150));
    }
}

void preemptive_emergency_task(void *pvParameters)
{
    while (1) {
        // Check button every 5ms
        if (gpio_get_level(BUTTON_PIN) == 0 && !preempt_emergency) {
            preempt_emergency = true;
            preempt_start_time = esp_timer_get_time();
            
            // This high-priority task will IMMEDIATELY preempt others
            uint64_t response_time = esp_timer_get_time() - preempt_start_time;
            uint32_t response_ms = (uint32_t)(response_time / 1000);
            
            if (response_ms > preempt_max_response) {
                preempt_max_response = response_ms;
            }
            
            ESP_LOGW(PREEMPT_TAG, "IMMEDIATE EMERGENCY! Response: %d ms (Max: %d ms)", 
                     response_ms, preempt_max_response);
            
            gpio_set_level(LED3_PIN, 1);
            vTaskDelay(pdMS_TO_TICKS(200));
            gpio_set_level(LED3_PIN, 0);
            
            preempt_emergency = false;
        }
        
        vTaskDelay(pdMS_TO_TICKS(5)); // Check every 5ms
    }
}

void test_preemptive_multitasking(void)
{
    ESP_LOGI(PREEMPT_TAG, "=== Preemptive Multitasking Demo ===");
    ESP_LOGI(PREEMPT_TAG, "RTOS will preempt tasks automatically");
    ESP_LOGI(PREEMPT_TAG, "Press button to test emergency response");
    
    // Create tasks with different priorities
    xTaskCreate(preemptive_task1, "PreTask1", 2048, NULL, 2, NULL);      // Normal priority
    xTaskCreate(preemptive_task2, "PreTask2", 2048, NULL, 1, NULL);      // Low priority
    xTaskCreate(preemptive_emergency_task, "Emergency", 2048, NULL, 5, NULL); // High priority
    
    // Delete the main task
    vTaskDelete(NULL);
}

void app_main(void)
{
    // GPIO Configuration
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << LED1_PIN) | (1ULL << LED2_PIN) | (1ULL << LED3_PIN),
        .pull_down_en = 0,
        .pull_up_en = 0,
    };
    gpio_config(&io_conf);

    // Button configuration
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = 1ULL << BUTTON_PIN;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    ESP_LOGI("MAIN", "Multitasking Comparison Demo");
    ESP_LOGI("MAIN", "Choose test mode:");
    ESP_LOGI("MAIN", "1. Cooperative (comment out preemptive call)");
    ESP_LOGI("MAIN", "2. Preemptive (comment out cooperative call)");
    
    // Uncomment ONE of the following:
    // test_cooperative_multitasking();  // Test cooperative
    test_preemptive_multitasking();       // Test preemptive
}