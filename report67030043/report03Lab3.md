### คำถามสำหรับการทดลอง
1. Processor Task รู้ได้อย่างไรว่าข้อมูลมาจาก Queue ไหน?
คำตอบ:
เมื่อใช้ Queue Set, ฟังก์ชัน xQueueSelectFromSet() จะคืนค่า “handle” ของ Queue ที่มีข้อมูลพร้อมใช้งาน
Processor Task สามารถตรวจสอบได้โดยการเปรียบเทียบ handle ที่คืนค่ามากับ Queue ที่สร้างไว้ เช่น
if (xActivatedQueue == xQueue1) { ... }
else if (xActivatedQueue == xQueue2) { ... }
สรุป:
Processor Task ทราบว่า ข้อมูลมาจาก Queue ไหน โดยใช้ handle ที่ได้จาก xQueueSelectFromSet()
2. เมื่อหลาย Queue มีข้อมูลพร้อมกัน เลือกประมวลผลอันไหนก่อน?
คำตอบ:
ถ้ามีหลาย Queue ภายใน Queue Set มีข้อมูลพร้อมในเวลาเดียวกัน
FreeRTOS จะ เลือกตามลำดับที่ถูกเพิ่มเข้าใน Queue Set ก่อน (first-added order)
หรือพูดง่าย ๆ ว่า Queue ที่ถูกเพิ่มเข้าชุดก่อนจะถูกเลือกตรวจสอบก่อน
สรุป:
Queue ที่อยู่ลำดับแรกใน Queue Set จะถูกเลือกก่อน หากหลาย Queue พร้อมกัน
3. Queue Sets ช่วยประหยัด CPU อย่างไร?
คำตอบ:
โดยปกติ ถ้ามีหลาย Queue ต้องตรวจสอบทีละอันว่า “มีข้อมูลไหม”
แต่ Queue Set ช่วยให้ Task สามารถ รอหลาย Queue พร้อมกันได้ภายในคำสั่งเดียว (xQueueSelectFromSet())
ทำให้ไม่ต้อง loop เช็กหลายครั้ง ลด context switching และลดการใช้ CPU
สรุป:
Queue Set รวมหลาย Queue ให้ตรวจพร้อมกันในครั้งเดียว → ประหยัดเวลาและ CPU utilization