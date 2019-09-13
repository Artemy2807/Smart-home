#define IN3 2 
#define IN4 1
#define EN2 0
#define PHOTORESISTOR A0 //Пин к которому подлкючён фоторезистор

bool flagOpen = true, flagClose = true;

void setup(){
  //Инициализируем входы и выходы
  pinMode(PHOTORESISTOR, INPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EN2, OUTPUT);
}

void loop(){
  float value = analogRead(PHOTORESISTOR); 
  if(value > 150 && flagOpen){
    openCurtains(IN3, IN4, EN2, 2000); 
    flagOpen = false; 
    flagClose = true;
  }
  else if(value < 150 && flagClose)
   {closeCurtains(IN3, IN4, EN2, 2000); flagOpen = true; flagClose = false;}
}

void openCurtains(int in1, int in2, int en1, long Time){
  long time = millis();
  int Speed = 255;
    while(millis() - time < Time){
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      analogWrite(en1, Speed);
      Speed = 0;
    }
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(en1, Speed);
}

void closeCurtains(int in1, int in2, int en1, long Time){
  long time = millis();
  int Speed = 255;
    while(millis() - time < Time){
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      analogWrite(en1, Speed);
      Speed = 0;
    }
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(en1, Speed);
}

