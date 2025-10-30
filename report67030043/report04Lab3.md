### คำถามสำหรับการทดลอง
1. เมื่อ Producers มากกว่า Resources จะเกิดอะไรขึ้น?
คำตอบ:
เมื่อจำนวน Producers (ผู้ต้องการใช้ทรัพยากร) มากกว่าจำนวน Resources (จำนวนสิทธิ์ใน Counting Semaphore)
Producers ที่เหลือจะต้อง รอ (Blocked state) จนกว่า Resource จะถูกปล่อยคืน (xSemaphoreGive())
ระบบจึงจะอนุญาตให้ Producer ถัดไปเข้าถึง Resource ได้
สรุป:
เมื่อ Producers มากกว่า Resources → เกิดการรอคิว (Blocking) เพื่อใช้ทรัพยากรที่จำกัด
2. Load Generator มีผลต่อ Success Rate อย่างไร?
คำตอบ:
Load Generator คือ Task ที่สร้างภาระ (โหลด) ให้ระบบโดยการเพิ่มความถี่ของการขอใช้ Resource
หากโหลดมากเกินไป จะทำให้ Resource ถูกใช้เต็มและ Success Rate ในการได้สิทธิ์ (take semaphore) ลดลง
แต่ถ้าโหลดพอเหมาะ ระบบจะทำงานได้ราบรื่นและ Success Rate สูงขึ้น
สรุป:
โหลดมาก → Success Rate ลดลง
โหลดเหมาะสม → ระบบเสถียรและ Success Rate สูง
3. Counting Semaphore จัดการ Resource Pool อย่างไร?
คำตอบ:
Counting Semaphore ใช้ตัวนับ (counter) เพื่อบันทึกจำนวน Resource ที่ว่างอยู่
เมื่อ Task ขอใช้ Resource (xSemaphoreTake) → counter จะลดลง
เมื่อ Task คืน Resource (xSemaphoreGive) → counter จะเพิ่มขึ้น
ระบบจะไม่อนุญาตให้ใช้เกินจำนวน Resource ที่มีอยู่จริง
สรุป:
Counting Semaphore = ตัวจัดการ Resource Pool แบบนับจำนวนสิทธิ์ที่เหลืออยู่ในระบบ