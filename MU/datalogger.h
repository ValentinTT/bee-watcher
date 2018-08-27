#ifndef DATALOGGER_H
#define DATALOGGER_H
/**void setupDatalogger()
 * This function start the communication with the microSd adapter module
 */
void setupDatalogger();

/**void logData(bool isExternalData)
 * This function saves the data from the hives into a file of the microSd
 */
void logData(bool isExternalData);
#endif
