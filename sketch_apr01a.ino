void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(A0, INPUT);
  pinMode(0, OUTPUT);
  pinMode(1, INPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}

double heartMonitor = 0;
unsigned long time = 0;
double hold1, hold2, hold3, average;
double potentiometre = 0;
double period = 0;
double aboveThreshold = 0;

void loop() {
  // put your main code here, to run repeatedly:
  
  potentiometre = analogRead(A1);
 
  
  hold3 = hold2;
  hold2 = hold1;
  hold1 = analogRead(A0);
  average = ((hold1+hold2+hold3)/3);
  
  heartMonitor = analogRead(A0);
  time = millis();
  
  if(average<potentiometre)
  {
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
  }
  else
  {
    if(average>potentiometre)
    {
      period = time-aboveThreshold;
      digitalWrite(7, HIGH);
      digitalWrite(6, LOW);
      aboveThreshold = time;
      //if(period>200.00)
        //Serial.println(period);
    }
  }
  
  //Serial.println(potentiometre);
  Serial.println(average);
  //Serial.println(period);
  /*Serial.print(hold1);
  Serial.print("\t");
  Serial.print(hold2);
  Serial.print("\t");
  Serial.println(hold3);*/
  //Serial.print("\t");
  //Serial.println(time);
  delay(6);
}
