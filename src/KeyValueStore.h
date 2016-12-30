#ifndef KeyValueStore_h
#define KeyValueStore_h

#include "Arduino.h"
#include "FS.h"
#include "ArduinoJson.h"

class KeyValueStore
{
  public:
    KeyValueStore();
    String readString(String);
    int readInt(String);
    void output();
    bool write(String, String);
    bool write(String, int);
    bool clear();
  private:
    bool clear(String);
    bool migrate();
    String readJson();
    String FILENAME_OLD;
    String FILENAME_JSON;
};
#endif
