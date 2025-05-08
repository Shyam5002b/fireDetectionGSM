#include <SoftwareSerial.h>

const String PHONE_1 = "+919994686038";
const String PHONE_2 = ""; //optional
const String PHONE_3 = ""; //optional

#define rxPin 2
#define txPin 3
SoftwareSerial sim800L(rxPin, txPin);

#define flame_sensor_pin_1 8
#define flame_sensor_pin_2 9
#define flame_sensor_pin_3 10

#define buzzer_pin 4

String f1 = "";
String f2 = "";
String f3 = "";

void setup() {
  Serial.begin(9600);
  sim800L.begin(9600);
  pinMode(flame_sensor_pin_1, INPUT);
  pinMode(flame_sensor_pin_2, INPUT);
  pinMode(flame_sensor_pin_3, INPUT);
  
  pinMode(buzzer_pin, OUTPUT);
  digitalWrite(buzzer_pin, LOW);
  
  sim800L.println("AT");
  delay(2000);
  sim800L.println("AT+CMGF=1");
  delay(2000);
}

void loop() {
  while (sim800L.available()) {
    Serial.println(sim800L.readString());
  }
  
  int flame_value_1 = digitalRead(flame_sensor_pin_1);
  int flame_value_2 = digitalRead(flame_sensor_pin_2);
  int flame_value_3 = digitalRead(flame_sensor_pin_3);
  
  if (flame_value_1 == LOW) {
    digitalWrite(buzzer_pin, HIGH);
    f1 = "1, ";
  }
  
  if (flame_value_2 == LOW) {
    f2 = "2, ";
  }
  
  if (flame_value_3 == LOW) {
    f3 = "3";
  }
  if (flame_value_1 == LOW || flame_value_2 == LOW || flame_value_3 == LOW) {
    digitalWrite(buzzer_pin, HIGH);
    String message = "Flame detected at Sensor(s) " + f1 + f2 + f3;
    send_multi_sms(message);
    make_multi_call();
  }
  // Turn off the buzzer if no fire detected
  if (flame_value_1 == HIGH && flame_value_2 == HIGH && flame_value_3 == HIGH) {
    digitalWrite(buzzer_pin, LOW);
  }
}

void send_multi_sms(String text) {
  if (PHONE_1 != "") {
    send_sms(text, PHONE_1);
  }
  if (PHONE_2 != "") {
    send_sms(text, PHONE_2);
  }
  if (PHONE_3 != "") {
    send_sms(text, PHONE_3);
  }
}

void make_multi_call() {
  if (PHONE_1 != "") {
    make_call(PHONE_1);
  }
  if (PHONE_2 != "") {
    make_call(PHONE_2);
  }
  if (PHONE_3 != "") {
    make_call(PHONE_3);
  }
}

void send_sms(String text, String phone) {
  Serial.println("Sending SMS...");
  delay(500);
  sim800L.print("AT+CMGF=1\r");
  delay(10000);
  sim800L.print("AT+CMGS=\"" + phone + "\"\r");
  delay(10000);
  sim800L.print(text);
  delay(100);
  sim800L.write(0x1A); // ascii code for ctrl-26
  delay(5000);
}

void make_call(String phone) {
  Serial.println("Making call...");
  sim800L.println("ATD" + phone + ";");
  delay(30000); // 20 sec delay
  sim800L.println("ATH");
  delay(5000); // 1 sec delay
}
