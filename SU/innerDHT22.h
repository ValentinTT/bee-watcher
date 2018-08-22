#ifndef INNERDHT22_H
#define INNERDHT22_H
/** void setupInnerDHT22()
 * This function start the I2C communication with the module
 */
void setupInnerDHT22();

/** void readTemperatureAndHumidity() 
 * This function read the temperature and humidity of the dht22 sensor in about 250ms to 500ms
 * and then save if on the Data package.
 */
void readTemperatureAndHumidity();
#endif

