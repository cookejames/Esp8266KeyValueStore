#include "KeyValueStore.h"

KeyValueStore::KeyValueStore() {
  FILENAME = "/KeyValueStore";
  SPIFFS.begin();
}

bool KeyValueStore::clear() {
  if (!SPIFFS.exists(FILENAME)) {
    return true;
  }
  return SPIFFS.remove(FILENAME);
}

bool KeyValueStore::write(String key, String value) {
  File f = SPIFFS.open(FILENAME, "r");

  //Read all the file into a buffer
  String buffer[100];
  int lineNo = 0;
  bool replaced = false;
  String search = key + "=";
  while(f && f.available()) {
    String line = f.readStringUntil('\n');
    //Replace any lines with the key to be written
    if (line.startsWith(search)) {
      buffer[lineNo] = key + "=" + value;
      replaced = true;
    } else {
      buffer[lineNo] = line;
    }
    lineNo++;
  }
  //If the key doesn't exist add it
  if (!replaced) {
    buffer[lineNo] = key + "=" + value;
  }
  f.close();

  //Write the buffer backk to the file
  f = SPIFFS.open(FILENAME, "w");
  for (int i = 0; i <= lineNo; i++) {
    f.println(buffer[i]);
  }
  f.close();

  return true;
}

void KeyValueStore::output() {
  File f = SPIFFS.open(FILENAME, "r");
  if (!f) {
    return;
  }

  while(f.available()) {
    String line = f.readStringUntil('\n');
    Serial.println(line);
  }
  f.close();
}

String KeyValueStore::read(String key) {
  File f = SPIFFS.open(FILENAME, "r");
  if (!f) {
    return "";
  }
  String search = key + "=";
  while(f.available()) {
    String line = f.readStringUntil('\n');
    if (line.startsWith(search)) {
      f.close();
      String value = line.substring(search.length());
      value.trim();
      return value;
    }
  }
  f.close();
  return "";
}
