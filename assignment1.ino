#include <dht.h>

// DEfine pin cinnections
#define DHTPIN 2        // DHT11 connected to D4 (GPIO2)
#define TRIG_PIN 12     // Ultrasonic Sensor Trigger pin
#define ECHO_PIN 14     // Ultrasonic Sensor Echo pin
#define RELAY_PIN 16    // Relay pin

// Set operational constants
#define TEMP_SETPOINT 25  // Desired temperature threshold for relay activation
#define SOUND_VELOCITY 0.034  // Sound velocity in cm used for distance calculation
#define PERSON_CLOSE_DISTANCE 500 // Distance in cm to consider a person is close

// DHT sensor
dht DHT;

// Variables for Ultrasonic Sensor
long duration;
float distanceCm;

void setup() {
  Serial.begin(115200); // Initialize serial communication at 115200 baud rate
  pinMode(TRIG_PIN, OUTPUT); // Sets the TRIG_PIN as an output for the ultrasonic sensor
  pinMode(ECHO_PIN, INPUT);  // Sets the ECHO_PIN as an input for the ultrasonic sensor
  pinMode(RELAY_PIN, OUTPUT); // Sets the RELAY_PIN as an output for controlling the relay
}



void loop() {
  // Read temperature and humidity from DHT11
  int chk = DHT.read11(DHTPIN);
  Serial.print("Temperature=");  // Print the word "Temperature="
  Serial.println(DHT.temperature);  // Print the temperature reading
  Serial.print("Humidity=");  // Print the word "Humidity="
  Serial.println(DHT.humidity);  // Print the humidity reading

  // Measure distance with ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);  // Ensure TRIG_PIN is low
  delayMicroseconds(2);  // Short delay before sending the pulse
  digitalWrite(TRIG_PIN, HIGH);  // Send a 10-microsecond high pulse
  delayMicroseconds(10);  // Duration of the pulse
  digitalWrite(TRIG_PIN, LOW);  // Set TRIG_PIN back to low
  duration = pulseIn(ECHO_PIN, HIGH);  // Read the time for which the echo pin is high
  distanceCm = duration * SOUND_VELOCITY / 2;  // Calculate distance
  Serial.print("Distance (cm): ");  // Print the word "Distance (cm):"
  Serial.println(distanceCm);  // Print the calculated distance

  // Control logic for the relay based on temperature and distance
  if (DHT.temperature > TEMP_SETPOINT && distanceCm < PERSON_CLOSE_DISTANCE) {
    digitalWrite(RELAY_PIN, HIGH); // Activate the relay if conditions are met
    Serial.println("Relay ON - Power ON");  // Notify that the heating is turned on
  } else {
    digitalWrite(RELAY_PIN, LOW);  // Deactivate the relay if conditions are not met
    Serial.println("Relay OFF - Power OFF");  // Notify that the heating is turned off
  }

  delay(1000); // Delay before next reading
}
