#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "DYNAMIC_MEMORY";

void dynamic_allocation_example(void)
{
    ESP_LOGI(TAG, "Initial free heap: %d bytes", xPortGetFreeHeapSize());
    
    // Allocate memory for data buffer
    size_t buffer_size = 1024;
    uint8_t *data_buffer = (uint8_t*)pvPortMalloc(buffer_size);
    
    if (data_buffer != NULL) {
        ESP_LOGI(TAG, "Allocated %d bytes at %p", buffer_size, data_buffer);
        ESP_LOGI(TAG, "Free heap after allocation: %d bytes", xPortGetFreeHeapSize());
        
        // Use the buffer
        memset(data_buffer, 0xAA, buffer_size);
        ESP_LOGI(TAG, "Buffer initialized with pattern");
        
        // Free the memory
        vPortFree(data_buffer);
        ESP_LOGI(TAG, "Memory freed");
        ESP_LOGI(TAG, "Free heap after free: %d bytes", xPortGetFreeHeapSize());
        
        data_buffer = NULL; // Prevent accidental reuse
    } else {
        ESP_LOGE(TAG, "Failed to allocate %d bytes", buffer_size);
    }
    
    ESP_LOGI(TAG, "Minimum free heap size ever: %d bytes", 
             xPortGetMinimumEverFreeHeapSize());
}

// Dynamic task creation example
void create_dynamic_task(void)
{
    TaskHandle_t dynamic_task_handle;
    
    // Create task with dynamic allocation
    BaseType_t result = xTaskCreate(
        dynamic_worker_task,    // Task function
        "DynamicWorker",        // Task name
        2048,                   // Stack size
        NULL,                   // Parameters
        5,                      // Priority
        &dynamic_task_handle    // Task handle
    );
    
    if (result == pdPASS) {
        ESP_LOGI(TAG, "Dynamic task created successfully");
        ESP_LOGI(TAG, "Free heap after task creation: %d bytes", xPortGetFreeHeapSize());
        
        // Let task run for a while
        vTaskDelay(pdMS_TO_TICKS(5000));
        
        // Delete the task
        vTaskDelete(dynamic_task_handle);
        ESP_LOGI(TAG, "Dynamic task deleted");
        ESP_LOGI(TAG, "Free heap after task deletion: %d bytes", xPortGetFreeHeapSize());
    } else {
        ESP_LOGE(TAG, "Failed to create dynamic task");
    }
}

void dynamic_worker_task(void *parameter)
{
    int counter = 0;
    
    while (1) {
        ESP_LOGI(TAG, "Dynamic task running: %d", counter++);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

### Dynamic Queue and Semaphore Creation:

void dynamic_objects_example(void)
{
    ESP_LOGI(TAG, "Creating dynamic FreeRTOS objects");
    
    // Create dynamic queue
    QueueHandle_t dynamic_queue = xQueueCreate(10, sizeof(int));
    if (dynamic_queue != NULL) {
        ESP_LOGI(TAG, "Dynamic queue created");
        
        // Use queue
        int test_data = 42;
        xQueueSend(dynamic_queue, &test_data, 0);
        
        int received_data;
        xQueueReceive(dynamic_queue, &received_data, 0);
        ESP_LOGI(TAG, "Queue test data: %d", received_data);
        
        // Delete queue
        vQueueDelete(dynamic_queue);
        ESP_LOGI(TAG, "Dynamic queue deleted");
    }
    
    // Create dynamic semaphore
    SemaphoreHandle_t dynamic_semaphore = xSemaphoreCreateBinary();
    if (dynamic_semaphore != NULL) {
        ESP_LOGI(TAG, "Dynamic semaphore created");
        
        // Use semaphore
        xSemaphoreGive(dynamic_semaphore);
        xSemaphoreTake(dynamic_semaphore, 0);
        
        // Delete semaphore
        vSemaphoreDelete(dynamic_semaphore);
        ESP_LOGI(TAG, "Dynamic semaphore deleted");
    }
    
    ESP_LOGI(TAG, "Final free heap: %d bytes", xPortGetFreeHeapSize());
}