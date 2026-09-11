#include <Arduino.h>

// =============================
// KHAI BAO CHAN
// =============================
#define LED_PIN 39

#define UART_RX 40
#define UART_TX 41

// UART2
HardwareSerial MyUART(2);

// Baud UART hien tai
uint32_t currentBaud = 9600;


// =============================
// KHOI TAO UART
// =============================
void startUART(uint32_t baud)
{
    MyUART.end();
    delay(50);

    MyUART.begin(
        baud,
        SERIAL_8N1,
        UART_RX,
        UART_TX
    );

    currentBaud = baud;
}


// =============================
// NHAP NHAY LED
// =============================
void blinkLED(int times, int delayTime)
{
    for (int i = 0; i < times; i++)
    {
        digitalWrite(LED_PIN, HIGH);
        delay(delayTime);

        digitalWrite(LED_PIN, LOW);
        delay(delayTime);
    }
}


// =============================
// LED BAO HIEU VAO CAU HINH
// =============================
void configModeLED()
{
    // Nhap nhay nhanh 5 lan
    blinkLED(5, 80);
}


// =============================
// SETUP
// =============================
void setup()
{
    // Serial USB de xem tren Serial Monitor
    Serial.begin(115200);

    // Cho USB CDC khoi dong
    delay(1000);

    // LED
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // Khoi tao UART
    startUART(9600);

    Serial.println();
    Serial.println("======================================");
    Serial.println("        ESP32-S3 BEN PHAI");
    Serial.println("          UART + LED");
    Serial.println("======================================");

    Serial.println("GPIO LED : 39");
    Serial.println("GPIO RX  : 40");
    Serial.println("GPIO TX  : 41");

    Serial.print("UART Baud: ");
    Serial.println(currentBaud);

    Serial.println("--------------------------------------");
    Serial.println("Dang cho lenh tu ESP32 ben trai...");
    Serial.println();
}


// =============================
// LOOP
// =============================
void loop()
{
    // Co du lieu tu ESP32 ben trai
    if (MyUART.available())
    {
        String command = MyUART.readStringUntil('\n');

        command.trim();

        Serial.print("[UART] Nhan: ");
        Serial.println(command);


        // ==================================
        // LENH CONFIG
        // ==================================
        if (command == "CONFIG")
        {
            Serial.println(">> VAO CHE DO CAU HINH");

            configModeLED();

            Serial.println(">> Cho lua chon Baud Rate...");
        }


        // ==================================
        // LENH CHON 9600
        // ==================================
        else if (command == "BAUD9600")
        {
            Serial.println(">> Nhan yeu cau: 9600 bps");

            // LED nhap nhay 1 lan
            blinkLED(1, 200);

            Serial.println(">> LED bao hieu: 1 lan");

            delay(100);

            // Chuyen UART sang 9600
            startUART(9600);

            Serial.println(">> UART da cau hinh: 9600 bps");
        }


        // ==================================
        // LENH CHON 115200
        // ==================================
        else if (command == "BAUD115200")
        {
            Serial.println(">> Nhan yeu cau: 115200 bps");

            // LED nhap nhay 2 lan
            blinkLED(2, 200);

            Serial.println(">> LED bao hieu: 2 lan");

            delay(100);

            // Chuyen UART sang 115200
            startUART(115200);

            Serial.println(">> UART da cau hinh: 115200 bps");
        }


        // ==================================
        // LENH TRO VE VAN HANH
        // ==================================
        else if (command == "OPERATION")
        {
            Serial.println(">> TRO VE CHE DO VAN HANH");

            // Bao hieu 3 lan
            blinkLED(3, 100);

            Serial.println(">> Che do VAN HANH");
            Serial.print(">> UART Baud: ");
            Serial.println(currentBaud);
        }


        // ==================================
        // LENH KHONG HOP LE
        // ==================================
        else
        {
            Serial.println(">> Lenh khong hop le!");
        }
    }
}