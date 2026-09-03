#include <Arduino.h>

#define LED_PIN 39

#define UART_RX 40
#define UART_TX 41

HardwareSerial ESP1Serial(1);

void setup() {
    // LED của ESP2
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // Serial Monitor
    Serial.begin(115200);

    // UART với ESP1
    ESP1Serial.begin(
        9600,
        SERIAL_8N1,
        UART_RX,
        UART_TX
    );

    Serial.println("ESP32-S3 #2 READY");
    Serial.println("Nhap 1 = bat LED ESP1");
    Serial.println("Nhap 0 = tat LED ESP1");
}

void loop() {

    // ===============================
    // PC -> ESP2 -> ESP1
    // ===============================
    if (Serial.available()) {

        char cmd = Serial.read();

        if (cmd == '1' || cmd == '0') {

            ESP1Serial.write(cmd);

            Serial.print("ESP2 gui ESP1: ");
            Serial.println(cmd);
        }
    }

    // ===============================
    // ESP1 -> ESP2
    // ===============================
    if (ESP1Serial.available()) {

        char cmd = ESP1Serial.read();

        if (cmd == '1') {

            digitalWrite(LED_PIN, HIGH);

            Serial.println("Nhan tu ESP1: 1");
            Serial.println("LED ESP2 ON");
        }

        else if (cmd == '0') {

            digitalWrite(LED_PIN, LOW);

            Serial.println("Nhan tu ESP1: 0");
            Serial.println("LED ESP2 OFF");
        }
    }
}