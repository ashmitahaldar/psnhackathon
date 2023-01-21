#include <Bridge.h>
#include <TembooYunShield.h>
#include "credentials.h" // contains Temboo account information, as described below

#define trigPin 13
#define echoPin 12
int calls = 1;   // Execution count, so this doesn't run forever
int maxCalls = 10;   // Maximum number of times the Choreo should be executed

void setup() {
Bridge.begin();
Console.begin();
Serial.begin (9600);
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
}

void loop() {

long duration, distance;
digitalWrite(trigPin, LOW); 
delayMicroseconds(2); 
digitalWrite(trigPin, HIGH);
delayMicroseconds(10); 
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = (duration/2) / 29.1;

if (calls <= maxCalls) {
    Console.println("Running SendSMS - Run #" + String(calls++));
    
    TembooYunShieldChoreo SendSMSChoreo;

    // Invoke the Temboo client
    SendSMSChoreo.begin();

    // Set Temboo account credentials
    SendSMSChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendSMSChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendSMSChoreo.setAppKey(TEMBOO_APP_KEY);
    SendSMSChoreo.setDeviceType(TEMBOO_DEVICE_TYPE);
    
    // Set profile to use for execution
    SendSMSChoreo.setProfile("AshSMSService");
    
    // Identify the Choreo to run
    SendSMSChoreo.setChoreo("/Library/Twilio/SMSMessages/SendSMS");

    if (distance >= 200 || distance <= 0){ 
      Serial.println("Out of range");
      // Run the Choreo; when results are available, print them to Console
      SendSMSChoreo.run();
    }
    else {
      Serial.print(distance);
      Serial.println(" cm");
    }
    if (distance <= 10){ 
      tone(9,400,100);
    }
    else {
      noTone(9);
    }
    delay(50);
    
    while(SendSMSChoreo.available()) {
        char c = SendSMSChoreo.read();
        Console.print(c);
      }
    SendSMSChoreo.close();
  }

}

