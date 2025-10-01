# ปฏิบัติการ: FreeRTOS Overview

## ภาพรวม
การปฏิบัติการนี้จะแนะนำการใช้งาน ESP-IDF และการสร้าง Task พื้นฐานด้วย FreeRTOS

## 📋 รายการแลป

### Lab 1: ESP-IDF Setup และโปรเจกต์แรก (45 นาที)
- **ไฟล์**: [lab1-setup-first-project/](lab1-setup-first-project/)
- **เป้าหมาย**: ติดตั้งและสร้างโปรเจกต์แรก
- **กิจกรรม**:
  - ตรวจสอบการติดตั้ง ESP-IDF
  - สร้างโปรเจกต์ใหม่ด้วย `idf.py create-project`
  - ทำความเข้าใจโครงสร้างโปรเจกต์
  - Build และ Flash โปรแกรมแรก

### Lab 2: Hello World และ Serial Communication (30 นาที)
- **ไฟล์**: [lab2-hello-world/](lab2-hello-world/)
- **เป้าหมาย**: ทำความเข้าใจ Serial Output และ Monitor
- **กิจกรรม**:
  - ใช้ `printf()` และ `ESP_LOGI()`
  - ใช้ `idf.py monitor` เพื่อดู output
  - ทดสอบ log levels ต่างๆ

### Lab 3: สร้าง Task แรกด้วย FreeRTOS (45 นาที)
- **ไฟล์**: [lab3-first-task/](lab3-first-task/)
- **เป้าหมาย**: สร้างและจัดการ Task พื้นฐาน
- **กิจกรรม**:
  - สร้าง Task ด้วย `xTaskCreate()`
  - ทำความเข้าใจ Task Function และ Parameters
  - ใช้ `vTaskDelay()` และ `vTaskDelete()`
  - สร้าง Multiple Tasks

## 🛠️ เครื่องมือที่ใช้
- ESP32 Development Board
- USB Cable
- ESP-IDF (pre-installed)
- VS Code หรือ Terminal

## 📊 การประเมินผล
- การติดตั้งและตั้งค่าเครื่องมือ (30%)
- การสร้างและ build โปรเจกต์ (40%)
- การสร้าง Task และทำความเข้าใจ API (30%)

## 📝 Checklist
- [✅ ] ตรวจสอบการติดตั้ง ESP-IDF
- [✅ ] สร้างโปรเจกต์ใหม่สำเร็จ
- [✅ ] Build และ Flash โปรแกรมสำเร็จ
- [ ✅] ดู Serial Output ได้
- [ ✅] สร้าง Task ขยั้นพื้นฐานได้
- [ ✅] ทำความเข้าใจ Task lifecycle