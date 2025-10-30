## คำถามทบทวน

1. เหตุใด Task function ต้องมี infinite loop?
เพราะ Task ใน FreeRTOS ถูกออกแบบให้ทำงานต่อเนื่องจนกว่าจะถูกลบ (vTaskDelete()) หรือระบบหยุดทำงาน
หากไม่มี while(1) ฟังก์ชันจะสิ้นสุด (return) และ Task นั้นจะหายไปจาก Scheduler ทำให้ไม่สามารถกลับมาทำงานได้อีก
สรุป: while(1) ทำให้ Task อยู่ในระบบและรันต่อเนื่องตลอดเวลา
2. ความหมายของ stack size ใน xTaskCreate() คืออะไร?
Stack size คือขนาดหน่วยความจำ (RAM) ที่ Task แต่ละตัวใช้เก็บข้อมูลชั่วคราว เช่น ตัวแปรภายใน, return address และ context เมื่อมีการสลับ Task
หน่วยวัดเป็น “word” (ไม่ใช่ byte) โดยใน ESP32, 1 word = 4 bytes
เช่น 2048 หมายถึงใช้พื้นที่ stack ขนาด 8 KB
สรุป: Stack size คือพื้นที่หน่วยความจำที่ Task ใช้ระหว่างการทำงาน
3. ความแตกต่างระหว่าง vTaskDelay() และ vTaskDelayUntil()
ฟังก์ชัน	ลักษณะการหน่วงเวลา	เหมาะสำหรับ
vTaskDelay(tick)	หน่วงเวลาแบบสัมพัทธ์ (relative delay) – นับเวลาต่อจากเวลาที่เรียกใช้	งานทั่วไป
vTaskDelayUntil(&lastWakeTime, tick)	หน่วงเวลาแบบสัมบูรณ์ (absolute delay) – รักษาคาบเวลาเท่าเดิมทุกครั้ง	งานแบบคาบเวลา (periodic task)
สรุป: ถ้าต้องการให้ Task ทำงานทุก ๆ 100 ms อย่างสม่ำเสมอ ควรใช้ vTaskDelayUntil()
4. การใช้ vTaskDelete(NULL) vs vTaskDelete(handle) ต่างกันอย่างไร?
คำสั่ง	ลบ Task ใด	ใช้เมื่อ
vTaskDelete(NULL)	ลบ Task ตัวเอง	Task ทำงานเสร็จและต้องการหยุดตัวเอง
vTaskDelete(handle)	ลบ Task อื่นที่มี handle	ต้องการให้ Task หนึ่งควบคุมการลบอีก Task หนึ่ง
ตัวอย่าง:
TaskHandle_t taskAHandle;
xTaskCreate(taskA, "TaskA", 2048, NULL, 1, &taskAHandle);
vTaskDelete(taskAHandle);  // ลบ Task A จาก Task อื่น
5. Priority 0 กับ Priority 24 อันไหนสูงกว่า?
Priority 24 สูงกว่า Priority 0
ใน FreeRTOS ค่ายิ่งมาก แสดงว่ามีความสำคัญสูงกว่า
Task ที่มี Priority สูงจะได้รับ CPU ก่อนเมื่อพร้อมทำงาน