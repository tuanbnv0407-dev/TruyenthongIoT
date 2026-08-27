#include <Servo.h>

Servo myservo;
int potPin = A0; // Chân đọc biến trở
int val;         // Biến lưu giá trị đọc được

void setup() {
  myservo.attach(9); // Chân phát tín hiệu Servo
}

void loop() {
  val = analogRead(potPin);            // Đọc giá trị từ A0 (0 - 1023)
  val = map(val, 0, 1023, 0, 180);     // Chuyển đổi sang dải góc (0 - 180 độ)
  myservo.write(val);                  // Gán góc cho Servo
  delay(15);                           // Chờ Servo di chuyển
}