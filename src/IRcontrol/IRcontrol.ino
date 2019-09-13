#include <Wire.h> //Библиотека для работы с I2C устройствами
#include <LiquidCrystal_I2C.h> //Библиотека для работы с ЖК дисплеем
#include <IRremote.h> //Библиотека для работы с ИК приёмником
#define RECV_PIN  13 //Пин к которому подлкючён ИК приёмник
#define RED_PIN 10 //Пин к которому подлкючён красный вывод RGB ленты
#define GREEN_PIN 11 //Пин к которому подлкючён зелёный вывод RGB ленты
#define BLUE_PIN 9 //Пин к которому подлкючён синий вывод RGB ленты
#define BUTTON_TOP 16615543 //Код для кнопки "вверх" на пульте
#define BUTTON_BOTTOM 16619623 //Код для кнопки "вниз" на пульте
#define BUTTON_LEFT 16591063 //Код для кнопки "влево" на пульте
#define BUTTON_RIGHT 16607383 //Код для кнопки "вправо" на пульте
#define BUTTON_OK 16623703 //Код для кнопки "ok" на пульте
int mode = 0; 
bool flags[4] = {false, false, false, false};
int brtLine[3] = {255, 255, 255}; //Массив в котором будет хранится значения яркости RGB 
IRrecv irrecv(RECV_PIN); //Вызываем конструктор клсса, указываем пин к которому подключён ИК приёмник
decode_results results;
LiquidCrystal_I2C lcd(
  0x27, //Адрес дисплея по шине I2C
  16, //Количество столбцов в дисплее
  2 //Количество строк в дисплее
);

void setup() {
  //Инициализируем входы и выходы
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  lcd.begin(); //Начинаем работу с ЖК дисплеем
  lcd.backlight(); //Включаем подсветку у ЖК десплея
  irrecv.enableIRIn(); //Начинаем работу ИК приёмника
}

void loop() {
    if (irrecv.decode(&results)) irrecv.resume(); //Если пришли данные, то считываем их и декодируем
    switch(results.value){
      case BUTTON_TOP:
      {
        //Смена управляемым устройством
        if(mode + 1 > 5)mode = 1; //Если 
        else mode++; //
        lcd.clear(); //Чистим дисплей
      }
      break;
      case BUTTON_BOTTOM:
      {
        //Смена управляемым устройством
        if(mode - 1 < 1) mode = 5;
        else mode--;
        lcd.clear(); //Чистим дисплей
      }
      break;
      case BUTTON_LEFT:
      {
        //Выключение управляемого устройства
        lcd.clear(); //Чистим дисплей
        flags[mode - 1] = false;
      }
      break;
      case BUTTON_RIGHT:
      {
        //Включение управляемого устройства
        lcd.clear(); //Чистим дисплей
        flags[mode - 1] = true;
      }
      break;
      case BUTTON_OK:
      {
        if(mode == 5){
          for(int i = 1; i < 4; i++){
            switch(i){
              case 1:
              {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("BRT RGB line:");
                lcd.setCursor(4, 1);
                lcd.print(brtLine[1]);
                lcd.setCursor(8, 1);
                lcd.print(brtLine[2]);
                brtLine[i - 1] = getColor();
              }
                break;
                case 2:
                {
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("BRT RGB line:");
                  lcd.setCursor(0, 1);
                  lcd.print(brtLine[0]);
                  lcd.setCursor(8, 1);
                  lcd.print(brtLine[2]);
                  brtLine[i - 1] = getColor();
                }
                break;
                case 3:
                {
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("BRT RGB line:");
                  lcd.setCursor(0, 1);
                  lcd.print(brtLine[0]);
                  lcd.setCursor(4, 1);
                  lcd.print(brtLine[1]);
                  brtLine[i - 1] = getColor();
                }
                break;
              }
            }
          }      
        }
      break;
    }
    results.value = 0;
    switch(mode){
      case 0:
      {
          lcd.setCursor(0, 0);
          lcd.print("Smart Home");
      }
      break;
      case 1:
      {
          lcd.setCursor(0, 0);
          lcd.print("Curtains:");
          lcd.setCursor(0, 1);
          if(flags[mode - 1] == true)lcd.print("On");
          else lcd.print("Off");
      }
      break;
      case 2:
      {
          lcd.setCursor(0, 0);
          lcd.print("Door:");
          lcd.setCursor(0, 1);
          if(flags[mode - 1] == true)lcd.print("On");
          else lcd.print("Off");
      }
      break;
      case 3:
      {
          lcd.setCursor(0, 0);
          lcd.print("Passive cooling:");
          lcd.setCursor(0, 1);
          if(flags[mode - 1] == true)lcd.print("On");
          else lcd.print("Off");
      }
      break;
      case 4:
      {
          lcd.setCursor(0, 0);
          lcd.print("Active cooling:");
          lcd.setCursor(0, 1);
          if(flags[mode - 1] == true)lcd.print("On");
          else lcd.print("Off");
      }
      break;
      case 5:
      {
        lcd.setCursor(0, 0);
        lcd.print("BRT RGB line:");
        lcd.setCursor(0, 1);
        lcd.print(brtLine[0]);
        lcd.setCursor(4, 1);
        lcd.print(brtLine[1]);
        lcd.setCursor(8, 1);
        lcd.print(brtLine[2]);
      }
      break;
    }
    analogWrite(RED_PIN, brtLine[0]);
    analogWrite(GREEN_PIN, brtLine[1]);
    analogWrite(BLUE_PIN, brtLine[2]);
}

int getColor(){
     decode_results result;
     int valueColor[3] = {0, 0, 0};
     int Color = 0;
     int index = 1;
      while(true){
        if (irrecv.decode(&result)) 
          { irrecv.resume(); }
          if(result.value == 16580863) {valueColor[index - 1] = 1; index++;} 
          else if(result.value == 16613503){valueColor[index - 1] = 2;index++;} 
          else if(result.value == 16597183){valueColor[index - 1] = 3;index++;} 
          else if(result.value == 16589023){valueColor[index - 1] = 4;index++;} 
          else if(result.value == 16621663){valueColor[index - 1] = 5;index++;} 
          else if(result.value == 16605343){valueColor[index - 1] = 6;index++;} 
          else if(result.value == 16584943){valueColor[index - 1] = 7;index++;}
          else if(result.value == 16617583){valueColor[index - 1] = 8;index++;} 
          else if(result.value == 16601263){valueColor[index - 1] = 9;index++;} 
          else if(result.value == 16625743){valueColor[index - 1] = 0;index++;}
          if(result.value == 16623703 && index == 2) 
            { Color = valueColor[0]; break;}
          else if(result.value == 16623703 && index == 3) 
            { Color = (valueColor[0] * 10) + valueColor[1]; break;}
          if(index == 4){
            Color = (valueColor[0] * 100) + (valueColor[1] * 10) + valueColor[2];
            if(Color > 255) Color = 255;
            break;
          }
          result.value = 0;
        }
    return Color;
   }
