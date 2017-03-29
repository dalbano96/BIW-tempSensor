#include <Bridge.h>
#include <BridgeClient.h>
#include <BridgeServer.h>
#include <BridgeSSLClient.h>
#include <BridgeUdp.h>

#include <YunServer.h>
#include <YunClient.h>

#include <Console.h>
#include <FileIO.h>
#include <HttpClient.h>
#include <Mailbox.h>
#include <Process.h>

#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>

#include <TimeAlarms.h>
#include <Time.h>
#include <TimeLib.h>


#include <Console.h>
#include <FileIO.h>
#include <HttpClient.h>
#include <Mailbox.h>
#include <Process.h>


#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>


//LiquidCrystal lcd (12,11,5,4,3,2);

int temp_sensor1 = 6; //pin DS18B20 is connected to
int temp_sensor2 = 7; //second temp sensor

int fogger_relay = 3; //pinout of the 5V relay for foggers
int water_relay = 4; //pinout of the 5V relay for water level sensor
int waterSensor = 5; //pintout of the water level sensor

float temperatureC1 = 0; //variable to store temp
float temperatureF1 = 0;

float temperatureC2 = 0;
float temperatureF2 = 0;

int lowerLimit = 13; //define lower threshold of temp - wasabi 13
int upperLimit = 17; //define upper threshold of temp - wasabi 17

byte mac[] = { 0xC4, 0x93, 0x00, 0x04, 0x36, 0xFF}; // ma mac adresse
byte ip[] = { 192, 168, 0, 5 }; //  ip fixe arduino ethernet shield
byte gateway[] = { 192, 168, 0, 254 }; // freebox
byte mask[] = { 255, 255, 255, 0}; // masque reseau

//time_t getNtpTime();
//time_t prevDisplay = 0;

//temp sensor 1
OneWire OneWirePin1(temp_sensor1);
DallasTemperature sensors1(&OneWirePin1);

//temp sensor 2
OneWire OneWirePin2(temp_sensor2);
DallasTemperature sensors2(&OneWirePin2);

void setup(void) {
  //Serial.begin(9600);
  Bridge.begin();
  Console.begin();
  //FileSystem.begin();

  //TURNING OFF LED PIN ON UNO -- fix for avrdude upload via wifi
  //pinMode(13, OUTPUT);

  //lcd.begin(16,2);
  //lcd.print("START UP");
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  sensors1.begin();
  sensors2.begin();

  /*  //NTP Time Sync
    Ethernet.begin(mac, ip, gateway);
    Serial.println("waiting for sync");
    setSyncProvider(getNtpTime);
    while (timeStatus() == timeNotSet)
      ; // wait until the time is set by the sync provider*/

   while (!Console);
  //while(!Serial);
  //Serial.println("Filesystem datalogger\n");

  setTime(14, 00, 0, 17, 12, 16); // set time to Thursday 7:00:00pm Dec 15 2016
  Alarm.timerRepeat(150, RepeatsPump); //triggers pump every 2m 30s
}

void loop(){ 

 
  //digitalWrite(13, LOW);

  //waterLevel_run();
  //waterSensor_TEST();
  Console.print("Requesting Temperatures from sensors: ");

  sensors1.requestTemperatures();
  sensors2.requestTemperatures();

  Console.println("DONE");

  temperatureC1 = sensors1.getTempCByIndex(0);
  temperatureC2 = sensors2.getTempCByIndex(0);

  temperatureF1 = temperatureC1 * 1.8 + 32;
  temperatureF2 = temperatureC2 * 1.8 + 32;

  /*  if ( now() != prevDisplay) //update the display only if the time has changed
    {
      prevDisplay = now();
      digitalClockDisplay();
    }*/

  /*digitalClockDisplay();
    Alarm.delay(1000); // wait one second between clock display*/

  Console.print("Sensor 1 Temperature is ");
  Console.print(temperatureC1);
  Console.print(" deg Celcius, ");
  Console.print(temperatureF1);
  Console.println(" deg Fahrenheit");

  Console.print("Sensor 2 Temperature is ");
  Console.print(temperatureC2);
  Console.print(" deg Celcius, ");
  Console.print(temperatureF2);
  Console.println(" deg Fahrenheit");

  foggersTemp();

  //writing to USB data logger
  dataLogger();

  delay(3000);

  Serial.print('\n');

}

