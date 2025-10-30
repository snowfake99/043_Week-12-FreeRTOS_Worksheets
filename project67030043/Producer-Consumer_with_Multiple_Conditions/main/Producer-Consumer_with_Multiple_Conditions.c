#define DATA_AVAILABLE_BIT     BIT0
#define BUFFER_NOT_FULL_BIT    BIT1
#define PROCESSING_READY_BIT   BIT2
#define CONSUMER_READY_BIT     BIT3

void multi_condition_producer(void *parameter)
{
    int data_id = 0;
    
    while (1) {
        // Wait for buffer to have space and consumer to be ready
        EventBits_t bits = xEventGroupWaitBits(
            system_event_group,
            BUFFER_NOT_FULL_BIT | CONSUMER_READY_BIT,
            pdFALSE,     // Don't clear bits
            pdTRUE,      // Wait for ALL conditions
            portMAX_DELAY
        );
        
        if ((bits & (BUFFER_NOT_FULL_BIT | CONSUMER_READY_BIT)) == 
            (BUFFER_NOT_FULL_BIT | CONSUMER_READY_BIT)) {
            
            // Produce data
            ESP_LOGI(TAG, "Producer: Creating data item %d", data_id++);
            // add_to_buffer(data_id);
            
            // Signal data is available
            xEventGroupSetBits(system_event_group, DATA_AVAILABLE_BIT);
            
            // Check if buffer is now full
            // if (buffer_is_full()) {
            //     xEventGroupClearBits(system_event_group, BUFFER_NOT_FULL_BIT);
            // }
        }
        
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void multi_condition_consumer(void *parameter)
{
    // Signal consumer is ready
    xEventGroupSetBits(system_event_group, CONSUMER_READY_BIT);
    
    while (1) {
        // Wait for data to be available and processing to be ready
        EventBits_t bits = xEventGroupWaitBits(
            system_event_group,
            DATA_AVAILABLE_BIT | PROCESSING_READY_BIT,
            pdFALSE,     // Don't clear bits yet
            pdTRUE,      // Wait for ALL conditions
            pdMS_TO_TICKS(5000)
        );
        
        if ((bits & (DATA_AVAILABLE_BIT | PROCESSING_READY_BIT)) == 
            (DATA_AVAILABLE_BIT | PROCESSING_READY_BIT)) {
            
            ESP_LOGI(TAG, "Consumer: Processing data");
            // int data = get_from_buffer();
            // process_data(data);
            
            // Clear data available if buffer is now empty
            // if (buffer_is_empty()) {
            //     xEventGroupClearBits(system_event_group, DATA_AVAILABLE_BIT);
            // }
            
            // Signal buffer has space
            xEventGroupSetBits(system_event_group, BUFFER_NOT_FULL_BIT);
            
            vTaskDelay(pdMS_TO_TICKS(1000)); // Processing time
        } else {
            ESP_LOGW(TAG, "Consumer: Timeout waiting for conditions");
        }
    }
}