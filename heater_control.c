*/
Name: SANDRA S
Date: 10/8/2025
Description: Basic Heater Control System
*/

#define TEMP_SENSOR_PIN A0
#define HEATER_PIN 2

#define TARGET_TEMP 60
#define STABILIZING_MARGIN 5
#define OVERHEAT_TEMP 75
#define START_TEMP 30

// Use enum with typedef for compatibility with C
typedef enum 
{
  IDLE,
  HEATING,
  STABILIZING,
  TARGET_REACHED,
  OVERHEAT
} HeaterState;

HeaterState currentState = IDLE;

void setup() 
{
  Serial.begin(9600);
  pinMode(HEATER_PIN, OUTPUT);
  digitalWrite(HEATER_PIN, LOW);
}

float readTemperature() 
{
  int analogValue = analogRead(TEMP_SENSOR_PIN);
  float voltage = (analogValue * 5.0f) / 1023.0f;
  return voltage * 100.0f; // LM35 outputs 10mV per °C
}

void loop() 
{
  float temp = readTemperature();

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" °C, State: ");

  switch (currentState) 
  {
    case IDLE:
      Serial.println("IDLE");
      if (temp >= START_TEMP) 
      {
        currentState = HEATING;
      }
      digitalWrite(HEATER_PIN, LOW);
      break;

    case HEATING:
      Serial.println("HEATING");
      digitalWrite(HEATER_PIN, HIGH);
      if (temp >= TARGET_TEMP) 
      {
        currentState = STABILIZING;
      }
      break;

    case STABILIZING:
      Serial.println("STABILIZING");
      if (temp >= TARGET_TEMP + STABILIZING_MARGIN) 
      {
        currentState = OVERHEAT;
      }
      else if (temp <= TARGET_TEMP + STABILIZING_MARGIN &&
                 temp >= TARGET_TEMP - STABILIZING_MARGIN) 
                 {
        digitalWrite(HEATER_PIN, LOW);
        currentState = TARGET_REACHED;
      } else 
      {
        digitalWrite(HEATER_PIN, HIGH);
        currentState = HEATING;
      }
      break;

    case TARGET_REACHED:
      Serial.println("TARGET_REACHED");
      digitalWrite(HEATER_PIN, LOW);
      if (temp > TARGET_TEMP + STABILIZING_MARGIN) 
      {
        currentState = OVERHEAT;
      } else if (temp < TARGET_TEMP - STABILIZING_MARGIN) 
      {
        currentState = HEATING;
      }
      break;

    case OVERHEAT:
      Serial.println("OVERHEAT");
      digitalWrite(HEATER_PIN, LOW);
      break;

    default:
      currentState = IDLE;
      break;
  }

  delay(1000); // 1 second delay for simulation
}
