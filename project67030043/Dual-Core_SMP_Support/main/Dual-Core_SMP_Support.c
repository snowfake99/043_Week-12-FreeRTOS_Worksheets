#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

static const char *TAG = "DUAL_CORE";

void core_info_task(void *parameter)
{
    int core_id = xPortGetCoreID();
    const char* core_name = (core_id == 0) ? "PRO_CPU" : "APP_CPU";
    
    ESP_LOGI(TAG, "Task running on Core %d (%s)", core_id, core_name);
    
    while (1) {
        ESP_LOGI(TAG, "Core %d: Task executing", core_id);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "ESP32 Dual-Core FreeRTOS Demo");
    ESP_LOGI(TAG, "Main task running on Core %d", xPortGetCoreID());
    
    // Create tasks without affinity (can run on either core)
    xTaskCreate(core_info_task, "CoreTask1", 2048, NULL, 5, NULL);
    xTaskCreate(core_info_task, "CoreTask2", 2048, NULL, 5, NULL);
    xTaskCreate(core_info_task, "CoreTask3", 2048, NULL, 5, NULL);
    xTaskCreate(core_info_task, "CoreTask4", 2048, NULL, 5, NULL);
    
    ESP_LOGI(TAG, "Tasks created - they will distribute across cores");
}


// Core affinity definitions
#define tskNO_AFFINITY      0x7FFFFFFF  // Task can run on any core
#define PRO_CPU_NUM         0           // Core 0 (Protocol CPU)
#define APP_CPU_NUM         1           // Core 1 (Application CPU)

void core_pinned_tasks_example(void)
{
    ESP_LOGI(TAG, "Creating core-pinned tasks");
    
    // Task pinned to Core 0 (PRO_CPU)
    xTaskCreatePinnedToCore(
        realtime_task,          // Task function
        "RealtimeTask",         // Task name
        2048,                   // Stack size
        NULL,                   // Parameters
        20,                     // High priority
        NULL,                   // Task handle
        PRO_CPU_NUM             // Pin to Core 0
    );
    
    // Task pinned to Core 1 (APP_CPU)
    xTaskCreatePinnedToCore(
        communication_task,     // Task function
        "CommTask",             // Task name
        4096,                   // Stack size
        NULL,                   // Parameters
        15,                     // Medium priority
        NULL,                   // Task handle
        APP_CPU_NUM             // Pin to Core 1
    );
    
    // Task with no affinity (can run on either core)
    xTaskCreate(
        background_task,        // Task function
        "BackgroundTask",       // Task name
        2048,                   // Stack size
        NULL,                   // Parameters
        5,                      // Low priority
        NULL                    // Task handle
        // No core specification = tskNO_AFFINITY
    );
}

