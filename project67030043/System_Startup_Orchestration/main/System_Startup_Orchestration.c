#include "freertos/event_groups.h"

// System startup events
#define HARDWARE_INIT_BIT      BIT0
#define DRIVERS_LOADED_BIT     BIT1
#define FILESYSTEM_READY_BIT   BIT2
#define NETWORK_STACK_BIT      BIT3
#define TIME_SYNCHRONIZED_BIT  BIT4
#define CONFIG_VALIDATED_BIT   BIT5
#define SERVICES_STARTED_BIT   BIT6
#define READY_FOR_USER_BIT     BIT7

// Startup phases
#define PHASE_1_COMPLETE    (HARDWARE_INIT_BIT | DRIVERS_LOADED_BIT)
#define PHASE_2_COMPLETE    (PHASE_1_COMPLETE | FILESYSTEM_READY_BIT | CONFIG_VALIDATED_BIT)
#define PHASE_3_COMPLETE    (PHASE_2_COMPLETE | NETWORK_STACK_BIT | TIME_SYNCHRONIZED_BIT)
#define SYSTEM_FULLY_READY  (PHASE_3_COMPLETE | SERVICES_STARTED_BIT | READY_FOR_USER_BIT)

EventGroupHandle_t startup_events;

void hardware_init_task(void *parameter)
{
    ESP_LOGI(TAG, "Hardware initialization starting...");
    
    // Initialize GPIO
    ESP_LOGI(TAG, "Configuring GPIO...");
    vTaskDelay(pdMS_TO_TICKS(200));
    
    // Initialize SPI/I2C
    ESP_LOGI(TAG, "Initializing communication interfaces...");
    vTaskDelay(pdMS_TO_TICKS(300));
    
    // Initialize ADC/PWM
    ESP_LOGI(TAG, "Configuring analog interfaces...");
    vTaskDelay(pdMS_TO_TICKS(150));
    
    ESP_LOGI(TAG, "Hardware initialization complete");
    xEventGroupSetBits(startup_events, HARDWARE_INIT_BIT);
    
    vTaskDelete(NULL);
}

void driver_loader_task(void *parameter)
{
    // Wait for hardware to be ready
    xEventGroupWaitBits(startup_events, HARDWARE_INIT_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    
    ESP_LOGI(TAG, "Loading device drivers...");
    
    // Load sensor drivers
    ESP_LOGI(TAG, "Loading sensor drivers...");
    vTaskDelay(pdMS_TO_TICKS(500));
    
    // Load display drivers
    ESP_LOGI(TAG, "Loading display drivers...");
    vTaskDelay(pdMS_TO_TICKS(300));
    
    // Load storage drivers
    ESP_LOGI(TAG, "Loading storage drivers...");
    vTaskDelay(pdMS_TO_TICKS(400));
    
    ESP_LOGI(TAG, "All drivers loaded");
    xEventGroupSetBits(startup_events, DRIVERS_LOADED_BIT);
    
    vTaskDelete(NULL);
}

void filesystem_init_task(void *parameter)
{
    // Wait for drivers
    xEventGroupWaitBits(startup_events, DRIVERS_LOADED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    
    ESP_LOGI(TAG, "Initializing filesystem...");
    
    // Mount filesystem
    ESP_LOGI(TAG, "Mounting flash filesystem...");
    vTaskDelay(pdMS_TO_TICKS(800));
    
    // Check filesystem integrity
    ESP_LOGI(TAG, "Checking filesystem integrity...");
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    ESP_LOGI(TAG, "Filesystem ready");
    xEventGroupSetBits(startup_events, FILESYSTEM_READY_BIT);
    
    vTaskDelete(NULL);
}

void network_init_task(void *parameter)
{
    // Wait for phase 2 completion
    xEventGroupWaitBits(startup_events, PHASE_2_COMPLETE, pdFALSE, pdTRUE, portMAX_DELAY);
    
    ESP_LOGI(TAG, "Initializing network stack...");
    
    // Initialize TCP/IP stack
    ESP_LOGI(TAG, "Starting TCP/IP stack...");
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    // Connect to network
    ESP_LOGI(TAG, "Connecting to network...");
    vTaskDelay(pdMS_TO_TICKS(3000));
    
    ESP_LOGI(TAG, "Network stack ready");
    xEventGroupSetBits(startup_events, NETWORK_STACK_BIT);
    
    // Start time synchronization
    ESP_LOGI(TAG, "Synchronizing time...");
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    ESP_LOGI(TAG, "Time synchronized");
    xEventGroupSetBits(startup_events, TIME_SYNCHRONIZED_BIT);
    
    vTaskDelete(NULL);
}

void startup_orchestrator_task(void *parameter)
{
    ESP_LOGI(TAG, "System startup orchestrator started");
    
    // Wait for each phase and log progress
    ESP_LOGI(TAG, "Waiting for Phase 1 (Hardware & Drivers)...");
    xEventGroupWaitBits(startup_events, PHASE_1_COMPLETE, pdFALSE, pdTRUE, 
                       pdMS_TO_TICKS(10000));
    ESP_LOGI(TAG, "Phase 1 complete");
    
    ESP_LOGI(TAG, "Waiting for Phase 2 (Filesystem & Config)...");
    xEventGroupWaitBits(startup_events, PHASE_2_COMPLETE, pdFALSE, pdTRUE, 
                       pdMS_TO_TICKS(15000));
    ESP_LOGI(TAG, "Phase 2 complete");
    
    ESP_LOGI(TAG, "Waiting for Phase 3 (Network & Time)...");
    xEventGroupWaitBits(startup_events, PHASE_3_COMPLETE, pdFALSE, pdTRUE, 
                       pdMS_TO_TICKS(20000));
    ESP_LOGI(TAG, "Phase 3 complete");
    
    // Start application services
    ESP_LOGI(TAG, "Starting application services...");
    start_application_services();
    xEventGroupSetBits(startup_events, SERVICES_STARTED_BIT);
    
    // Final system ready
    xEventGroupSetBits(startup_events, READY_FOR_USER_BIT);
    
    EventBits_t final_bits = xEventGroupGetBits(startup_events);
    if ((final_bits & SYSTEM_FULLY_READY) == SYSTEM_FULLY_READY) {
        ESP_LOGI(TAG, "🎉 SYSTEM FULLY OPERATIONAL 🎉");
    } else {
        ESP_LOGW(TAG, "System startup incomplete. Missing: 0x%08x", 
                 SYSTEM_FULLY_READY & ~final_bits);
    }
    
    vTaskDelete(NULL);
}

void setup_startup_orchestration(void)
{
    startup_events = xEventGroupCreate();
    
    // Create initialization tasks
    xTaskCreate(hardware_init_task, "HWInit", 2048, NULL, 8, NULL);
    xTaskCreate(driver_loader_task, "DriverLoad", 2048, NULL, 7, NULL);
    xTaskCreate(filesystem_init_task, "FSInit", 2048, NULL, 6, NULL);
    xTaskCreate(network_init_task, "NetInit", 3072, NULL, 5, NULL);
    xTaskCreate(startup_orchestrator_task, "Orchestrator", 2048, NULL, 4, NULL);
}