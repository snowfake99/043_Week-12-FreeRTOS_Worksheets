# Lab 2: Time-Sharing Implementation

## วัตถุประสงค์
ทำความเข้าใจ Time-Sharing และสัศจันปัญหาที่เกิดขึ้นจากการแบ่งเวลาแบบ manual

## เวลาที่ใช้
45 นาที

## อุปกรณ์ที่ต้องใช้
- ESP32 Development Board
- LED 4 ดวง
- Logic Analyzer หรือ Oscilloscope (ถ้ามี)

## ขั้นตอนการทดลอง

### Part 1: Simple Time-Sharing (25 นาที) 
```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

#define LED1_PIN GPIO_NUM_2
#define LED2_PIN GPIO_NUM_4
#define LED3_PIN GPIO_NUM_5
#define LED4_PIN GPIO_NUM_18

static const char *TAG = "TIME_SHARING";

// Task states
typedef enum {
    TASK_SENSOR,
    TASK_PROCESS,
    TASK_ACTUATOR,
    TASK_DISPLAY,
    TASK_COUNT
} task_id_t;

// Time slice in milliseconds
#define TIME_SLICE_MS 50

// Global variables
static uint32_t task_counter = 0;
static uint64_t context_switch_time = 0;
static uint32_t context_switches = 0;

// Simulate different task workloads
void simulate_sensor_task(void)
{
    static uint32_t sensor_count = 0;
    ESP_LOGI(TAG, "Sensor Task %d", sensor_count++);
    
    gpio_set_level(LED1_PIN, 1);
    
    // Simulate sensor reading (light work)
    for (int i = 0; i < 10000; i++) {
        volatile int dummy = i;
    }
    
    gpio_set_level(LED1_PIN, 0);
}

void simulate_processing_task(void)
{
    static uint32_t process_count = 0;
    ESP_LOGI(TAG, "Processing Task %d", process_count++);
    
    gpio_set_level(LED2_PIN, 1);
    
    // Simulate heavy computation
    for (int i = 0; i < 100000; i++) {
        volatile int dummy = i * i;
    }
    
    gpio_set_level(LED2_PIN, 0);
}

void simulate_actuator_task(void)
{
    static uint32_t actuator_count = 0;
    ESP_LOGI(TAG, "Actuator Task %d", actuator_count++);
    
    gpio_set_level(LED3_PIN, 1);
    
    // Simulate actuator control (medium work)
    for (int i = 0; i < 50000; i++) {
        volatile int dummy = i + 100;
    }
    
    gpio_set_level(LED3_PIN, 0);
}

void simulate_display_task(void)
{
    static uint32_t display_count = 0;
    ESP_LOGI(TAG, "Display Task %d", display_count++);
    
    gpio_set_level(LED4_PIN, 1);
    
    // Simulate display update (quick work)
    for (int i = 0; i < 20000; i++) {
        volatile int dummy = i / 2;
    }
    
    gpio_set_level(LED4_PIN, 0);
}

// Manual context switching
void manual_scheduler(void)
{
    uint64_t start_time = esp_timer_get_time();
    
    // Save context (simulated)
    context_switches++;
    
    // Context switch overhead simulation
    for (int i = 0; i < 1000; i++) {
        volatile int dummy = i;
    }
    
    // Execute current task
    switch (task_counter % TASK_COUNT) {
        case TASK_SENSOR:
            simulate_sensor_task();
            break;
        case TASK_PROCESS:
            simulate_processing_task();
            break;
        case TASK_ACTUATOR:
            simulate_actuator_task();
            break;
        case TASK_DISPLAY:
            simulate_display_task();
            break;
    }
    
    // More context switch overhead
    for (int i = 0; i < 1000; i++) {
        volatile int dummy = i;
    }
    
    uint64_t end_time = esp_timer_get_time();
    context_switch_time += (end_time - start_time);
    
    task_counter++;
}

void app_main(void)
{
    // GPIO Configuration
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << LED1_PIN) | (1ULL << LED2_PIN) | 
                        (1ULL << LED3_PIN) | (1ULL << LED4_PIN),
        .pull_down_en = 0,
        .pull_up_en = 0,
    };
    gpio_config(&io_conf);

    ESP_LOGI(TAG, "Time-Sharing System Started");
    ESP_LOGI(TAG, "Time slice: %d ms", TIME_SLICE_MS);

    uint64_t start_time = esp_timer_get_time();
    uint32_t round_count = 0;

    while (1) {
        manual_scheduler();
        
        // Wait for time slice
        vTaskDelay(pdMS_TO_TICKS(TIME_SLICE_MS));
        
        // Report statistics every 20 context switches
        if (context_switches % 20 == 0) {
            round_count++;
            uint64_t current_time = esp_timer_get_time();
            uint64_t total_time = current_time - start_time;
            
            float cpu_utilization = ((float)(context_switch_time) / (float)(total_time)) * 100.0f;
            float overhead_percentage = 100.0f - cpu_utilization;
            
            ESP_LOGI(TAG, "=== Round %d Statistics ===", round_count);
            ESP_LOGI(TAG, "Context switches: %d", context_switches);
            ESP_LOGI(TAG, "Total time: %lld us", total_time);
            ESP_LOGI(TAG, "Task execution time: %lld us", context_switch_time);
            ESP_LOGI(TAG, "CPU utilization: %.1f%%", cpu_utilization);
            ESP_LOGI(TAG, "Overhead: %.1f%%", overhead_percentage);
            ESP_LOGI(TAG, "Avg time per task: %lld us", context_switch_time / context_switches);
        }
    }
}
```

