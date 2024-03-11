#define USE_ARDUINO_INTERRUPTS true
#define TEMP_MAX 5
#define SENSOR_PIN 13
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PulseSensorPlayground.h>
#include <LiquidCrystal.h>
const int rs =12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

OneWire oneWire(SENSOR_PIN);         // setup a oneWire instance
DallasTemperature tempSensor(&oneWire); // pass oneWire to DallasTemperature library
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // LCD pins

// Constants
const int PULSE_SENSOR_PIN = 0;  // Analog PIN where the PulseSensor is connected
const int LED_PIN = 8;          // On-board LED PIN
const int THRESHOLD = 550;       // Threshold for detecting a heartbeat
 
// Create PulseSensorPlayground object
PulseSensorPlayground pulseSensor;

float tempCelsius;    // temperature in Celsius
float tempFahrenheit; // temperature in Fahrenheit


void setup()
{
  lcd.begin(16, 2);
  lcd.print("Pulse: ");
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  Serial.begin(9600); // initialize serial
  tempSensor.begin();    // initialize the sensor

  pulseSensor.analogInput(PULSE_SENSOR_PIN);
  pulseSensor.blinkOnPulse(LED_PIN);
  pulseSensor.setThreshold(THRESHOLD);

  if (pulseSensor.begin())
  {
    Serial.println("PulseSensor object created successfully!");
  }
}

void loop()
{
  float totalTemp = 0;
  for(int i = 0;i<TEMP_MAX;i++){
    tempSensor.requestTemperatures();             // send the command to get temperatures
    totalTemp += tempSensor.getTempCByIndex(0);
  }
  tempCelsius = totalTemp/TEMP_MAX;  // read temperature in Celsius
  //tempFahrenheit = tempCelsius * 9 / 5 + 32; // convert Celsius to Fahrenheit

  lcd.setCursor(7,1);
  lcd.print(String(tempCelsius,2));
  delay(1000);
  
  int currentBPM = pulseSensor.getBeatsPerMinute();
  int detectPulse = pulseSensor.sawStartOfBeat()?1:0;   
  if (detectPulse){
    Serial.println("♥ A HeartBeat Happened!");
    Serial.print("BPM: ");
    Serial.println(currentBPM);
    //lcd.clear();
    lcd.setCursor(7, 0);
    lcd.print(currentBPM);
    delay(1000);
  }
  else{
    lcd.setCursor(7,0);
    lcd.print("0  "); 
  }

  delay(20);
}
