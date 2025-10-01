# Lab 1: ESP-IDF Setup และโปรเจกต์แรก

## วัตถุประสงค์
ติดตั้งและตรวจสอบการทำงานของ ESP-IDF พร้อมสร้างโปรเจกต์แรก

## เวลาที่ใช้
45 นาที

## ข้อกำหนดเบื้องต้น
- ESP32 Development Board
- USB Cable
- คอมพิวเตอร์ที่ติดตั้ง ESP-IDF แล้ว

## ขั้นตอนการทดลอง

### Step 1: ตรวจสอบการติดตั้ง ESP-IDF (10 นาที)

```bash
# 1. ตรวจสอบ ESP-IDF environment
echo $IDF_PATH

# 2. ตรวจสอบเวอร์ชัน
idf.py --version

# 3. ตรวจสอบ Python และ pip
python --version
pip --version

# 4. ตรวจสอบ toolchain
xtensa-esp32-elf-gcc --version
```

**ผลลัพธ์ที่คาดหวัง:**
```
ESP-IDF v5.x
Python 3.x.x
xtensa-esp32-elf-gcc version x.x.x
```

### Step 2: สร้างโปรเจกต์แรก (15 นาที)

```bash
# 1. สร้างโปรเจกต์ใหม่
idf.py create-project hello_esp32
cd hello_esp32

# 2. ดูโครงสร้างโปรเจกต์
ls -la
```

**โครงสร้างโปรเจกต์:**
```
hello_esp32/
├── CMakeLists.txt          # Build configuration
├── main/
│   ├── CMakeLists.txt      # Main component
│   └── hello_esp32.c       # Main source file
├── README.md               # Project documentation
└── sdkconfig               # Project configuration (after build)
```

### Step 3: ศึกษาไฟล์หลัก (10 นาที)

**ไฟล์ `main/hello_esp32.c`:**
```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    printf("Hello, ESP32 World!\n");
    
    // Keep the program running
    while(1) {
        printf("ESP32 is running...\n");
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 second delay
    }
}
```

**ไฟล์ `CMakeLists.txt` (root):**
```cmake
cmake_minimum_required(VERSION 3.16)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(hello_esp32)
```

**ไฟล์ `main/CMakeLists.txt`:**
```cmake
idf_component_register(SRCS "hello_esp32.c"
                       INCLUDE_DIRS ".")
```

### Step 4: Build โปรเจกต์ (10 นาที)

```bash
# 1. กำหนด target chip
idf.py set-target esp32

# 2. Build โปรเจกต์
idf.py build
```

**ข้อความที่ควรเห็น:**
```
Project build complete. To flash, run:
  idf.py flash
or
  idf.py -p (PORT) flash
```

**ไฟล์ที่ถูกสร้าง:**
```
build/
├── bootloader/
├── partition_table/
├── hello_esp32.bin         # Main application binary
├── hello_esp32.elf         # ELF file with debug info
└── flash_args              # Flash arguments
```

## การแก้ไขปัญหาที่อาจเกิดขึ้น

### ปัญหา 1: ESP-IDF not found
```bash
# แก้ไข: Set up environment
source $HOME/esp/esp-idf/export.sh
# หรือ (Windows)
%USERPROFILE%\esp\esp-idf\export.bat
```

### ปัญหา 2: Permission denied
```bash
# Linux/macOS
sudo chmod 666 /dev/ttyUSB0
# หรือเพิ่ม user ใน dialout group
sudo usermod -a -G dialout $USER
```

### ปัญหา 3: Target not set
```bash
idf.py set-target esp32
```

## แบบฝึกหัด

### Exercise 1: แก้ไขข้อความ
แก้ไขไฟล์ `main/hello_esp32.c` ให้แสดงข้อความที่แตกต่าง:

```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

void app_main(void)
{
    printf("=== My First ESP32 Project ===\n");
    printf("Chip model: %s\n", esp_get_idf_version());
    printf("Free heap: %d bytes\n", esp_get_free_heap_size());
    
    int counter = 0;
    while(1) {
        printf("Running for %d seconds\n", counter++);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

### Exercise 2: เพิ่ม Build Information
สร้างไฟล์ `main/build_info.h`:

```c
#ifndef BUILD_INFO_H
#define BUILD_INFO_H

#define PROJECT_NAME "Hello ESP32"
#define PROJECT_VERSION "1.0.0"
#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__

#endif
```

แก้ไข `main/hello_esp32.c`:

```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "build_info.h"