void realtime_task(void *parameter)
{
    ESP_LOGI(TAG, "Real-time task on Core %d", xPortGetCoreID());
    
    while (1) {
        // Time-critical operations
        ESP_LOGI(TAG, "Real-time processing on Core %d", xPortGetCoreID());
        
        // Precise timing operations
        precise_timing_operation();
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void communication_task(void *parameter)
{
    ESP_LOGI(TAG, "Communication task on Core %d", xPortGetCoreID());
    
    while (1) {
        // Network and communication operations
        ESP_LOGI(TAG, "Communication processing on Core %d", xPortGetCoreID());
        
        // WiFi/Bluetooth operations
        network_operations();
        
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void background_task(void *parameter)
{
    while (1) {
        int current_core = xPortGetCoreID();
        ESP_LOGI(TAG, "Background task currently on Core %d", current_core);
        
        // This task can migrate between cores as needed
        background_processing();
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void dynamic_affinity_example(void)
{
    TaskHandle_t flexible_task_handle;
    
    // Create task without initial affinity
    xTaskCreate(flexible_worker_task, "FlexibleWorker", 2048, NULL, 10, &flexible_task_handle);
    
    // Let it run for a while
    vTaskDelay(pdMS_TO_TICKS(5000));
    
    // Pin to Core 0 for performance-critical phase
    ESP_LOGI(TAG, "Pinning task to Core 0 for performance phase");
    vTaskCoreAffinitySet(flexible_task_handle, (1 << PRO_CPU_NUM));
    
    vTaskDelay(pdMS_TO_TICKS(5000));
    
    // Pin to Core 1 for different workload
    ESP_LOGI(TAG, "Moving task to Core 1 for communication phase");
    vTaskCoreAffinitySet(flexible_task_handle, (1 << APP_CPU_NUM));
    
    vTaskDelay(pdMS_TO_TICKS(5000));
    
    // Remove affinity - allow scheduling on any core
    ESP_LOGI(TAG, "Removing affinity - task can run on any core");
    vTaskCoreAffinitySet(flexible_task_handle, tskNO_AFFINITY);
}

void flexible_worker_task(void *parameter)
{
    int phase = 0;
    
    while (1) {
        int current_core = xPortGetCoreID();
        ESP_LOGI(TAG, "Flexible task phase %d on Core %d", phase, current_core);
        
        switch (phase % 3) {
            case 0:
                // Compute-intensive work
                compute_intensive_work();
                break;
            case 1:
                // I/O operations
                io_operations();
                break;
            case 2:
                // Network operations
                network_operations();
                break;
        }
        
        phase++;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


// Function to execute on remote core
void remote_core_function(void *arg)
{
    int *data = (int*)arg;
    int core_id = xPortGetCoreID();
    
    ESP_LOGI(TAG, "IPC function executing on Core %d with data: %d", core_id, *data);
    
    // Perform core-specific operation
    *data = *data * 2; // Example operation
    
    ESP_LOGI(TAG, "IPC function completed on Core %d, result: %d", core_id, *data);
}

void ipc_communication_example(void)
{
    int current_core = xPortGetCoreID();
    int target_core = (current_core == 0) ? 1 : 0;
    
    ESP_LOGI(TAG, "Running on Core %d, calling function on Core %d", current_core, target_core);
    
    int shared_data = 42;
    
    // Call function on remote core
    esp_err_t result = esp_ipc_call_blocking(target_core, remote_core_function, &shared_data);
    
    if (result == ESP_OK) {
        ESP_LOGI(TAG, "IPC call successful, shared_data is now: %d", shared_data);
    } else {
        ESP_LOGE(TAG, "IPC call failed: %s", esp_err_to_name(result));
    }
}


// Core-specific data structures
typedef struct {
    int core_id;
    QueueHandle_t input_queue;
    QueueHandle_t output_queue;
    SemaphoreHandle_t processing_semaphore;
    TaskHandle_t worker_task;
} core_context_t;

static core_context_t core_contexts[2];

void setup_core_specific_resources(void)
{
    for (int core = 0; core < 2; core++) {
        core_contexts[core].core_id = core;
        core_contexts[core].input_queue = xQueueCreate(10, sizeof(int));
        core_contexts[core].output_queue = xQueueCreate(10, sizeof(int));
        core_contexts[core].processing_semaphore = xSemaphoreCreateBinary();
        
        // Create worker task pinned to specific core
        char task_name[20];
        sprintf(task_name, "CoreWorker%d", core);
        
        xTaskCreatePinnedToCore(
            core_worker_task,
            task_name,
            2048,
            &core_contexts[core],
            10,
            &core_contexts[core].worker_task,
            core
        );
        
        ESP_LOGI(TAG, "Setup resources for Core %d", core);
    }
}

void core_worker_task(void *parameter)
{
    core_context_t *ctx = (core_context_t*)parameter;
    int data;
    
    ESP_LOGI(TAG, "Core %d worker task started", ctx->core_id);
    
    while (1) {
        // Wait for work
        if (xQueueReceive(ctx->input_queue, &data, portMAX_DELAY) == pdTRUE) {
            ESP_LOGI(TAG, "Core %d processing data: %d", ctx->core_id, data);
            
            // Process data (core-specific algorithm)
            int result = data * ctx->core_id + 100;
            
            // Simulate processing time
            vTaskDelay(pdMS_TO_TICKS(100));
            
            // Send result
            xQueueSend(ctx->output_queue, &result, 0);
            
            ESP_LOGI(TAG, "Core %d completed processing: %d -> %d", 
                     ctx->core_id, data, result);
        }
    }
}

void distribute_work_to_cores(void)
{
    ESP_LOGI(TAG, "Distributing work to both cores");
    
    // Send work to both cores
    for (int work_item = 1; work_item <= 10; work_item++) {
        int target_core = work_item % 2; // Alternate between cores
        
        if (xQueueSend(core_contexts[target_core].input_queue, &work_item, 
                      pdMS_TO_TICKS(1000)) == pdTRUE) {
            ESP_LOGI(TAG, "Sent work item %d to Core %d", work_item, target_core);
        }
    }
    
    // Collect results
    for (int core = 0; core < 2; core++) {
        int result;
        int items_received = 0;
        
        ESP_LOGI(TAG, "Collecting results from Core %d", core);
        
        while (xQueueReceive(core_contexts[core].output_queue, &result, 
                           pdMS_TO_TICKS(2000)) == pdTRUE) {
            ESP_LOGI(TAG, "Received result from Core %d: %d", core, result);
            items_received++;
            
            if (items_received >= 5) break; // Expect 5 items per core
        }
    }
}