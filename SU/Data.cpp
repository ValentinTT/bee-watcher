#include "Data.h"

void resetData() {
  Data.temperature = dataInitialValue;
  Data.humidity = dataInitialValue;
  Data.hiveEntrance = dataInitialValue;
  Data.safe = hiveSafe;
}

