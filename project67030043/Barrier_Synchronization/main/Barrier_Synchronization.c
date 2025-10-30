// Barrier pattern - wait for all tasks to reach a point
#define TASK_A_READY_BIT    BIT0
#define TASK_B_READY_BIT    BIT1
#define TASK_C_READY_BIT    BIT2
#define ALL_TASKS_READY     (TASK_A_READY_BIT | TASK_B_READY_BIT | TASK_C_READY_BIT)

void barrier_task_a(void *parameter)
{
    while (1) {
        // Do task-specific work
        ESP_LOGI(TAG, "Task A: Doing work phase 1");
        vTaskDelay(pdMS_TO_TICKS(1000 + rand() % 2000));
        
        // Signal ready for synchronization
        ESP_LOGI(TAG, "Task A: Ready for barrier");
        xEventGroupSetBits(system_event_group, TASK_A_READY_BIT);
        
        // Wait for all tasks to be ready
        xEventGroupWaitBits(system_event_group, ALL_TASKS_READY, pdTRUE, pdTRUE, portMAX_DELAY);
        
        ESP_LOGI(TAG, "Task A: All tasks synchronized, continuing");
        
        // Do synchronized work
        synchronized_work_phase();
    }
}

void barrier_task_b(void *parameter)
{
    while (1) {
        ESP_LOGI(TAG, "Task B: Doing work phase 1");
        vTaskDelay(pdMS_TO_TICKS(500 + rand() % 3000));
        
        ESP_LOGI(TAG, "Task B: Ready for barrier");
        xEventGroupSetBits(system_event_group, TASK_B_READY_BIT);
        
        xEventGroupWaitBits(system_event_group, ALL_TASKS_READY, pdTRUE, pdTRUE, portMAX_DELAY);
        
        ESP_LOGI(TAG, "Task B: All tasks synchronized, continuing");
        synchronized_work_phase();
    }
}

void barrier_task_c(void *parameter)
{
    while (1) {
        ESP_LOGI(TAG, "Task C: Doing work phase 1");
        vTaskDelay(pdMS_TO_TICKS(800 + rand() % 1500));
        
        ESP_LOGI(TAG, "Task C: Ready for barrier");
        xEventGroupSetBits(system_event_group, TASK_C_READY_BIT);
        
        xEventGroupWaitBits(system_event_group, ALL_TASKS_READY, pdTRUE, pdTRUE, portMAX_DELAY);
        
        ESP_LOGI(TAG, "Task C: All tasks synchronized, continuing");
        synchronized_work_phase();
    }
}