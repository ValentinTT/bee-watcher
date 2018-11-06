#import "Data.h"

void resetData() {  
  externalData.temperature = dataInitialValue;
  externalData.humidity = dataInitialValue;
  externalData.hiveEntrance = dataInitialValue;
  externalData.safe = hiveSafe; //3 means ok, 2 means fume, 1 means movement, 0 means both
}

void resetMainData() {  
  Data.temperature = dataInitialValue;
  Data.humidity = dataInitialValue;
  Data.hiveEntrance = dataInitialValue;
  Data.fumesLevel = dataInitialValue;
  Data.safe = hiveSafe; //3 means ok, 2 means fume, 1 means movement, 0 means both
}

