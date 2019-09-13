#define MICROPHONEPORT A2 //Порт к которому подлкючён микрофон.
#define KPINTEGRAL 0.1 //Коэффициент для вычисления интеграла.
#define KPPOWER 1.2 //Коэффициент для вычисления значения которое будет подоваться на RGB ленту.
#define KPSMH 0.5 //Коэффициент для сглаживания значения которое будет подоваться на RGB ленту.
//Коэффициент - это число на которое умножается выражение или число, для его сглаживания, увелечения или уменьшения.
/* RGBpins - массив который содержит номера портов к которым подключена
   RGB лента. Номера портов: (красный: 10, зелёный: 11, синий: 9).*/
const int RGBpins[] = {10, 11, 9};
int RGBvalue[] = {0, 0, 0}; //В этом массиве будут храниться значения которые будут подоваться на RGB ленту.
int oldRGBvalue[] = {0, 0, 0};//В этом массиве будут храниться старые значения которые подовались на RGB ленту.
float integrals[] = {0, 0, 0};//В этом массиве будут храниться интегралы для значений которые будут подаваться на RGB ленту.
//В нашем случае интеграл - это число которое будет прибовляться к другому числу для его корректировки.

void setup() {
  //pinMode(MICROPHONEPORT, INPUT); //Инициализирует порт к которому подлкючён микрофон.
  //for(int i = 0; i < 3; i++) pinMode(RGBpins[i], OUTPUT); //Этот цикл for проходится по массиву RGBpins и инициализирует порты к которым подлкючена лента. Эта запись эквивалентна данной записи:
  /*
   * [Подсказка]: индексация массива начинается с 0.
   * pinMode(RGBpins[0], OUTPUT); //Инициализирует порт к которому подлкючён красный пин RGB ленты.
   * pinMode(RGBpins[1], OUTPUT); //Инициализирует порт к которому подлкючён зелёный пин RGB ленты.
   * pinMode(RGBpins[2], OUTPUT); //Инициализирует порт к которому подлкючён синий пин RGB ленты.
   */
  Serial.begin(9600); //Инициализирует и настраивает скорость порта.

}

void loop() {
  float var = analogRead(MICROPHONEPORT); //Считывает данные с порта к которому подлкючён микрофон.
  //Serial.println(var); //Вывод данных с порта. 
  //integralReg - это функция для плавного включения или выключения светодиодов на RGB ленте.
  /*
   * Если var > 0 и var < 400, тогда в помещение тихо, если var > 400 и var < 600, тогда в помещение средняя громкость, если var > 600, тогда в помещение громко.
   * Если var > 0 и var < 400, тогда расчитываем напряжение для плавного включения зелёного цвета и плавного выключения красного, синего на RGB ленте.
   * Если var > 400 и var < 600, тогда расчитываем напряжение для плавного включения синего цвета и плавного выключения красного, зелёного на RGB ленте.
   * Если var > 600, тогда расчитываем напряжение для плавного включения красного цвета и плавного выключения синего, зелёного на RGB ленте.
   */
  if(var > 0 && var < 400){
    integralReg(0, //Желаемая яркость светодиода.
     RGBvalue[0], //Настоящая яркость светодиода.
     RGBvalue[0], //Переменная куда будет записыватся яркость светодиода.
     oldRGBvalue[0], //Старая яркость светодиода.
     KPINTEGRAL, //Коэффициент для вычисления интеграла
     KPPOWER, //Коэффициент для вычисления яркости светодиода.
     KPSMH, //Коэффициент для сглаживания резкого перепада яркостей.
     integrals[0] //Старый интеграл.
    );
    integralReg(255, RGBvalue[1], RGBvalue[1], oldRGBvalue[1], KPINTEGRAL, KPPOWER, KPSMH, integrals[1]);
    integralReg(0, RGBvalue[2], RGBvalue[2], oldRGBvalue[2], KPINTEGRAL, KPPOWER, KPSMH, integrals[2]);
  }
  else if(var > 400 && var < 600){
    integralReg(0, RGBvalue[0], RGBvalue[0], oldRGBvalue[0], KPINTEGRAL, KPPOWER, KPSMH,integrals[0]);
    integralReg(0, RGBvalue[1], RGBvalue[1], oldRGBvalue[1], KPINTEGRAL, KPPOWER, KPSMH, integrals[1]);
    integralReg(255, RGBvalue[2], RGBvalue[2], oldRGBvalue[2], KPINTEGRAL, KPPOWER, KPSMH, integrals[2]);
  }
  else if(var > 600){
    integralReg(255, RGBvalue[0], RGBvalue[0], oldRGBvalue[0], KPINTEGRAL, KPPOWER, KPSMH, integrals[0]);
    integralReg(0, RGBvalue[1], RGBvalue[1], oldRGBvalue[1], KPINTEGRAL, KPPOWER, KPSMH, integrals[1]);
    integralReg(0, RGBvalue[2], RGBvalue[2], oldRGBvalue[2], KPINTEGRAL, KPPOWER, KPSMH, integrals[2]);
  } 
  for(int i = 0; i < 3; i++) analogWrite(RGBpins[i], RGBvalue[i]); //Подаём расчитаные значения на RGB ленту.
}

void integralReg(const int& likeVar, const int& var, int& powerPin, int& oldPowerPin, const double& kpIntegral, const double& kpPower, const double& kpSmh, float& integral){
   float error = likeVar - var;
   oldPowerPin = powerPin;
   float integralT = error * kpIntegral;
   integral += integralT;
   if(integral > 255) integral = 255; 
   powerPin = (int)(error * kpPower + integral);
   powerPin = powerPin>255 ? 255 : (powerPin<0 ? 0 : powerPin);
   powerPin = (powerPin + oldPowerPin)*kpSmh;
}

