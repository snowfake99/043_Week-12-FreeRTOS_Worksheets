### คำถามสำหรับการทดลоง
1. เมื่อ give semaphore หลายครั้งติดต่อกัน จะเกิดอะไรขึ้น?
คำตอบ:
สำหรับ Binary Semaphore การ xSemaphoreGive() หลายครั้งติดต่อกันจะไม่มีผลสะสม
เพราะ Binary Semaphore มีเพียง “1 สิทธิ์” เท่านั้น (เหมือนสวิตช์เปิด–ปิด)
หาก semaphore ถูก give แล้วโดยที่ยังไม่ถูก take → ค่าจะยังคงเป็น 1
การ give เพิ่มอีกจะไม่เพิ่มจำนวนสิทธิ์
สรุป:
Binary Semaphore ไม่สามารถสะสมค่าได้ — give ซ้ำจะไม่มีผลถ้ายังไม่ได้ take
2. ISR สามารถใช้ xSemaphoreGive หรือต้องใช้ xSemaphoreGiveFromISR?
คำตอบ:
ISR (Interrupt Service Routine) ต้องใช้ฟังก์ชัน xSemaphoreGiveFromISR() เท่านั้น
เพราะ FreeRTOS แยกการทำงานของ ISR ออกจาก task ปกติ
การใช้ฟังก์ชันปกติอย่าง xSemaphoreGive() ภายใน ISR จะทำให้ระบบไม่ปลอดภัย และอาจเกิด crash
สรุป:
ใน ISR → ต้องใช้ xSemaphoreGiveFromISR() เพื่อความถูกต้องและปลอดภัยของระบบ
3. Binary Semaphore แตกต่างจาก Queue อย่างไร?
คำตอบ:
รายการ	Binary Semaphore	Queue
หน้าที่หลัก	ซิงโครไนซ์ (synchronization) ระหว่าง Task หรือ ISR	ส่งข้อมูล (communication) ระหว่าง Task
ขนาดข้อมูล	ไม่มีข้อมูลจริง มีแค่สถานะ (ให้ / ยังไม่ให้)	สามารถส่งข้อมูลจริง เช่น ตัวเลขหรือ struct
ปริมาณที่เก็บได้	เก็บได้สูงสุด 1 สิทธิ์ (binary)	เก็บได้หลายรายการตาม Queue length
การใช้งานทั่วไป	ป้องกัน race condition, แจ้งเหตุการณ์	ส่งข้อมูลหรือข้อความระหว่าง Task
สรุป:
Semaphore ใช้ “ซิงโครไนซ์เหตุการณ์” ส่วน Queue ใช้ “ส่งข้อมูล” ระหว่าง Task