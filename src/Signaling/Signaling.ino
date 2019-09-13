#include <Servo.h>
/*
 * Ультразвуковой датчик расстояния и Arduino – полный гайд
 * Распиновка ультразвукового датчика расстояния:
 *  VCC: +5V
 *  Trig : Триггер (INPUT) – 11 пин
 *  Echo: Эхо (OUTPUT) – 12 пин
 *  GND: GND
 */
#define TRIGPIN 8
#define ECHOPIN 7
#define PISHALKA 3
#define REDPIN 10
#define SERVOPIN 5
#define MINANGLE 50
#define MAXANGLE 180

void setup(){
  Serial.begin(9600); //Инициализируем и настраиваем скорость порта.
  //Инициализируем входы и выходы
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  pinMode(PISHALKA, OUTPUT);
  pinMode(REDPIN, OUTPUT);
  myservo.attach(SERVOPIN);
  delay(100);
  myservo.write(MINANGLE); //Устанавливаем угол сервы в 50 градусов или положение двери открыто.
}

void loop(){
  /*
   * Датчик срабатывает и генерирует импульсы шириной 10 мкс или больше
   * Генерируем короткий LOW импульс, чтобы обеспечить «чистый» импульс HIGH:
   */
   digitalWrite(TRIGPIN, LOW);
   delayMicroseconds(5);
   digitalWrite(TRIGPIN, HIGH);
   delayMicroseconds(10);
   digitalWrite(TRIGPIN, LOW);
   /*
    * Считываем данные с ультразвукового датчика: значение HIGH, которое
    * зависит от длительности (в микросекундах) между отправкой
    * акустической волны и ее обратном приеме на эхолокаторе.
    */
    pinMode(echoPin, INPUT);
    long duration = pulseIn(echoPin, HIGH);
    long cm = (duration/2) / 29.1;
    /*
     * Логика работы алгоритма:
     *                                  ________________
     *                  _______________ |              | __________________________
     *                 |                |    Начало    |                          |
     *                 |                |______________|                          |
     *                 |                                                          |
     *                 |                      ||                                  |
     *                 |                      ||                                  |
     *                 |                      \/                                  |
     *                 |              __________________                          |
     *               _____            |                |            ____          |
     *              | Нет |  / ====== | До препядствия | ====== \  | Да |         |
     *              |_____|  \ ====== |     < 5 см?    | ====== /  |____|         |
     *                                |________________|                          |
     *                                                                |           |
     *                                                               \|/          |
     *                                                     _________________________
     *                                                     |  Мигаем RGB лентой и  |
     *                                                     | включаем пьезоэлемент,|
     *                                                     | после закрываем дверь |
     *                                                     |_______________________|
     *
    */
    if(cm < 3){
       for(int i = 1; i < 17; i++){
          if(i % 2 == 0){
            digitalWrite(REDPIN, 255);
            tone (PISHALKA, 2000);
          }else{
            digitalWrite(REDPIN, 0);
            noTone (PISHALKA);
          }
          delay(100);
       }
       noTone(PISHALKA);
       myservo.write(MAXANGLE); //Устанавливаем угол сервы в 180 градусов или положение закрыто открыто.
       delay(1000);
       myservo.write(MINANGLE); //Устанавливаем угол сервы в 50 градусов или положение двери открыто.
    }
}

