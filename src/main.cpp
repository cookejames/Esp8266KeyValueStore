#include "KeyValueStore.h"

KeyValueStore kvstore;

String readString(bool output) {
  char byte = 0;
  String string = "";
  while (true) {
    if (Serial.available() > 0) {
      byte = Serial.read();
      if (output) {
        Serial.print(byte);
      }
      if (byte == '\n') {
        string.trim();
        return string;
      }
      string += byte;
    }
  }
}

String readString() {
  return readString(false);
}

bool userConfirm() {
  while (true) {
    if (Serial.available() > 0) {
      return Serial.read() == 'y';
    }
  }
}

void enterUserValue(String key) {
  Serial.printf("Set '%s': \n", key.c_str());
  String input = readString(true);

  if (input.length() == 0) {
    Serial.println("Skipping...");
    return;
  }

  Serial.print("You entered ");
  Serial.println(input);
  Serial.println("Confirm (y/n)?");
  if (userConfirm()) {
    kvstore.write(key, input);
  }
}

void setup() {
  Serial.begin(9600);
  delay(4000);

  Serial.println("Clear local storage (y/n)?");
  if (userConfirm()) {
    kvstore.clear();
    Serial.println("Storage cleared");
  } else {
    Serial.println("Storage contents:");
    kvstore.output();
  }

  enterUserValue("mqttServer");
  enterUserValue("mqttUsername");
  enterUserValue("mqttPassword");
  enterUserValue("wifiSsid");
  enterUserValue("wifiPassword");
  enterUserValue("nodeId");

  Serial.println("Storage is now set to:");
  kvstore.output();
}



void loop() {}
