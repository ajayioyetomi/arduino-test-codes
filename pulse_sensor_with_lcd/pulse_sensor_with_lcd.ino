// Include necessary libraries
#define USE_ARDUINO_INTERRUPTS true
#define TEMP_MAX 50
#define tempSensorPin A1
#include <PulseSensorPlayground.h>
#include <LiquidCrystal.h>
const int rs =12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // LCD pins
 
// Constants
const int PULSE_SENSOR_PIN = 0;  // Analog PIN where the PulseSensor is connected
const int LED_PIN = 13;          // On-board LED PIN
const int THRESHOLD = 550;       // Threshold for detecting a heartbeat
 
// Create PulseSensorPlayground object
PulseSensorPlayground pulseSensor;
 
void setup()
{
  // Initialize Serial Monitor
  lcd.begin(16, 2);
  lcd.print("Pulse: ");
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  Serial.begin(9600);
  
  // Configure PulseSensor
  pulseSensor.analogInput(PULSE_SENSOR_PIN);
  pulseSensor.blinkOnPulse(LED_PIN);
  pulseSensor.setThreshold(THRESHOLD);
 
  // Check if PulseSensor is initialized
  if (pulseSensor.begin())
  {
    Serial.println("PulseSensor object created successfully!");
  }
}
 
void loop()
{
  int totalTempReading = 0;
  for(int i =0;i<TEMP_MAX;i++){
    totalTempReading += analogRead(tempSensorPin);
  }
  int averageTempValue = totalTempReading/TEMP_MAX;
  float tempValue = averageTempValue * 4.88;
  float tempC = tempValue/10;
  // Get the current Beats Per Minute (BPM)
  int currentBPM = pulseSensor.getBeatsPerMinute();
 
  // Check if a heartbeat is detected
  if (pulseSensor.sawStartOfBeat())
  {
    Serial.println("♥ A HeartBeat Happened!");
    Serial.print("BPM: ");
    Serial.println(currentBPM);
    //lcd.clear();
    lcd.setCursor(7, 0);
    lcd.print(currentBPM);
    lcd.setCursor(7,1);
    lcd.print(tempC);
    delay(2000);
  }
  else{
    lcd.setCursor(7,0);
    lcd.print("0  "); 
    lcd.setCursor(7,1);
    lcd.print(tempC);
    delay(10); 
  }

}