### Part 2: Time-Sharing with Variable Workloads (15 นาที)

สร้างเวอร์ชันที่มี workload ต่างกัน:

```c
// เพิ่มในส่วนท้ายของไฟล์ก่อนหน้า

// Variable time slice experiment
void variable_time_slice_experiment(void)
{
    ESP_LOGI(TAG, "\n=== Variable Time Slice Experiment ===");
    
    // Test different time slices
    uint32_t time_slices[] = {10, 25, 50, 100, 200};
    int num_slices = sizeof(time_slices) / sizeof(time_slices[0]);
    
    for (int i = 0; i < num_slices; i++) {
        ESP_LOGI(TAG, "Testing time slice: %d ms", time_slices[i]);
        
        // Reset counters
        context_switches = 0;
        context_switch_time = 0;
        task_counter = 0;
        
        uint64_t test_start = esp_timer_get_time();
        
        // Run for 5 seconds
        for (int j = 0; j < 50; j++) {
            manual_scheduler();
            vTaskDelay(pdMS_TO_TICKS(time_slices[i]));
        }
        
        uint64_t test_end = esp_timer_get_time();
        uint64_t test_duration = test_end - test_start;
        
        float efficiency = ((float)context_switch_time / (float)test_duration) * 100.0f;
        
        ESP_LOGI(TAG, "Time slice %d ms: Efficiency %.1f%%", time_slices[i], efficiency);
        ESP_LOGI(TAG, "Context switches: %d", context_switches);
        
        vTaskDelay(pdMS_TO_TICKS(1000)); // Pause between tests
    }
}
```

### Part 3: การวิเคราะห์ปัญหา (5 นาที)

```c
// Problem demonstration
void demonstrate_problems(void)
{
    ESP_LOGI(TAG, "\n=== Demonstrating Time-Sharing Problems ===");
    
    // Problem 1: Priority Inversion
    ESP_LOGI(TAG, "Problem 1: No priority support");
    ESP_LOGI(TAG, "Critical task must wait for less important tasks");
    
    // Problem 2: Fixed time slice issues
    ESP_LOGI(TAG, "Problem 2: Fixed time slice problems");
    ESP_LOGI(TAG, "Short tasks waste time, long tasks get interrupted");
    
    // Problem 3: Context switching overhead
    ESP_LOGI(TAG, "Problem 3: Context switching overhead");
    ESP_LOGI(TAG, "Time wasted in switching between tasks");
    
    // Problem 4: No inter-task communication
    ESP_LOGI(TAG, "Problem 4: No proper inter-task communication");
    ESP_LOGI(TAG, "Tasks cannot communicate safely");
}
```

## การทดสอบและวัดผล

### 1. การวัด CPU Utilization
- สังเกตการแสดงสถิติทุก 20 context switches
- บันทึกค่า CPU utilization และ overhead

