/**
*	Daryl Albano
*	03/29/17
*	testSerial.c
*/

String input;

void setup() {
	Serial.begin(9600);
}

void loop() {
	// Testing out user input, it should return the entered value to webpage
	if (Serial.available()) {
		input = Serial.read();
		Serial.println(input);
		delay(1000);
	}
}