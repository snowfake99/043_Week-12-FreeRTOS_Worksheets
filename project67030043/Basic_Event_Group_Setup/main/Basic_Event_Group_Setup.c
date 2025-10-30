#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"

static const char *TAG = "EVENT_GROUP";

// Event group handle
EventGroupHandle_t system_event_group;

// Event bit definitions
#define WIFI_CONNECTED_BIT     BIT0
#define TIME_SYNCED_BIT        BIT1
#define SENSORS_READY_BIT      BIT2
#define CONFIG_LOADED_BIT      BIT3
#define CALIBRATION_DONE_BIT   BIT4

// Combined conditions
#define BASIC_SYSTEM_READY     (WIFI_CONNECTED_BIT | CONFIG_LOADED_BIT)
#define FULL_SYSTEM_READY      (BASIC_SYSTEM_READY | TIME_SYNCED_BIT | SENSORS_READY_BIT | CALIBRATION_DONE_BIT)

void app_main(void)
{
    ESP_LOGI(TAG, "Creating system event group...");
    
    // Create event group
    system_event_group = xEventGroupCreate();
    if (system_event_group == NULL) {
        ESP_LOGE(TAG, "Failed to create event group");
        return;
    }
    
    ESP_LOGI(TAG, "Event group created successfully");
    
    // Create system initialization tasks
    xTaskCreate(wifi_init_task, "WiFiInit", 3072, NULL, 6, NULL);
    xTaskCreate(time_sync_task, "TimeSync", 2048, NULL, 5, NULL);
    xTaskCreate(sensor_init_task, "SensorInit", 2048, NULL, 5, NULL);
    xTaskCreate(config_loader_task, "ConfigLoader", 2048, NULL, 5, NULL);
    xTaskCreate(calibration_task, "Calibration", 2048, NULL, 4, NULL);
    
    // Create main application task that waits for system ready
    xTaskCreate(main_app_task, "MainApp", 4096, NULL, 3, NULL);
    
    // Create monitoring task
    xTaskCreate(system_monitor_task, "Monitor", 2048, NULL, 2, NULL);
}
```

## Waiting for Events

### Basic Wait Operations:

```c
void main_app_task(void *parameter)
{
    ESP_LOGI(TAG, "Main application starting...");
    
    // Wait for basic system to be ready (ANY of the required bits)
    ESP_LOGI(TAG, "Waiting for basic system initialization...");
    EventBits_t bits = xEventGroupWaitBits(
        system_event_group,      // Event group handle
        BASIC_SYSTEM_READY,      // Bits to wait for
        pdFALSE,                 // Don't clear bits on exit
        pdTRUE,                  // Wait for ALL bits (AND)
        portMAX_DELAY            // Wait forever
    );
    
    if ((bits & BASIC_SYSTEM_READY) == BASIC_SYSTEM_READY) {
        ESP_LOGI(TAG, "Basic system ready - starting core functions");
        start_core_functions();
    }
    
    // Wait for full system ready
    ESP_LOGI(TAG, "Waiting for full system initialization...");
    bits = xEventGroupWaitBits(
        system_event_group,
        FULL_SYSTEM_READY,
        pdFALSE,                 // Don't clear bits
        pdTRUE,                  // Wait for ALL bits
        pdMS_TO_TICKS(30000)     // Timeout after 30 seconds
    );
    
    if ((bits & FULL_SYSTEM_READY) == FULL_SYSTEM_READY) {
        ESP_LOGI(TAG, "Full system ready - starting advanced features");
        start_advanced_features();
    } else {
        ESP_LOGW(TAG, "System initialization timeout - running with limited features");
        ESP_LOGW(TAG, "Missing bits: 0x%08x", FULL_SYSTEM_READY & ~bits);
        start_limited_mode();
    }
    
    // Main application loop
    while (1) {
        // Check for shutdown request
        bits = xEventGroupGetBits(system_event_group);
        if (bits & SHUTDOWN_REQUEST_BIT) {
            ESP_LOGI(TAG, "Shutdown requested - cleaning up...");
            cleanup_and_shutdown();
            break;
        }
        
        // Do main application work
        run_main_application();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void conditional_wait_examples(void)
{
    EventBits_t bits;
    
    // Wait for ANY of multiple events (OR condition)
    ESP_LOGI(TAG, "Waiting for any network connection...");
    bits = xEventGroupWaitBits(
        system_event_group,
        WIFI_CONNECTED_BIT | ETHERNET_CONNECTED_BIT | CELLULAR_CONNECTED_BIT,
        pdFALSE,     // Don't clear bits
        pdFALSE,     // Wait for ANY (OR)
        pdMS_TO_TICKS(10000)
    );
    
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "WiFi connection established");
    } else if (bits & ETHERNET_CONNECTED_BIT) {
        ESP_LOGI(TAG, "Ethernet connection established");
    } else if (bits & CELLULAR_CONNECTED_BIT) {
        ESP_LOGI(TAG, "Cellular connection established");
    } else {
        ESP_LOGW(TAG, "No network connection within timeout");
    }
    
    // Wait for ALL required events (AND condition)
    ESP_LOGI(TAG, "Waiting for sensor subsystem ready...");
    bits = xEventGroupWaitBits(
        system_event_group,
        SENSOR_POWER_ON_BIT | SENSOR_CALIBRATED_BIT | SENSOR_SELFTEST_OK_BIT,
        pdTRUE,      // Clear bits after wait
        pdTRUE,      // Wait for ALL (AND)
        pdMS_TO_TICKS(5000)
    );
    
    if ((bits & (SENSOR_POWER_ON_BIT | SENSOR_CALIBRATED_BIT | SENSOR_SELFTEST_OK_BIT)) == 
        (SENSOR_POWER_ON_BIT | SENSOR_CALIBRATED_BIT | SENSOR_SELFTEST_OK_BIT)) {
        ESP_LOGI(TAG, "All sensor subsystems ready");
        start_sensor_operations();
    } else {
        ESP_LOGE(TAG, "Sensor initialization failed");
        handle_sensor_error();
    }
}


