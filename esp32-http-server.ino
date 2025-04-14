#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

// Pins and sensors
#define gasSensorPin 34
#define DHTPIN 2
#define DHTTYPE DHT22

// LCD and DHT setup
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

// Wi-Fi credentials (Wokwi auto-connects to Wokwi-GUEST)
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Gas threshold
const int gasThreshold = 600;

void setup() {
  Serial.begin(115200);

  // Init LCD
  lcd.init();
  lcd.backlight();

  // Init DHT
  dht.begin();

  // Connect to WiFi
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");
  WiFi.begin(ssid, password);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    lcd.print(".");
    Serial.print(".");
    retries++;
  }

  lcd.clear();
  if (WiFi.status() == WL_CONNECTED) {
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connected");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());
  } else {
    lcd.setCursor(0, 0);
    lcd.print("WiFi Failed :(");
  }

  delay(3000);
  lcd.clear();
}

void loop() {
  int gasLevel = analogRead(gasSensorPin);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Print to serial monitor
  Serial.print("Gas: ");
  Serial.print(gasLevel);
  Serial.print(" | Temp: ");
  Serial.print(temperature);
  Serial.print(" C | Hum: ");
  Serial.println(humidity);

  // Display on LCD
  lcd.setCursor(0, 0);
  lcd.print("G:");
  lcd.print(gasLevel);
  lcd.print(" T:");
  lcd.print(isnan(temperature) ? 0.0 : temperature, 1);

  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(isnan(humidity) ? 0.0 : humidity, 1);
  lcd.print("%");

  delay(2000);
}