### 2. การทดสอบ Time Slice ต่างๆ
- ทดสอบ time slice: 10ms, 25ms, 50ms, 100ms, 200ms
- เปรียบเทียบประสิทธิภาพ

### 3. การสังเกต LED Pattern
- LED1: Sensor task (งานเบา)
- LED2: Processing task (งานหนัก)
- LED3: Actuator task (งานปานกลาง)
- LED4: Display task (งานเบา)

## คำถามสำหรับวิเคราะห์

1. Time slice ขนาดไหนให้ประสิทธิภาพดีที่สุด? เพราะอะไร?
จากผลการทดลอง Time slice ที่ประมาณ 50ms – 100ms จะให้สมดุลที่ดีที่สุด เพราะ overhead ของ context switching ไม่มากเกินไป และแต่ละงานยังได้เวลาเพียงพอที่จะทำงานได้เสร็จพอดี ไม่ถูกตัดกลางคัน ทำให้ CPU utilization สูงกว่า 85–95% ขึ้นไป
2. ปัญหาอะไรที่เกิดขึ้นเมื่อ time slice สั้นเกินไป?
ถ้า time slice สั้นเกินไป (เช่น 10ms) ระบบจะต้อง context switch บ่อย → ทำให้ overhead สูงมาก (25–30%) เวลา CPU ส่วนใหญ่หมดไปกับการสลับงานแทนที่จะประมวลผลงานจริง ทำให้ประสิทธิภาพโดยรวมลดลง และ LED อาจกะพริบไม่สม่ำเสมอเพราะ task ถูกแทรกบ่อย
3. ปัญหาอะไรที่เกิดขึ้นเมื่อ time slice ยาวเกินไป?
ถ้า time slice ยาวเกินไป (เช่น 200ms) จะทำให้บางงานต้องรอนานเกินความจำเป็น → การตอบสนองช้าลง (เช่น Display task หรือ Sensor task ที่ควรอัปเดตเร็ว ๆ) และงานที่สั้นมากจะเหมือน “เสียเวลา” เพราะยังถือ CPU ไว้ทั้ง ๆ ที่งานเสร็จแล้ว ก่อให้เกิดการใช้ทรัพยากรไม่เต็มประสิทธิภาพ
4. Context switching overhead คิดเป็นกี่เปอร์เซ็นต์ของเวลาทั้งหมด?
ขึ้นอยู่กับขนาด time slice
ที่ 10ms overhead ~25–30%
ที่ 50ms overhead ~10–15%
ที่ 100ms overhead ~5–10%
สรุปคือ overhead ยิ่งสูงเมื่อ time slice สั้นลง เพราะต้องสลับงานบ่อย
5. งานไหนที่ได้รับผลกระทบมากที่สุดจากการ time-sharing?
Processing Task (งานหนัก) → ได้รับผลกระทบชัดเจน เพราะมักจะถูกตัดกลางคันถ้า time slice สั้นเกินไป ทำให้งานไม่เสร็จในรอบเดียว ต้องถูกแบ่งย่อยหลายรอบ → ประสิทธิภาพตก
งานเบา ๆ อย่าง Sensor Task หรือ Display Task ไม่ได้รับผลกระทบมากนัก แต่จะต้องรอนานถ้า time slice ยาวเกินไป

## ผลการทดลองที่คาดหวัง

| Time Slice | Context Switches/sec | CPU Utilization | Overhead |
|------------|---------------------|-----------------|----------|
| 10ms       | 100                 | 70-75%          | 25-30%   |
| 50ms       | 20                  | 85-90%          | 10-15%   |
| 100ms      | 10                  | 90-95%          | 5-10%    |

## บทสรุร

การทดลองนี้แสดงให้เห็นถึง:
1. **Context Switching Overhead** - เวลาที่สููญเสียในการเปลี่ยน task
2. **Time Slice Trade-offs** - ความสมดุลระหว่างการตอบสนองและประสิทธิภาพ
3. **Fixed Schedule Limitations** - ข้อจำกัดของการใช้เวลาคงที่
4. **Need for RTOS** - เหตุผลที่จำเป็นต้องมี Real-Time Operating System