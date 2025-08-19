
const int potPin0 = A0;
const int potPin1 = A1;
const int potPin2 = A2;
const int potPin3 = A3;
void setup() {
  pinMode(potPin0, INPUT);
  pinMode(potPin1, INPUT);
  pinMode(potPin2, INPUT);
  pinMode(potPin3, INPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println(analogRead(potPin0));
  Serial.println(analogRead(potPin1));
  Serial.println(analogRead(potPin2));
  Serial.println(analogRead(potPin3));
  Serial.println("-----------");
  delay(1000);
}
