// Sensor event bits
#define TEMP_SENSOR_DATA_BIT    BIT0
#define HUMID_SENSOR_DATA_BIT   BIT1
#define PRESS_SENSOR_DATA_BIT   BIT2
#define LIGHT_SENSOR_DATA_BIT   BIT3
#define MOTION_SENSOR_DATA_BIT  BIT4

// Processing state bits
#define CALIBRATION_ACTIVE_BIT  BIT8
#define FUSION_READY_BIT        BIT9
#define DATA_VALID_BIT          BIT10
#define ALERT_CONDITION_BIT     BIT11

// Sensor combinations
#define BASIC_ENVIRONMENTAL    (TEMP_SENSOR_DATA_BIT | HUMID_SENSOR_DATA_BIT)
#define FULL_ENVIRONMENTAL     (BASIC_ENVIRONMENTAL | PRESS_SENSOR_DATA_BIT | LIGHT_SENSOR_DATA_BIT)
#define ALL_SENSORS           (FULL_ENVIRONMENTAL | MOTION_SENSOR_DATA_BIT)

EventGroupHandle_t sensor_events;

typedef struct {
    float temperature;
    float humidity;
    float pressure;
    float light_level;
    bool motion_detected;
    uint32_t timestamp;
} sensor_fusion_data_t;

sensor_fusion_data_t current_sensor_data;

void temperature_sensor_task(void *parameter)
{
    while (1) {
        // Read temperature sensor
        current_sensor_data.temperature = 20.0 + (rand() % 300) / 10.0;
        current_sensor_data.timestamp = xTaskGetTickCount();
        
        ESP_LOGI(TAG, "Temperature: %.1f°C", current_sensor_data.temperature);
        
        // Signal data available
        xEventGroupSetBits(sensor_events, TEMP_SENSOR_DATA_BIT);
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void humidity_sensor_task(void *parameter)
{
    while (1) {
        current_sensor_data.humidity = 30.0 + (rand() % 700) / 10.0;
        ESP_LOGI(TAG, "Humidity: %.1f%%", current_sensor_data.humidity);
        
        xEventGroupSetBits(sensor_events, HUMID_SENSOR_DATA_BIT);
        
        vTaskDelay(pdMS_TO_TICKS(1500));
    }
}

void pressure_sensor_task(void *parameter)
{
    while (1) {
        current_sensor_data.pressure = 980.0 + (rand() % 400) / 10.0;
        ESP_LOGI(TAG, "Pressure: %.1f hPa", current_sensor_data.pressure);
        
        xEventGroupSetBits(sensor_events, PRESS_SENSOR_DATA_BIT);
        
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void sensor_fusion_task(void *parameter)
{
    ESP_LOGI(TAG, "Sensor fusion task started");
    
    while (1) {
        // Wait for basic environmental data
        EventBits_t bits = xEventGroupWaitBits(
            sensor_events,
            BASIC_ENVIRONMENTAL,
            pdTRUE,          // Clear bits after reading
            pdTRUE,          // Wait for ALL bits
            pdMS_TO_TICKS(5000)
        );
        
        if ((bits & BASIC_ENVIRONMENTAL) == BASIC_ENVIRONMENTAL) {
            ESP_LOGI(TAG, "Basic environmental data ready - performing fusion");
            
            // Basic fusion calculation
            float comfort_index = calculate_comfort_index(
                current_sensor_data.temperature, 
                current_sensor_data.humidity
            );
            
            ESP_LOGI(TAG, "Comfort index: %.1f", comfort_index);
            
            // Check for full environmental data
            bits = xEventGroupGetBits(sensor_events);
            if ((bits & FULL_ENVIRONMENTAL) == FULL_ENVIRONMENTAL) {
                ESP_LOGI(TAG, "Full environmental data available");
                
                // Advanced fusion with all environmental sensors
                float environmental_index = calculate_environmental_index(
                    current_sensor_data.temperature,
                    current_sensor_data.humidity,
                    current_sensor_data.pressure,
                    current_sensor_data.light_level
                );
                
                ESP_LOGI(TAG, "Environmental index: %.1f", environmental_index);
                
                // Clear the additional sensor bits
                xEventGroupClearBits(sensor_events, PRESS_SENSOR_DATA_BIT | LIGHT_SENSOR_DATA_BIT);
                
                // Check for alert conditions
                if (environmental_index > 80.0 || environmental_index < 20.0) {
                    ESP_LOGW(TAG, "Environmental alert condition detected!");
                    xEventGroupSetBits(sensor_events, ALERT_CONDITION_BIT);
                }
            }
            
            xEventGroupSetBits(sensor_events, DATA_VALID_BIT);
        } else {
            ESP_LOGW(TAG, "Timeout waiting for basic sensor data");
        }
    }
}

void alert_handler_task(void *parameter)
{
    while (1) {
        // Wait for alert conditions
        xEventGroupWaitBits(sensor_events, ALERT_CONDITION_BIT, pdTRUE, pdTRUE, portMAX_DELAY);
        
        ESP_LOGW(TAG, "🚨 ALERT: Environmental conditions out of range!");
        ESP_LOGW(TAG, "Temperature: %.1f°C, Humidity: %.1f%%", 
                 current_sensor_data.temperature, current_sensor_data.humidity);
        
        // Handle alert (notifications, logging, etc.)
        handle_environmental_alert();
        
        // Wait before checking again
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

float calculate_comfort_index(float temp, float humidity)
{
    // Simple comfort calculation
    float ideal_temp = 22.0;
    float ideal_humidity = 50.0;
    
    float temp_factor = 100.0 - abs(temp - ideal_temp) * 5.0;
    float humidity_factor = 100.0 - abs(humidity - ideal_humidity) * 2.0;
    
    return (temp_factor + humidity_factor) / 2.0;
}

float calculate_environmental_index(float temp, float humidity, float pressure, float light)
{
    // More complex environmental calculation
    float comfort = calculate_comfort_index(temp, humidity);
    float pressure_factor = (pressure > 1000.0) ? 100.0 : pressure / 10.0;
    float light_factor = (light > 500.0) ? 100.0 : light / 5.0;
    
    return (comfort * 0.5) + (pressure_factor * 0.25) + (light_factor * 0.25);
}