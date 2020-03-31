void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(A1, INPUT);
  pinMode(0, OUTPUT);
  pinMode(1, INPUT);
}

double heartMonitor = 0;
unsigned long time = 0;
double hold1, hold2, hold3, average;

void loop() {
  // put your main code here, to run repeatedly:

  hold3 = hold2;
  hold2 = hold1;
  hold1 = analogRead(A1);
  average = ((hold1+hold2+hold3)/3);
  
  heartMonitor = analogRead(A1);
  time = millis();
  Serial.println(average);
  /*Serial.print(hold1);
  Serial.print("\t");
  Serial.print(hold2);
  Serial.print("\t");
  Serial.println(hold3);*/
  //Serial.print("\t");
  //Serial.println(time);
  delay(5);
}