void wifi_init_task(void *parameter)
{
    ESP_LOGI(TAG, "WiFi initialization task starting");
    
    // Simulate WiFi initialization
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    ESP_LOGI(TAG, "WiFi connecting...");
    // wifi_connect(); // Actual WiFi connection code
    
    vTaskDelay(pdMS_TO_TICKS(3000));
    
    ESP_LOGI(TAG, "WiFi connected successfully");
    
    // Set WiFi connected bit
    xEventGroupSetBits(system_event_group, WIFI_CONNECTED_BIT);
    
    // Continue monitoring WiFi status
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(5000));
        
        // Check WiFi status
        // if (!wifi_is_connected()) {
        //     ESP_LOGW(TAG, "WiFi disconnected");
        //     xEventGroupClearBits(system_event_group, WIFI_CONNECTED_BIT);
        //     
        //     // Attempt reconnection
        //     wifi_reconnect();
        //     xEventGroupSetBits(system_event_group, WIFI_CONNECTED_BIT);
        // }
    }
}

void sensor_init_task(void *parameter)
{
    ESP_LOGI(TAG, "Sensor initialization starting");
    
    // Initialize sensors step by step
    ESP_LOGI(TAG, "Powering on sensors...");
    // sensor_power_on();
    vTaskDelay(pdMS_TO_TICKS(1000));
    xEventGroupSetBits(system_event_group, SENSOR_POWER_ON_BIT);
    
    ESP_LOGI(TAG, "Running sensor self-tests...");
    // bool selftest_ok = sensor_selftest();
    vTaskDelay(pdMS_TO_TICKS(2000));
    // if (selftest_ok) {
        xEventGroupSetBits(system_event_group, SENSOR_SELFTEST_OK_BIT);
        ESP_LOGI(TAG, "Sensor self-test passed");
    // } else {
    //     ESP_LOGE(TAG, "Sensor self-test failed");
    //     xEventGroupSetBits(system_event_group, SENSOR_ERROR_BIT);
    //     vTaskDelete(NULL);
    // }
    
    ESP_LOGI(TAG, "Calibrating sensors...");
    // sensor_calibrate();
    vTaskDelay(pdMS_TO_TICKS(3000));
    xEventGroupSetBits(system_event_group, SENSOR_CALIBRATED_BIT);
    
    ESP_LOGI(TAG, "Sensors ready");
    xEventGroupSetBits(system_event_group, SENSORS_READY_BIT);
    
    vTaskDelete(NULL); // Initialization complete
}


void config_loader_task(void *parameter)
{
    ESP_LOGI(TAG, "Loading system configuration...");
    
    // Clear any previous config status
    xEventGroupClearBits(system_event_group, CONFIG_LOADED_BIT | CONFIG_ERROR_BIT);
    
    // Load configuration files
    bool config_success = true;
    
    ESP_LOGI(TAG, "Loading network config...");
    vTaskDelay(pdMS_TO_TICKS(500));
    // config_success &= load_network_config();
    
    ESP_LOGI(TAG, "Loading sensor config...");
    vTaskDelay(pdMS_TO_TICKS(300));
    // config_success &= load_sensor_config();
    
    ESP_LOGI(TAG, "Loading application config...");
    vTaskDelay(pdMS_TO_TICKS(200));
    // config_success &= load_app_config();
    
    if (config_success) {
        ESP_LOGI(TAG, "Configuration loaded successfully");
        xEventGroupSetBits(system_event_group, CONFIG_LOADED_BIT);
    } else {
        ESP_LOGE(TAG, "Configuration loading failed");
        xEventGroupSetBits(system_event_group, CONFIG_ERROR_BIT);
    }
    
    vTaskDelete(NULL);
}

void error_handler_task(void *parameter)
{
    EventBits_t error_bits = CONFIG_ERROR_BIT | SENSOR_ERROR_BIT | NETWORK_ERROR_BIT;
    
    while (1) {
        // Wait for any error condition
        EventBits_t bits = xEventGroupWaitBits(
            system_event_group,
            error_bits,
            pdTRUE,          // Clear error bits after handling
            pdFALSE,         // Wait for ANY error
            portMAX_DELAY
        );
        
        if (bits & CONFIG_ERROR_BIT) {
            ESP_LOGE(TAG, "Handling configuration error");
            handle_config_error();
        }
        
        if (bits & SENSOR_ERROR_BIT) {
            ESP_LOGE(TAG, "Handling sensor error");
            handle_sensor_error();
        }
        
        if (bits & NETWORK_ERROR_BIT) {
            ESP_LOGE(TAG, "Handling network error");
            handle_network_error();
        }
    }
}