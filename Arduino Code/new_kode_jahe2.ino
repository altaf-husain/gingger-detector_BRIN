#include <Jahe_kering.h>
#include <Wire.h>
#include <SparkFun_AS7265X.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Random Forest tunggal
Eloquent::ML::Port::RandomForestRegressor rf;

// Sensor & OLED
AS7265X sensor;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Tombol scan
#define BTN1 3
bool lastButtonState = HIGH;
bool buttonState = HIGH;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

float features[18];

#define DEBOUNCE_MS 200
unsigned long lastPress = 0;

bool tombolDitekan() {

  bool reading = digitalRead(BTN1);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        return true;
      }
    }
  }

  lastButtonState = reading;

  return false;
}

void setup() {

  Serial.begin(115200);

  delay(2000);

  Wire.begin();

  pinMode(BTN1, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);

  displayCenter("Sensor Jahe", 10, 1);
  displayCenter("Random Forest", 28, 1);
  displayCenter("Tekan BTN1", 44, 1);

  display.display();

  if (!sensor.begin()) {

    display.clearDisplay();
    displayCenter("AS7265X ERROR", 25, 1);
    display.display();

    while (1);
  }

  sensor.setGain(64);
  sensor.setIntegrationCycles(50);

  Serial.println("Sistem siap");
}

void loop() {

  static bool tombolSebelumnya = HIGH;

  bool tombolSekarang = digitalRead(BTN1);

  if (tombolSebelumnya == HIGH && tombolSekarang == LOW) {

    delay(30);

    if (digitalRead(BTN1) == LOW) {

      Serial.println("Tombol terdeteksi");

      showScanning();

      readSpectral();
      float x[18];

      for (int i = 0; i < 18; i++) {
      x[i] = features[i];
    }

      float hasil = rf.predict(x);

      tampilkan("Prediksi", hasil);

      while (digitalRead(BTN1) == LOW) {
        delay(10);
      }
    }
  }

  tombolSebelumnya = tombolSekarang;
}

void readSpectral() {

  // ==========================
  // UV → hanya 410 nm (A)
  // ==========================
  sensor.enableBulb(AS7265x_LED_UV);
  delay(3000);
  sensor.takeMeasurements();

  features[0] = sensor.getCalibratedA(); //410

  sensor.disableBulb(AS7265x_LED_UV);

  Serial.print("UV 410 nm : ");
  Serial.println(features[0], 6);

  // ==========================
  // VIS → 435 - 680 nm
  // B,C,D,E,F,G,H,I,J,K
  // ==========================
  sensor.enableBulb(AS7265x_LED_WHITE);
delay(3000);
sensor.takeMeasurements();

features[1]  = sensor.getCalibratedB(); //435
features[2]  = sensor.getCalibratedC(); //460
features[3]  = sensor.getCalibratedD(); //485
features[4]  = sensor.getCalibratedE(); //510
features[5]  = sensor.getCalibratedF(); //535
features[6]  = sensor.getCalibratedG(); //560
features[7]  = sensor.getCalibratedH(); //585
features[8]  = sensor.getCalibratedI(); //610
features[9]  = sensor.getCalibratedJ(); //645
features[10] = sensor.getCalibratedK(); //680

  Serial.print("435 nm : "); Serial.println(features[1], 6);
  Serial.print("460 nm : "); Serial.println(features[2], 6);
  Serial.print("485 nm : "); Serial.println(features[3], 6);
  Serial.print("510 nm : "); Serial.println(features[4], 6);
  Serial.print("535 nm : "); Serial.println(features[5], 6);
  Serial.print("560 nm : "); Serial.println(features[6], 6);
  Serial.print("585 nm : "); Serial.println(features[7], 6);
  Serial.print("610 nm : "); Serial.println(features[8], 6);
  Serial.print("645 nm : "); Serial.println(features[9], 6);
  Serial.print("680 nm : "); Serial.println(features[10], 6);
  sensor.disableBulb(AS7265x_LED_WHITE);

  // ==========================
  // NIR → 705 - 940 nm
  // L,R,S,T,U,V,W
  // ==========================
  sensor.enableBulb(AS7265x_LED_IR);
delay(3000);
sensor.takeMeasurements();

features[11] = sensor.getCalibratedL(); //705
features[12] = sensor.getCalibratedR(); //730
features[13] = sensor.getCalibratedS(); //760
features[14] = sensor.getCalibratedT(); //810
features[15] = sensor.getCalibratedU(); //860
features[16] = sensor.getCalibratedV(); //900
features[17] = sensor.getCalibratedW(); //940

  Serial.print("705 nm : "); Serial.println(features[11], 6);
  Serial.print("730 nm : "); Serial.println(features[12], 6);
  Serial.print("760 nm : "); Serial.println(features[13], 6);
  Serial.print("810 nm : "); Serial.println(features[14], 6);
  Serial.print("860 nm : "); Serial.println(features[15], 6);
  Serial.print("900 nm : "); Serial.println(features[16], 6);
  Serial.print("940 nm : "); Serial.println(features[17], 6);
  sensor.disableBulb(AS7265x_LED_IR);

}

void displayCenter(const char* text, int y, int textSize) {

  display.setTextSize(textSize);

  int16_t x1, y1;
  uint16_t w, h;

  display.getTextBounds(text, 0, y, &x1, &y1, &w, &h);

  display.setCursor((SCREEN_WIDTH - w) / 2, y);
  display.print(text);
}

void showScanning() {

  display.clearDisplay();

  displayCenter("Scanning...", 10, 1);
  displayCenter("Random Forest", 28, 1);
  displayCenter("Mohon Tunggu", 46, 1);

  display.display();
}

void tampilkan(const char* label, float nilai) {

  display.clearDisplay();

  displayCenter(label, 2, 1);

  display.drawLine(0, 14, 127, 14, WHITE);

  char buffer[20];
  snprintf(buffer, sizeof(buffer), "%.3f", nilai);

  displayCenter(buffer, 24, 2);

  display.drawLine(0, 42, 127, 42, WHITE);

  displayCenter("Prediksi RF", 50, 1);

  display.display();

  Serial.print(label);
  Serial.print(": ");
  Serial.println(nilai, 4);

  delay(5000);
}