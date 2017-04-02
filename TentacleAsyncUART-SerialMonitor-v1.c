/**
	*	Daryl Albano
	*	Big Island Wasabi
	*	04/01/17
	*	Tentacle Shield, Data Logger
*/

#define NUM_CIRCUITS 4
#define baud_host 9600
#define baud_circuits 9600

const unsigned int send_readings_every = 1000;
unsigned long next_serial_time;

String dataString;

const unsigned int reading_delay = 500;
unsigned long next_reading_time;

boolean request_pending = false;

const unsigned int blink_frequency = 250;
unsigned long next_blink_time;
boolean led_state = LOW;


char *channel_names[] = {"DO", "TEMP", "PH", "EC"};
String readings[NUM_CIRCUITS] = {"11","22","33","44"};

void setup() {
	Serial.begin(baud_host);
	next_serial_time = millis() + send_readings_every;
	next_reading_time = millis() + reading_delay;
	Serial.println("-----");
}

void loop() {
	// do_sensor_readings();
	do_serial();
	// blink_led();
}

void do_serial() {
	/* if (millis() >= next_serial_time) {
		Serial.println("-------------");
	} */
	
	// File dataFile = FileSystem.open("tentacle_data.csv");
	
	for (int i = 0; i < NUM_CIRCUITS; i++) {
		Serial.print(channel_names[i]);
		Serial.print(": ");
		Serial.print("[");
		Serial.print(readings[i]);
		Serial.print("]");
		
		/* if (dataFile) {
			dataLogger(channel_names[i], readings[i], dataFile);
		}
		else {
			Serial.println("error opening file");
		} */
	}
	// dataFile.close();
	// next_serial_time = millis() + send_readings_every;
}

/* void dataLogger(char *channel_name_Arg, String reading_Arg, File dataFile_Arg) {
	dataFile.print(channel_name_Arg);
	dataFile.print(",");
	dataFile.print(reading_Arg);
	dataFile.println();
} */