void app_main(void)
{
    printf("=== %s v%s ===\n", PROJECT_NAME, PROJECT_VERSION);
    printf("Built on: %s %s\n", BUILD_DATE, BUILD_TIME);
    printf("ESP-IDF Version: %s\n", esp_get_idf_version());
    printf("Chip: %s\n", CONFIG_IDF_TARGET);
    printf("Free heap: %d bytes\n", esp_get_free_heap_size());
    
    while(1) {
        printf("System is running...\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
```

### Exercise 3: การ Build แบบ Verbose
```bash
# Build with verbose output
idf.py -v build

# Clean และ build ใหม่
idf.py clean
idf.py build
```

## Checklist การทำงาน

- [✅ ] ตรวจสอบ ESP-IDF environment สำเร็จ
- [ ✅] สร้างโปรเจกต์ใหม่ได้
- [ ✅] เข้าใจโครงสร้างโปรเจกต์
- [✅ ] Build โปรเจกต์สำเร็จ
- [✅ ] แก้ไขโค้ดและ build ใหม่ได้
- [✅ ] เข้าใจไฟล์ CMakeLists.txt
- [✅ ] ทำแบบฝึกหัดครบ

## คำถามทบทวหน

1. ไฟล์ใดบ้างที่จำเป็นสำหรับโปรเจกต์ ESP-IDF ขั้นต่ำ?
โปรเจกต์ ESP-IDF ขั้นต่ำต้องมีไฟล์สำคัญคือ CMakeLists.txt ที่ root โปรเจกต์, โฟลเดอร์ main/ ซึ่งมี main.c (โค้ดหลัก) และ CMakeLists.txt ของโฟลเดอร์นั้น, และไฟล์ sdkconfig ที่ได้จาก idf.py menuconfig เพื่อเก็บค่าการตั้งค่าของโปรเจกต์ ถือเป็นชุดไฟล์ขั้นต่ำที่ ESP-IDF ต้องใช้ในการ build และ run บน ESP32
2. ความแตกต่างระหว่าง `hello_esp32.bin` และ `hello_esp32.elf` คืออะไร?
ไฟล์ .elf เป็น Executable and Linkable Format ที่มี symbol และ debug info ใช้สำหรับการดีบักและตรวจสอบ แต่ไม่ได้ถูกแฟลชลงชิปโดยตรง ขณะที่ไฟล์ .bin เป็น binary ที่ถูกแปลงและตัดข้อมูล debug ทิ้งออก เหลือเฉพาะโค้ดที่จำเป็นสำหรับรันบน ESP32 และถูกแฟลชลงชิปจริง ๆ
3. คำสั่ง `idf.py set-target` ทำอะไร?
คำสั่งนี้ใช้กำหนดเป้าหมายของชิปที่จะ build เช่น esp32, esp32c3, esp32s3 โดยจะไปแก้ไฟล์ sdkconfig ให้สอดคล้องกับ target ที่เลือก เพื่อให้โค้ดและไลบรารีที่ build มาเข้ากับรุ่นของชิปที่ใช้งานจริง หากไม่ตั้งค่าอาจทำให้ build ผิดหรือแฟลชไม่ตรงกับฮาร์ดแวร์
4. โฟลเดอร์ `build/` มีไฟล์อะไรบ้าง?
เมื่อสั่ง idf.py build ระบบจะสร้างโฟลเดอร์ build/ ขึ้นมา ในนี้จะมีไฟล์สำคัญ เช่น hello_esp32.bin (ไฟล์หลักที่จะรัน), bootloader.bin (ตัวบูตโหลดเดอร์), partition-table.bin (ตารางพาร์ทิชันบน Flash), และ hello_esp32.elf (สำหรับ debug) รวมถึงไฟล์ config, log และ intermediate files ที่ใช้ในกระบวนการคอมไพล์
5. การใช้ `vTaskDelay()` แทน `delay()` มีความสำคัญอย่างไร?
delay() เป็น busy wait ที่ทำให้ CPU ต้องวนรอเฉย ๆ ส่งผลให้เสียทรัพยากรและไม่สามารถทำงานอื่นได้ แต่ vTaskDelay() เป็นฟังก์ชันของ FreeRTOS ที่ทำให้ Task ปล่อย CPU ออกไป ให้ scheduler เลือก task อื่นมาทำงานต่อได้ ช่วยให้ระบบ multitasking ทำงานจริง ประหยัดพลังงาน และใช้ทรัพยากร CPU อย่างมีประสิทธิภาพมากกว่า

## บทสรุป

ในแลปนี้คุณได้เรียนรู้:
- การตรวจสอบ ESP-IDF environment
- การสร้างโปรเจกต์ ESP-IDF
- โครงสร้างโปรเจกต์และไฟล์สำคัญ
- กระบวนการ build
- การแก้ไขปัญหาเบื้องต้น

**ขั้นตอนต่อไป:** Lab 2 - Hello World และ Serial Communication