#include "KeyValueStore.h"

KeyValueStore::KeyValueStore() {
  FILENAME_OLD = "/KeyValueStore";
  FILENAME_JSON = "/KeyValueStoreJson";
  SPIFFS.begin();
}

bool KeyValueStore::migrate() {
  if (!SPIFFS.exists(FILENAME_OLD)) {
    return true;
  }
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  File f = SPIFFS.open(FILENAME_OLD, "r");
  if (!f) {
    return false;
  }
  while(f.available()) {
    String line = f.readStringUntil('\n');
    int separator = line.indexOf("=");
    if (separator > -1) {
      String key = line.substring(0, separator);
      String value = line.substring(separator + 1);
      value.trim();
      root[key] = value;
    }
  }
  f.close();
  clear(FILENAME_JSON);
  File fJson = SPIFFS.open(FILENAME_JSON, "w");
  root.printTo(fJson);
  fJson.close();
  Serial.println("Migrated KeyValueStore to JSON:");
  root.prettyPrintTo(Serial);
  Serial.println();
  clear(FILENAME_OLD);
  return true;
}

// Delete a file
bool KeyValueStore::clear(String filename) {
  if (!SPIFFS.exists(filename)) {
    return true;
  }
  return SPIFFS.remove(filename);
}

// Remove existing data
bool KeyValueStore::clear() {
  return clear(FILENAME_JSON) && clear(FILENAME_OLD);
}

bool KeyValueStore::write(String key, String value) {
  String json = readJson();
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return false;
  }
  root.set(key, value);
  File file = SPIFFS.open(FILENAME_JSON, "w");
  root.printTo(file);
  file.close();
  return true;
}

bool KeyValueStore::write(String key, int value) {
  String json = readJson();
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return false;
  }
  root.set(key, value);
  File file = SPIFFS.open(FILENAME_JSON, "w");
  root.printTo(file);
  file.close();
  return true;
}

// Read JSON from file as a string
String KeyValueStore::readJson() {
  File f = SPIFFS.open(FILENAME_JSON, "r");

  String json = "";
  while(f.available()) {
    json += f.readString();
  }
  f.close();
  return json;
}

// Output JSON to Serial
void KeyValueStore::output() {
  migrate();
  String json = readJson();
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  root.prettyPrintTo(Serial);
  Serial.println();
}

String KeyValueStore::readString(String key) {
  String json = readJson();
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return "";
  }
  return root.get<String>(key);
}

int KeyValueStore::readInt(String key) {
  String json = readJson();
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return 0;
  }
  return root.get<int>(key);
}
