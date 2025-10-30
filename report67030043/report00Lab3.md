## คำถามสำหรับวิเคราะห์

1. ระบบไหนมีเวลาตอบสนองดีกว่า? เพราะอะไร?
Preemptive Multitasking มีเวลาตอบสนองที่ดีกว่า เนื่องจาก RTOS สามารถแยกงานและบังคับสลับไปทำ task ที่สำคัญกว่า (เช่น Emergency task) ได้ทันที แม้ task อื่นจะกำลังทำงานหนักอยู่ ต่างจาก Cooperative ที่ต้องรอ task ปัจจุบัน “ยอมปล่อย” CPU ก่อน เวลาตอบสนองจึงไม่แน่นอนและอาจช้ามากหาก task ใด task หนึ่งไม่ยอม yield
2. ข้อดีของ Cooperative Multitasking คืออะไร?
โครงสร้างเรียบง่ายและเข้าใจง่าย
Resource ใช้น้อยกว่า (stack memory และ scheduling overhead ต่ำกว่า)
Debug ง่ายเพราะ flow ของการทำงานคาดเดาได้
เหมาะกับงานที่ไม่ต้องการ real-time เข้มงวดและ task ต่าง ๆ มีความร่วมมือกันดี (ทุก task ยอม yield)
3. ข้อเสียของ Cooperative Multitasking คืออะไร?
เวลาตอบสนองไม่แน่นอน หาก task หนึ่งไม่ yield จะทำให้ทั้งระบบช้า
ไม่เหมาะกับ real-time systems ที่ต้องการ response time เร็วและคงที่
ต้องออกแบบให้ programmer ใส่ yield (vTaskDelay() หรือ return) เอง หากพลาดอาจทำให้ระบบค้างหรือ delay นาน
4. ในสถานการณ์ใดที่ Cooperative จะดีกว่า Preemptive?
ระบบเล็ก ๆ ที่ resource จำกัดมาก เช่น microcontroller ขนาดเล็กที่ไม่มี OS เต็มรูปแบบ
แอปพลิเคชันที่ไม่มีความต้องการ real-time สูง เช่น demo, งานทดลอง, งานควบคุมที่ความเร็วไม่สำคัญ
การพัฒนาที่เน้นความง่ายและต้องการ debug ได้สะดวก
5. เหตุใด Preemptive จึงเหมาะสำหรับ Real-time systems?
เพราะ Preemptive มี determinism คือกำหนดพฤติกรรมเวลาตอบสนองได้แน่นอน Emergency task ที่มี priority สูงจะถูกสลับขึ้นมาทำงานทันทีแม้ระบบกำลัง busy อยู่ → ทำให้มั่นใจได้ว่างานที่ critical (เช่น safety, emergency, real-time control) จะได้รับการตอบสนองตรงเวลาเสมอ