void foggersTemp() {

  if (temperatureC1 < lowerLimit) {
    Console.println (", AIR TEMP BELOW THRESHHOLD");
    digitalWrite(fogger_relay, LOW);
    Console.println ("Foggers Off - TEMP LOW");
    //lcd.setCursor(0,1);
    //lcd.print(", LOW TEMP ");
    //lcd.print(temperature);
  }

  else if (temperatureC1 < upperLimit && temperatureC1 > lowerLimit) {
    Console.println ("AIR TEMP OK");
    digitalWrite(fogger_relay, LOW);
    Console.println ("Foggers Off");
    //lcd.setCursor(0,1);
    //lcd.print(",HIGH TEMP");
  }

  else if (temperatureC1 > upperLimit) {
    Console.println ("AIR TEMP ABOVE THRESHOLD");
    digitalWrite(fogger_relay, HIGH);
    Console.println ("Foggers On");
    //lcd.setCursor(0,1);
    //lcd.print(",HIGH TEMP LOW TEMP");
  }
}

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print("   TIME:");
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print("   DATE:");
  Serial.print(month());
  Serial.print("-");
  Serial.print(day());
  Serial.print("-");
  Serial.print(year());
  Serial.println();

}

void waterSensor_TEST()
{
  int waterSensor_Value = digitalRead(waterSensor);
  Serial.print("SENSOR = ");
  Serial.println(waterSensor_Value);
}
void waterLevel_run()
{
  int waterSensor_Value = digitalRead(waterSensor);
  int waterRelay_Value = digitalRead(water_relay);

  Serial.println("Running water level check");
  Serial.println(waterSensor_Value);
  Serial.println(waterRelay_Value);

  //function to check level of water in tank
  if (waterSensor_Value == LOW) {
    digitalWrite(water_relay, LOW);
    Serial.println("Turning water pump off...");
    Serial.println(waterSensor_Value);
    Serial.println(waterRelay_Value);
  }
  else {
    digitalWrite(water_relay, HIGH);
    Serial.println("Turning on water pump...");
    Serial.println(waterSensor_Value);
    Serial.println(waterRelay_Value);
  }
}


void printDigits(int digits)
{
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void RepeatsPump() {
  digitalWrite(water_relay, HIGH);
  Serial.println("Turning on pump");
  delay(60000);
  Serial.println("Turning off pump");
  digitalWrite(water_relay, LOW);
  Serial.println(" ");
}

String getTimeStamp() {
  String result;
  Process time;
  // date is a command line utility to get the date and the time
  // in different formats depending on the additional parameter
  time.begin("date");
  time.addParameter("+%D-%T");  // parameters: D for the complete date mm/dd/yy
  //             T for the time hh:mm:ss
  time.run();  // run the command

  // read the output of the command
  while (time.available() > 0) {
    char c = time.read();
    if (c != '\n') {
      result += c;
    }
  }

  return result;
}

/**
*	Function to log air and water temperature data
*	@pre Collect data from temperature sensors
*	@post Save collected data to file
*/
void dataLogger() {
  String dataString;
  dataString += getTimeStamp();
  dataString += " = ";
  dataString += "AIR TEMP: ";
  dataString += temperatureF1;
  dataString += ",";
  dataString += " WATER TEMP: ";
  dataString += temperatureF2;
  

  /*for (int analogPin = 6; analogPin < 8; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 7) {
      dataString += ",";
    }
  }*/

  String newFilename = String();
  newFilename += setFilename();
  File dataFile = FileSystem.open(newFilename, FILE_APPEND);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}

/**
*	Function to create a new filename based on existing files. Checks for available names. Sets file as a .csv filetype
*	@pre check for available filenames
*	@post return new filename as String
*/
String setFilename() {
  String fileName = String();
  fileName += "/mnt/sd/";
  unsigned int fileN = 1;
  while (!fileN == 0) {
    fileName = "datalog_";
    fileName += fileN;
    fileName += ".csv";
	
    char charFilename[fileName.length() + 1];
    fileName.toCharArray(charFilename, sizeof(charFilename));
    
    if(SD.exists(charFilename)) {
      fileName += " exists. ";
      Serial.println(fileName);
      fileN++;
    }
	else return fileName;
  }
}
