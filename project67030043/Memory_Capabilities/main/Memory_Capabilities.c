#include "esp_heap_caps.h"

// Memory capability flags
#define MALLOC_CAP_EXEC       (1<<0)   // Memory can execute code
#define MALLOC_CAP_32BIT      (1<<1)   // 32-bit aligned
#define MALLOC_CAP_8BIT       (1<<2)   // 8-bit access allowed  
#define MALLOC_CAP_DMA        (1<<3)   // DMA capable
#define MALLOC_CAP_PID2       (1<<4)   // PID2 memory
#define MALLOC_CAP_PID3       (1<<5)   // PID3 memory
#define MALLOC_CAP_PID4       (1<<6)   // PID4 memory
#define MALLOC_CAP_PID5       (1<<7)   // PID5 memory
#define MALLOC_CAP_PID6       (1<<8)   // PID6 memory
#define MALLOC_CAP_PID7       (1<<9)   // PID7 memory
#define MALLOC_CAP_SPIRAM     (1<<10)  // External SPIRAM
#define MALLOC_CAP_INTERNAL   (1<<11)  // Internal memory
#define MALLOC_CAP_DEFAULT    (1<<12)  // Default allocation

// Convenience combinations
#define MALLOC_CAP_IRAM_8BIT  (MALLOC_CAP_EXEC | MALLOC_CAP_8BIT)
#define MALLOC_CAP_DMA_32BIT  (MALLOC_CAP_DMA | MALLOC_CAP_32BIT)
```

### Memory Capability Example:

```c
#include "esp_heap_caps.h"
#include "esp_log.h"

static const char *TAG = "MEMORY_CAPS";

void memory_capabilities_demo(void)
{
    // Allocate DMA-capable memory
    void *dma_buffer = heap_caps_malloc(1024, MALLOC_CAP_DMA);
    if (dma_buffer) {
        ESP_LOGI(TAG, "DMA buffer allocated at %p", dma_buffer);
        heap_caps_free(dma_buffer);
    } else {
        ESP_LOGE(TAG, "Failed to allocate DMA buffer");
    }
    
    // Allocate executable memory (IRAM)
    void *exec_buffer = heap_caps_malloc(512, MALLOC_CAP_EXEC);
    if (exec_buffer) {
        ESP_LOGI(TAG, "Executable buffer allocated at %p", exec_buffer);
        heap_caps_free(exec_buffer);
    }
    
    // Allocate from external SPIRAM
    void *spiram_buffer = heap_caps_malloc(2048, MALLOC_CAP_SPIRAM);
    if (spiram_buffer) {
        ESP_LOGI(TAG, "SPIRAM buffer allocated at %p", spiram_buffer);
        heap_caps_free(spiram_buffer);
    }
    
    // Allocate 32-bit aligned memory
    void *aligned_buffer = heap_caps_aligned_alloc(32, 1024, MALLOC_CAP_32BIT);
    if (aligned_buffer) {
        ESP_LOGI(TAG, "32-bit aligned buffer allocated at %p", aligned_buffer);
        heap_caps_aligned_free(aligned_buffer);
    }
}