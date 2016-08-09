#ifndef KeyValueStore_h
#define KeyValueStore_h

#include "Arduino.h"
#include "FS.h"

class KeyValueStore
{
  public:
    KeyValueStore();
    String read(String);
    void output();
    bool write(String, String);
    bool clear();
  private:
    String FILENAME;
};
#endif
