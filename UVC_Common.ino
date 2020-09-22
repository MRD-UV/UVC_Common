#include "/Users/Shared/GitHub/Blaine-RS485-Serial-Test/MODBUS_Slave/MODBUS_Slave.ino"
#include "/Users/Shared/GitHub/NC-Prototype-Codes/ESP_working_Code_from_Arduino_BLE_work092220/ESP_working_Code_from_Arduino_BLE_work092220.ino"

#ifndef COMMON_CODE
#define COMMON_CODE

TaskHandle_t CommunicationCore;


//=====UV VARIABLES=====//
/* default settings will be set if SD card cannot be read*/
int evacTime;
int doseTime;
int warmupTime;
char Version[] = "2.1.1";

//=====CONSTANTS=====//
const int oneSecond = 1000;      //used in timer functions
const int baudRate = 115200;     // only needed when printing to Serial Monitor
const int debounceDelay = 500;

//=====VARIABLES=====//
unsigned long deviceTime;            //always counting up (millis())
unsigned long previousTime;          // time stamp for last action
unsigned long previousStateTime;     // time stamp for last state
unsigned long lastDebounceTime = 0;
unsigned long unix;
unsigned long ONUnix;
unsigned long ONTime;
unsigned long ReadValue;

//=====COUNTERS=====//
unsigned long SU = 0;  //counter for total lamp startups
unsigned long CY = 0;  //counter for total sanitization cycles
int STATE = 0;           //FSM State
int WT = 0;           //timer for WarmUpTime

int TP;
int HU;
int CO = 2345;

//=====FLOATS=====//


//=====PIN SETS=====//
const int button = 33;   // 2 Initiation Button
const int red = 25;      // 6 Error Light
const int yellow = 2;    // 5 Status Light
const int blue = 14;     //   BLE Conn Light
const int beeper = 13;   // 4 Active Buzzer
const int UV = 12;       // 7 RELAY
const int PIR = 26;      // 3 Motion Sensor
const int SDPin = 5;     // 8 chip select pin. Can be changed if needed
const int DHTPin = 32;  // DHT sensor


//=====BOOLEANS=====//
bool stby = true;
bool start = false;
bool newCmd = false;
bool newLamp = false;
bool cancel = false;
bool reset = false;
bool ERreset = false;
bool deviceConnected = false;
bool alertState = LOW;
bool newTM = false;

//=====STRINGS=====//
char filename[] = "/log/D2ymmdd.csv";   //data log file name
char E[] = "100";                       //Error Codes
char dateTime[21];                      //yyyy/mm/dd@hh:mm:ss
char dateStamp[11] = {0};               //yyyy/mm/dd
char timeStamp[9] = {0};                //hh:mm:ss
char dateLD[11];                        //yyyy-mm-dd date the lamp was changed
char LD[12] = "hellolo";
char LI[12] = "peaseniis";
char deviceName[10];


void setup() {
  Serial.begin(baudRate);

  //-----Pin Initialization-----//
  pinMode (SDPin, OUTPUT);
  pinMode (button, INPUT);
  pinMode (PIR, INPUT);
  pinMode (red, OUTPUT);
  pinMode (yellow, OUTPUT);
  pinMode (blue, OUTPUT);
  pinMode (beeper, OUTPUT);
  pinMode (UV, OUTPUT);
  pinMode (DHTPin, OUTPUT);
  pinMode(togglePin, OUTPUT);

  MODBUSsetup();
  BLEsetup();

  //Set Core 0 to CommunicationCoreLoop
  xTaskCreatePinnedToCore(
    SlaveCommunicationCoreLoop,
    "RS485 Communication",
    100000,
    NULL,
    0,
    &CommunicationCore,
    0);

}


#endif
