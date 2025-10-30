#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Static memory for task
#define STATIC_TASK_STACK_SIZE 2048
static StackType_t static_task_stack[STATIC_TASK_STACK_SIZE];
static StaticTask_t static_task_tcb;

void static_allocation_example(void)
{
    ESP_LOGI(TAG, "Creating static task");
    ESP_LOGI(TAG, "Free heap before static task: %d bytes", xPortGetFreeHeapSize());
    
    // Create task with static allocation
    TaskHandle_t static_task_handle = xTaskCreateStatic(
        static_worker_task,         // Task function
        "StaticWorker",             // Task name
        STATIC_TASK_STACK_SIZE,     // Stack size
        NULL,                       // Parameters
        5,                          // Priority
        static_task_stack,          // Stack memory
        &static_task_tcb            // Task control block
    );
    
    if (static_task_handle != NULL) {
        ESP_LOGI(TAG, "Static task created successfully");
        ESP_LOGI(TAG, "Free heap after static task: %d bytes", xPortGetFreeHeapSize());
        ESP_LOGI(TAG, "Note: No heap memory used for static task");
    } else {
        ESP_LOGE(TAG, "Failed to create static task");
    }
}

void static_worker_task(void *parameter)
{
    int counter = 0;
    
    while (1) {
        ESP_LOGI(TAG, "Static task running: %d", counter++);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
```

### Static Queue and Semaphore Creation:

```c
// Static queue memory
#define STATIC_QUEUE_LENGTH 10
static uint8_t static_queue_storage[STATIC_QUEUE_LENGTH * sizeof(int)];
static StaticQueue_t static_queue_struct;

// Static semaphore memory
static StaticSemaphore_t static_semaphore_struct;

void static_objects_example(void)
{
    ESP_LOGI(TAG, "Creating static FreeRTOS objects");
    ESP_LOGI(TAG, "Free heap before static objects: %d bytes", xPortGetFreeHeapSize());
    
    // Create static queue
    QueueHandle_t static_queue = xQueueCreateStatic(
        STATIC_QUEUE_LENGTH,        // Queue length
        sizeof(int),                // Item size
        static_queue_storage,       // Storage buffer
        &static_queue_struct        // Queue struct
    );
    
    if (static_queue != NULL) {
        ESP_LOGI(TAG, "Static queue created");
        
        // Test queue
        int test_data = 123;
        xQueueSend(static_queue, &test_data, 0);
        
        int received_data;
        xQueueReceive(static_queue, &received_data, 0);
        ESP_LOGI(TAG, "Static queue test data: %d", received_data);
    }
    
    // Create static binary semaphore
    SemaphoreHandle_t static_semaphore = xSemaphoreCreateBinaryStatic(&static_semaphore_struct);
    if (static_semaphore != NULL) {
        ESP_LOGI(TAG, "Static semaphore created");
        
        // Test semaphore
        xSemaphoreGive(static_semaphore);
        if (xSemaphoreTake(static_semaphore, 0) == pdTRUE) {
            ESP_LOGI(TAG, "Static semaphore test passed");
        }
    }
    
    ESP_LOGI(TAG, "Free heap after static objects: %d bytes", xPortGetFreeHeapSize());
    ESP_LOGI(TAG, "Note: No heap memory used for static objects");
}