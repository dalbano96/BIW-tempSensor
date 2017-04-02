void setup() {
  Serial.begin(9600); 
}

void loop() {
  Serial.print("Temperature: ");
  Serial.print("[");
  Serial.print("123.124");
  Serial.print("]");
  
  Serial.print(" pH Levels: ");
  Serial.print("[");
  Serial.print("123.2352");
  Serial.print("]");
  
  Serial.print(" Dissolved Oxygen: ");
  Serial.print("[");
  Serial.print("123.111");
  Serial.print("]");
  
  Serial.print(" Electrical Conductivity: ");
  Serial.print("[");
  Serial.print("123.222");
  Serial.println("]");
  delay(1000);
}
