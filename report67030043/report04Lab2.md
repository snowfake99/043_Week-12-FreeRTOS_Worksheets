### คำถามสำหรับการทดลอง
1. เมื่อไม่ใช้ Mutex จะเกิด data corruption หรือไม่?
คำตอบ:
มีโอกาสเกิด data corruption ได้แน่นอน 
เพราะหลาย Task อาจเข้าถึงและแก้ไขตัวแปรหรือทรัพยากรร่วม (shared resource) พร้อมกัน
ทำให้ค่าที่เขียน/อ่านไม่ถูกต้อง หรือข้อมูลบางส่วนถูกเขียนทับกัน
สรุป:
ไม่ใช้ Mutex → เสี่ยงเกิดการเขียนข้อมูลพร้อมกัน → ข้อมูลเพี้ยน (data corruption)
2. Priority Inheritance ทำงานอย่างไร?
คำตอบ:
Priority Inheritance คือกลไกที่ช่วยแก้ปัญหา Priority Inversion
เมื่อ Task ที่มี priority ต่ำครอบครอง Mutex ที่ Task priority สูงต้องใช้
ระบบจะ “ยก priority” ของ Task ต่ำให้เท่ากับ Task สูงชั่วคราว
จนกว่า Task ต่ำจะปล่อย Mutex → จากนั้น priority จะกลับคืนค่าเดิม
สรุป:
Priority Inheritance = ยก priority ชั่วคราวของ Task ที่ถือ Mutex เพื่อป้องกันการแย่ง CPU โดยไม่จำเป็น
3. Task priority มีผลต่อการเข้าถึง shared resource อย่างไร?
คำตอบ:
Task ที่มี priority สูงจะได้รับ CPU ก่อน ทำให้มีโอกาสเข้าถึง resource ได้เร็วกว่า
แต่ถ้าไม่มี Mutex ป้องกัน → อาจเกิด race condition เพราะ Task ต่ำอาจเขียนข้อมูลระหว่าง Task สูงกำลังใช้งานอยู่
ดังนั้นควรใช้ Mutex เพื่อให้การเข้าถึง resource เป็นแบบ “ทีละหนึ่ง” เท่านั้น
สรุป:
Priority สูง → ได้สิทธิ์ทำงานก่อน
แต่ต้องใช้ Mutex เพื่อควบคุมลำดับการเข้าถึง resource ให้ปลอดภัย