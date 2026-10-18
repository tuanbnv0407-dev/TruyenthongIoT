#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#define LED_PIN 48

#define SERVICE_UUID \
  "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"

#define WRITE_UUID \
  "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"

class LedCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *characteristic) override {
    auto value = characteristic->getValue();

    if (value.length() == 0) return;

    if (value[0] == '1') {
      rgbLedWrite(LED_PIN, 0, 30, 0);  // Bật màu xanh lá
      Serial.println("LED ON");
    }
    else if (value[0] == '0') {
      rgbLedWrite(LED_PIN, 0, 0, 0);   // Tắt LED
      Serial.println("LED OFF");
    }
  }
};

void setup() {
  Serial.begin(115200);
  rgbLedWrite(LED_PIN, 0, 0, 0);

  BLEDevice::init("MINH TUAN");

  BLEServer *server = BLEDevice::createServer();
  BLEService *service = server->createService(SERVICE_UUID);

  BLECharacteristic *command = service->createCharacteristic(
    WRITE_UUID,
    BLECharacteristic::PROPERTY_WRITE
  );

  command->setCallbacks(new LedCallbacks());
  service->start();

  BLEAdvertising *advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(SERVICE_UUID);
  advertising->setScanResponse(true);
  advertising->start();

  Serial.println("San sang! Ket noi ESP32S3_LED");
}

void loop() {
  delay(100);
}