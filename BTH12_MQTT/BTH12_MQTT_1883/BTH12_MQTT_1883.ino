#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

/* ====================================================================
   1. THÔNG TIN CẤU HÌNH THIẾT BỊ VÀ MẠNG
   ==================================================================== */
const char* device_name  = "ESP32_Device";
const char* ssid         = "Thân chưa mà xin";
const char* password     = "modicungg";

// Thông tin MQTT Broker (Home Assistant) - Dùng cổng chuẩn 1883
const char* mqtt_server  = "172.20.10.5"; 
const int   mqtt_port    = 1883;                  // Cổng MQTT thường (không mã hóa)
const char* mqtt_user    = "mqtt_user";           // Mã đăng nhập đã tạo trên HA
const char* mqtt_pass    = "Mt@472005";           // Mật khẩu tương ứng
const char* student_id   = "10124332";
const char* mqtt_topic   = "iot/lab1/10124332/sensor";

/* ====================================================================
   2. KHAI BÁO BIẾN TOÀN CỤC & ĐỐI TƯỢNG KẾT NỐI
   ==================================================================== */
WiFiClient espClient; // Sử dụng WiFiClient thông thường thay vì WiFiClientSecure
PubSubClient client(espClient);

unsigned long lastMsg = 0;

/* ====================================================================
   3. HÀM KẾT NỐI WIFI
   ==================================================================== */
void setup_wifi() {
  delay(10);
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

/* ====================================================================
   4. HÀM TỰ ĐỘNG TẠO LẠI KẾT NỐI MQTT NẾU BỊ MẤT MẠNG
   ==================================================================== */
void reconnect() {
  while (!client.connected()) {
    String clientId = device_name;
    clientId += "_";
    clientId += student_id;
    
    Serial.print("Attempting MQTT connection to ");
    Serial.print(mqtt_server);
    Serial.print(":");
    Serial.println(mqtt_port);

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println(">> MQTT CONNECTED SUCCESSFULLY! <<");
    } else {
      Serial.print(">> Failed to connect, rc=");
      Serial.print(client.state());
      Serial.println(". Try again in 2 seconds...");
      delay(2000);
    }
  }
}

/* ====================================================================
   5. HÀM KHỞI TẠO (SETUP)
   ==================================================================== */
void setup() {
  Serial.begin(115200);
  delay(100);

  // Kết nối WiFi
  setup_wifi();

  // Cấu hình Server & Cổng kết nối 1883
  client.setServer(mqtt_server, mqtt_port);
}

/* ====================================================================
   6. VÒNG LẶP CHÍNH (LOOP)
   ==================================================================== */
void loop() {
  // Kiểm tra duy trì kết nối MQTT
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Gửi dữ liệu định kỳ mỗi 5 giây một lần (dùng millis thay vì delay để không block loop)
  unsigned long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;

    // Đóng gói dữ liệu dạng JSON
    StaticJsonDocument<200> doc;
    doc["temp"] = random(250, 350) / 10.0; // Giả lập nhiệt độ (25.0 - 35.0 C)
    doc["hum"]  = random(400, 800) / 10.0; // Giả lập độ ẩm (40.0 - 80.0 %)

    char payload[128];
    serializeJson(doc, payload);

    // Gửi dữ liệu lên MQTT Broker
    if (client.publish(mqtt_topic, payload)) {
      Serial.print("Published payload: ");
      Serial.println(payload);
    } else {
      Serial.println("Publish failed!");
    }
  }
}