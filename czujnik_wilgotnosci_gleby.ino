#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define SensorPin A0
#define touchInputPin 16
//#define moistureSensorPowerPin 3

LiquidCrystal_I2C lcd(0x27, 16, 2);
int sensorValue = 0;
int calibrationLow = 2;
int calibrationHigh = 700;
bool shouldMeasureHumidity = true;

int touchState = 1;
long switchTime = 0;
long switchDelay = 1000;
long measureDelay = 10000;
bool isMeasuring = false;
bool isDisplaying = false;

void setup()
{
  pinMode(SensorPin, INPUT);
//  pinMode(moistureSensorPowerPin, OUTPUT);
//  turnMoistureSensorOff();
  Serial.begin(9600);

//  lcd.begin();
  turnLCDOff();
}

void loop()
{
  long currTime = millis();

  if (!isDisplaying) {
    if (!isMeasuring) {
      if (digitalRead(touchInputPin)) {
        isMeasuring = true;
//        turnMoistureSensorOn();
        isDisplaying = true;
        TurnLCDOn();
        delay(500);
        int humidity = measureHumidity();
//        turnMoistureSensorOff();
        isMeasuring = false;
        displayHumidity(humidity);
        turnLCDOff();
        isDisplaying = false;
      }
    }
  }
}

//void turnMoistureSensorOn(){
//  digitalWrite(moistureSensorPowerPin, HIGH);  
//  delay(100);
//}
//void turnMoistureSensorOff(){
//  digitalWrite(moistureSensorPowerPin, LOW);  
//  Serial.println("sensor off");
//}

void TurnLCDOn() {
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Measuring...");
}

void turnLCDOff() {
  lcd.noBacklight();
  lcd.noDisplay();
}

void displayHumidity(int humidity) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humidity: ");
  lcd.setCursor(9, 0);
  lcd.print(humidity);
  delay(9000);
}

int measureHumidity() {
  int humidityLoopMax = 16;
  int totalHumidityMeasured = 0;

  for (int i = 0; i < humidityLoopMax; i++) {
    int val = map(analogRead(SensorPin), calibrationLow, calibrationHigh, 0, 99);
    totalHumidityMeasured += val;
    delay(100); // TODO: replace with time state
    lcd.setCursor(i ,1);
    lcd.print("#");
  }

  int meanHumidity = totalHumidityMeasured / humidityLoopMax;

  return meanHumidity;
}
