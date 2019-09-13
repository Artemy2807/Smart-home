#define WATER_PUMP 12 //Пин к которому подлкючена помпа
#define WATER_SENSOR A1 //Пин к которому подлкючён датчик влажности почвы
#define LOWER_THRESHOLD 70 //Нижний порог влажности

void setup(){
  //Инициализируем входы и выходы
  pinMode(WATER_PUMP, OUTPUT);
  pinMode(WATER_SENSOR, INPUT);
}

void loop(){
  float value = analogRead(WATER_SENSOR); //Считываем данные с датчика влажности почвы
  if(value > LOWER_THRESHOLD) digitalWrite(WATER_PUMP, LOW); //Если почва влажнее нижнего порога влажности, то выключаем помпу 
  else digitalWrite(WATER_PUMP, HIGH);//Если-же включаем помпу
}

