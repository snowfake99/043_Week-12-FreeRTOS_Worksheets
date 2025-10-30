## คำถามสำหรับวิเคราะห์

1. Priority ไหนทำงานมากที่สุด? เพราะอะไร?
คำตอบ:
Task ที่มี Priority สูงสุด จะได้ทำงานมากที่สุด
เพราะ FreeRTOS ใช้หลักการ Preemptive Scheduling —
คือ Task ที่มี priority สูงจะ “แย่ง CPU” จาก Task ที่ priority ต่ำทันทีเมื่อพร้อม (Ready State)
ดังนั้น Task ที่ priority สูงจะได้เวลาประมวลผลมากที่สุด
สรุป:Priority สูง → ได้เวลาทำงานมากกว่า → ควบคุม CPU ได้ก่อน
2. เกิด Priority Inversion หรือไม่? จะแก้ไขได้อย่างไร?
คำตอบ:Priority Inversion เกิดขึ้นเมื่อ
Task ที่ priority สูงรอทรัพยากร (resource) ที่ถูกครอบครองโดย Task ที่ priority ต่ำกว่า
ทำให้ Task สูง “หยุดรอ” และ Task ต่ำยังทำงานไม่ได้เพราะถูก Task กลางแทรก
ตัวอย่าง:High → รอ Mutex จาก Lowแต่ Medium แย่ง CPU ทำให้ Low ปลดล็อกไม่ได้ → ระบบค้างบางส่วน
วิธีแก้:ใช้กลไก Priority Inheritance (การสืบทอดลำดับความสำคัญ)ระบบจะ “ยก Priority ของ Task ต่ำ” ให้เท่ากับ Task สูงชั่วคราว
จนกว่าจะปล่อย resource → ป้องกันการค้าง
3. Tasks ที่มี priority เดียวกันทำงานอย่างไร?
คำตอบ:Tasks ที่มี Priority เท่ากันจะ แบ่งเวลา (Time Slicing)โดย Scheduler จะสลับ Task ทุก ๆ “tick” (ตามค่า configTICK_RATE_HZ)เพื่อให้ทุก Task ได้ CPU อย่างเท่าเทียม
สรุป:Priority เท่ากัน → ทำงานแบบสลับรอบ (Round-Robin Scheduling)
4. การเปลี่ยน Priority แบบ dynamic ส่งผลอย่างไร?
คำตอบ:การเปลี่ยน Priority ระหว่างรัน (เช่นใช้ vTaskPrioritySet())
จะทำให้ Scheduler จัดลำดับใหม่ทันที
ถ้าเพิ่ม Priority → Task จะรันทันที (Preempt others)
ถ้าลด Priority → Task จะถูกเลื่อนออกจาก CPU ให้ Task อื่นทำงานแทน
สรุป:Dynamic Priority เปลี่ยนลำดับการทำงานแบบเรียลไทม์
ช่วยควบคุมการตอบสนองของระบบได้ยืดหยุ่น
5. CPU utilization ของแต่ละ priority เป็นอย่างไร?
คำตอบ:โดยทั่วไป
Task ที่ Priority สูง จะใช้ CPU มากที่สุด (ทำงานบ่อย)
Priority กลาง จะได้เวลาทำงานเฉพาะช่วงที่ไม่มี Task สูงพร้อม
Priority ต่ำ จะได้ทำงานเฉพาะเมื่อไม่มี Task อื่น ready เลย
สรุป:การใช้ CPU แปรผันตาม Priority
สูงสุด → ได้ CPU มากสุด
ต่ำสุด → ได้ CPU น้อยสุด หรือแทบไม่ได้ทำงานเลย