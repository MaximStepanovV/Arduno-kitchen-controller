///////////10 sep 2020
#include <Thread.h>                      // подключение библиотеки ArduinoThread замедляет передачу некоторых данных
#include <EEPROM.h>                      // Библиотека для сохранения переменных во внутреннюю паметь
#include <SPI.h>                         // Библиотека SPI шины
#include <Ethernet.h>                    // Ethernet библиотека
#include <PubSubClient.h>                // Библиотека MQTT
#include <DHT.h>                         // Библиотека для датчиков DHT11/22
#include <DallasTemperature.h>           /// библиотека датчиков темпереатуры
#include <IRremote.h>                    // Библиотека для ИК датчиков
#define DHTTYPE DHT22                    // Задаём тип DHT датчи
#define DHTPIN 3                         // Номер пина, к которому подсоединен датчик дома
#define DHTPIN2 4                        // Номер пина, к которому подсоединен датчик на улице
DHT dht(DHTPIN, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
OneWire ds(2);                           // Номер пина вход датчиков DALLAS  18b20
//////////////////////////////////////////////////////////////OneWire oneWire(2);                      // вход датчиков 18b20
/////DallasTemperature ds(&oneWire);          /// библиотека датчиков темпереатуры



int BUZZER = 46;                             // Номер пина, к которому подсоединен зуммер
int RECEIVE_PIN = 6;                         // Номер пина, к которому подсоединен ИК датчик
IRrecv irrecv(RECEIVE_PIN);
decode_results results;


Thread ThreadDHT = Thread(); // создаём поток управления 1 сек
Thread ThreadAnalogINfunction = Thread(); // создаём поток управления
Thread ThreadDALAS = Thread(); // создаём поток управления
Thread ThreadRECONNECT = Thread(); // создаём поток управления
int ThreadAnalogInterval = 1000; long ThreadDHTInterval = 60000; int ThreadDALASInterval = 10000; float ThreadRECONNECTInterval = 1000;

int h;
int h2; 
float t;
float t2;

boolean newInfo = 0; //переменная для новой информации html
char newInfo_c = 'u'; //переменная для новой информации html

//////задача по унификации переменных 16 сент
const int analogIn[8] = {A0, A2, A3, A4, A5, A6, A7};                     // Указываем пин, к которому подключен датчик Аналоговый
const int DigitalIn[8] = {8, 9, 40};                          // Указываем пин, к которому подключен датчик дискретный
const int DigitalOut[10] = {48, 50, 52, 22, 23, 24, 25, 26, 27, 28}; // Указываем пин, исполнительного реле {22,23,24,25,26,27,28,29,30,31};
const char *DigitalOutTopic[10] = {"DigitalOut0", "DigitalOut1", "DigitalOut2", "DigitalOut3", "DigitalOut4", "DigitalOut5", "DigitalOut6", "DigitalOut7", "DigitalOut8", "DigitalOut9",}; // Указываем пин, исполнительного реле
int LedDiditalSensor[4] = { 26, 30, 34};             //////////////// светодиод датчика движения
int AnalogSensorValue[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // Объявляем переменную для хранения значений с аналоговых датчиков и задаем ее начальное значение
int DigitalInValue[4];                     //////////////// переменная дискретных датчиков
int DigitalOutState[10];                     //////////////// переменная дискретных выходов - реле контроллера
int AnalogINMAXVALUE[8]; ////////// = {1000,1000,1000,1000,1000,100 0,1000,1000};;                     //////////////// переменная максимумума аналоговых датчиков
const char *charAnalogINName[] = {"AnalogIN0", "AnalogIN1", "AnalogIN2", "AnalogIN3", "AnalogIN4", "AnalogIN5", "AnalogIN6", "AnalogIN7"}; //////////НАЗВАНИЕ ТОПИКОВ АНАЛОГОВЫХ ВХОДОВ
const char *charAnalogINMAXName[] = {"AnalogMAXIN0", "AnalogMAXIN1", "AnalogMAXIN2", "AnalogMAXIN3", "AnalogMAXIN4", "AnalogMAXIN5", "AnalogMAXIN6", "AnalogMAXIN7"}; //////////НАЗВАНИЕ ТОПИКОВ МАКСИМУМОВ АНАЛОГОВЫХ ВХОДОВ
const char *charDigitalINName[] = {"DigitalIN0", "DigitalIN1", "DigitalIN2", "DigitalIN3", "DigitalIN4", "DigitalIN5", "DigitalN6", "DigitalIN7"}; //////////НАЗВАНИЕ ТОПИКОВ ДИСКРЕТНЫХ ВХОДОВ
int EEPROMAdres[8] = {2, 4, 6, 8, 10, 12, 14, 16} ;        //////////////// адреса хранения переменных во внутренней памяти
int alarm_delay = 1000; ////пауза тревоги
float alarm_delay_reboot = 300; /////частота сигнала ошибки и последующей  перезагрузки
int alarm_delay_sensor = 2000;/////частота сигнала тревоги аналогово датчика к примеру превыышение газа
int alarm_delay_alarm = 5000;/////частота сигнала тревоги 
long delay_connect_error = 30000; ///период ожидания тревоги после потери связи с MJM
String  DevaceName = "KITCHEN_CONTROLLER" ;
String  MQTT_connection = "error" ; ////////////////ПОКАЗЫВАЕТ ЕСЛИ СОЕДИНЕНИЕ С СЕРВЕРОМ
int Reboot_signal = 0; ///////////задача на перезагрузку от сервера

int reconect_times = 0;
static char char_temp[10];               ////////значение  дтемпературы DHT1
static char char_hum[10];               ////////значение  влажности DHT1
static char char_temp2[10];             ////////значение  дтемпературы DHT2
static char char_hum2[10];              ////////значение  влажности DHT2
static char char_IR[10];                ////////значение  ИК датчика



// Задаём mac и ip адреса в Локальной сети
byte mac[]    = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
////IPAddress ip{192, 168, 1, 51};       //ip Адрес Ethernet Shild'a Arduino
IPAddress server{192, 168, 1, 50};       //ip Адрес для MQTT Брокера
EthernetClient ethClient;                    ///////////MQTTT
PubSubClient client(ethClient);              ///////////MQTTT
EthernetServer serverhttp(80);               /////////// HTTP
const char *mqtt_user = "majerdomo"; // Логи от сервер MQTTT
const char *mqtt_pass = "mpm2334048"; // Пароль от сервера MQTTT

//////////////////////////////////////



void setup()
{
  // 1 бод равно 0.8 бит/сек
  // 1 бит/сек равно 1.25 бод
  Serial.begin(9600);                  // Задаём скорость порта в БОД'ах.
  Serial.println("DHT22 test!");       // Тестовое сообщ. при откр. Монитора порта
  //////////////////////////ds.begin();                          ///датчики температуры
  dht.begin();
  dht2.begin();
  Ethernet.begin(mac);             // Инициализируем mac, ipEthernet.begin(mac, ip);
  IPAddress ip = Ethernet.localIP();
  client.setServer(server, 1883);
  client.setCallback(callback);
  serverhttp.begin();                    /////http  страница
  ////pinMode(RELE_1, OUTPUT); слишком ярко горит
  pinMode(BUZZER, OUTPUT);
  pinMode(40, INPUT);
  irrecv.enableIRIn(); // Start the receiver ИК датчик

  ThreadAnalogINfunction.onRun(AnalogINfunction);     // назначаем потоку задачу
  ThreadAnalogINfunction.setInterval(ThreadAnalogInterval); // задаём интервал срабатывания, мсек 5 СЕКУНД
  ThreadDHT.onRun(DHT_sensor);     // назначаем потоку задачу
  ThreadDHT.setInterval(ThreadDHTInterval); // задаём интервал срабатывания, мсек 1 МИНУТА
  ThreadDALAS.onRun(DALASfunction);
  ThreadDALAS.setInterval(ThreadDALASInterval);
  ThreadRECONNECT.onRun(reconnect);
  ThreadRECONNECT.setInterval(ThreadRECONNECTInterval);
  for (int i = 0; i <= 7; i++) {        ////////////////////////////чтение значений из внутренней памяти при загрузке контроллера
    AnalogINMAXVALUE[i] = 4 * (EEPROM.read(EEPROMAdres[i]));
  }            ////////////////////////////чтение значений из внутренней памяти при загрузке контроллера

}

void (* resetFunc)(void) = 0; ////////////////////////функция перезагрузки

void reconnect() {                      ////////////////////////функция соенжинения с MQTT
  Serial.println();
  Serial.println("void reconnect");

  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient",mqtt_user,mqtt_pass)) {////////////mqtt авторизация с логином и паролем
      Serial.println("connected");
      // Once connected, publish an announcement...
     //// char str = DevaceName + "Last reboot"; /////////////////////// надо попробывать изменить все названия для трансляции на сервер
      client.publish("OK", "DATE");
      MQTT_connection = "OK" ;  /////////////ПЕРЕМЕННАЯ ПЕРЕДАЕТ НА ВЕБ СТРАНИЦУ СОСТОЯНИЕ
      // ... and resubscribe
      client.subscribe("Reboot_signal"); ///////////////////////////////подписываемся на перезагрузку от MJM
      client.subscribe("Alarm"); ///////////////////////////////подписываемся на тревогу
      for (int i = 0; i <= 7; i++) { ///////////////////////////////подписываемся на максимумы аналоговых датчиков
        client.subscribe(charAnalogINMAXName[i]);
      }
      for (int j = 0; j <= 9; j++) { ///////////////////////////////подписываемся на выхода - реле
        client.subscribe(DigitalOutTopic[j]);
      }
    } else {
      MQTT_connection = "ERROR" ; /////////////ПЕРЕМЕННАЯ ПЕРЕДАЕТ НА ВЕБ СТРАНИЦУ СОСТОЯНИЕ
      Serial.print("failed, rc=");
      Serial.println(client.state());
      Serial.print("Try again after  "); Serial.print(delay_connect_error/1000);Serial.print(" seconds. Try № "); Serial.println(reconect_times+1);
      delay(delay_connect_error); ///////пауза до следующей попытки соединиться
      reconect_times = reconect_times + 1;    /////////////////////////счётчик попыток соединений с сервером
      if (reconect_times > 9) { /////////// количество попыток соединения
      sendSMS ('iii'); Serial.println("SMS losst connect to MJM"), /////////////////////отправка SMS
      alarm_delay = alarm_delay_reboot; soundalarm ();     ///после 10 попыток соединения с паузой "delay_connect_error" отправка СМС, тревога  и презагрузка
      delay(10000);
      Serial.print("reset"), delay(1000);
        resetFunc();
      }
    }
  }
}



// Шапка Функции Callback (обратный вызов)
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String strTopic = String(topic);
  String strPayload = String((char*)payload);
  for (int i = 0; i < length ; i++) { ///
    Serial.print((char)payload[i]);
  }


  //////////////////универсальный приём максимумов аналоговых входов charAnalogINMAXName[]

  for (int i = 0; i <= 7; i++) { ////////////////////////////////////////пять максимумов аналоговых входов
    if (strTopic == charAnalogINMAXName[i]) //////////charAnalogINMAXName[i]
    { Serial.println();
      if ((char)payload[0] == '1') {AnalogINMAXVALUE[i] = 1 * 100;}
      if ((char)payload[0] == '2') {AnalogINMAXVALUE[i] = 2 * 100;}
      if ((char)payload[0] == '3') {AnalogINMAXVALUE[i] = 3 * 100;}
      if ((char)payload[0] == '4') {AnalogINMAXVALUE[i] = 4 * 100;}
      if ((char)payload[0] == '5') {AnalogINMAXVALUE[i] = 5 * 100;}
      if ((char)payload[0] == '6') {AnalogINMAXVALUE[i] = 6 * 100;}
      if ((char)payload[0] == '7') {AnalogINMAXVALUE[i] = 7 * 100;}
      if ((char)payload[0] == '8') {AnalogINMAXVALUE[i] = 8 * 100;}
      if ((char)payload[0] == '9') {AnalogINMAXVALUE[i] = 9 * 100;}
      if ((char)payload[0] == '10') {AnalogINMAXVALUE[i] = 10 * 100;}

      if ((char)payload[1] == '1') {AnalogINMAXVALUE[i] = AnalogINMAXVALUE[i] + 1 * 10;}
      if ((char)payload[1] == '2') {AnalogINMAXVALUE[i] = AnalogINMAXVALUE[i]  + 2 * 10;}
      if ((char)payload[1] == '3') {AnalogINMAXVALUE[i] = AnalogINMAXVALUE[i]  + 3 * 10;}
      if ((char)payload[1] == '4') {AnalogINMAXVALUE[i] = AnalogINMAXVALUE[i]  + 4 * 10;}
      if ((char)payload[1] == '5') {AnalogINMAXVALUE[i] = AnalogINMAXVALUE[i]  + 5 * 10;}
      if ((char)payload[1] == '6') {AnalogINMAXVALUE[i] = AnalogINMAXVALUE[i]  + 6 * 10;}
      if ((char)payload[1] == '7') {AnalogINMAXVALUE[i] = AnalogINMAXVALUE[i]  + 7 * 10;}
      if ((char)payload[1] == '8') {AnalogINMAXVALUE[i] = AnalogINMAXVALUE[i] + 8 * 10;}
      if ((char)payload[1] == '9') {AnalogINMAXVALUE[i] = AnalogINMAXVALUE[i]  + 9 * 10;}
    } Serial.print(AnalogINMAXVALUE[i]);
    EEPROM.write(EEPROMAdres[i], (AnalogINMAXVALUE[i] / 4));
  }   /////запись в память максимальных значений значений аналоговых датчиков


  ////////////////// Блок [Управление всех дискретных выходов  из MajorDomo] ************************************
  for (int i = 0; i <= 10; i++) {
    if (strTopic == DigitalOutTopic[i]) {
      if ((char)payload[0] == '1') {
        DigitalOutState[i] = 1;
      } else {
        DigitalOutState[i] = 0;
      }
      digitalWrite(DigitalOut[i], DigitalOutState[i]);
    }
  }// END Блок [Управление Реле 1 из MajorDomo] ********************************

  ///////////////// Блок [Reboot_signal] ************************************

  if (strTopic == "Reboot_signal") {
    if ((char)payload[0] == '1') {
      alarm_delay = alarm_delay_reboot; digitalWrite(Reboot_signal, 'reboot');
      soundalarm (); delay (300); resetFunc();
    } //////////////////тревога и перезагрузка

  }   // END Блок [Reboot_signal] ********************************

   ////////////////// Блок [Alarm] ************************************ включение сирены

  if (strTopic == "Alarm") {
    if ((char)payload[0] == '1') {alarm_delay = alarm_delay_alarm; soundalarm (); 
    } //////////////////тревога 

  }   // END Блок [Alarm] ******************************** 
}




/////////////////////////////////////////////////////////////программа  АНАЛОГОВЫХ датчиков
void AnalogINfunction() {
  for (int i = 0; i <= 3; i++) {
    int oldg = AnalogSensorValue[i];
    AnalogSensorValue[i] = analogRead(analogIn[i]); /////////////////Датчик газа//считываем значения с датчика
    if ( AnalogSensorValue[i] != oldg ) {
      dtostrf(AnalogSensorValue[i], 3, 0, char_temp);
      client.publish((charAnalogINName[i]), char_temp); /////////////////Датчик газа mq_2//
      Serial.println();
      Serial.print("AnalogIN = "); /////////////////Датчик газа//Для отслеживания данных с датчика транслируем их в монитор порта
      Serial.println(AnalogSensorValue[i]);
      if (AnalogSensorValue[i] > AnalogINMAXVALUE[i]) {
        alarm_delay = alarm_delay_sensor;
      }
    }
  }
}



///////////////////////////////////////////////////////////////программа ДИСКРЕТНЫХ  датчиков  универсальная
void DigitalINfunction() {

  for (int i = 0; i <= 3; i++) {
    int old1 = DigitalInValue[i];
    DigitalInValue[i] = digitalRead(DigitalIn[i]);    //////////////// датчик дискретный значение снимаемое со входа
    digitalWrite(LedDiditalSensor[i], DigitalInValue[i]); //////////////// датчик движения
    if (DigitalInValue[i] != old1) {
      Serial.println(); Serial.print(charDigitalINName[i]); Serial.print(DigitalInValue[i]);
      client.publish(charDigitalINName[i], DigitalInValue[i] ? "1" : "0");
    }
  }
}

///////////////////////////////////////////////////////////////программа DHT  датчиков

void DHT_sensor() {
  int oldh = h;
  int oldh2 = h2;
  float oldt = t;
  float oldt2 = t2;

  h = dht.readHumidity();     // Переменная типа int для Влажности
  t = dht.readTemperature();  // Переменная типа int для Температуры
  h2 = dht2.readHumidity();     // Переменная типа int для Влажности
  t2 = dht2.readTemperature();  // Переменная типа int для Температуры


  if (oldt != t) {
    Serial.println();
    Serial.print("temp 1  = ");
    Serial.print(t);
    dtostrf(t, 3, 1, char_temp);    // Перевод из int в char
    client.publish("sensor/temp", char_temp); //отправляем в Брокер значения
  }

  if (oldh != h) {
    Serial.println();
    Serial.print("hum 1  = ");
    Serial.print(h);
    dtostrf(h, 3, 0, char_hum);    // Перевод из int в char
    client.publish("sensor/hum", char_hum);
  }
  ///////////датчик DHT2
  ///
  if (!isnan(h) || !isnan(t)) { // Проверка. Если не удается считать показания
    if ((oldt2 != t2) and ((abs(t2 - oldt2)) < (abs(t2 / 5)))) {
      if  ((t2 != 0) and (t2 < 50 ) and (t2 > (- 41))) { ////проверка ошибки данных (t2 !=0)  and((abs(t2-oldt2))<(t2/5))and (t2 !=0)and((abs(t2-oldt2))<(t2/5)and((abs(t2-oldt2))<(t2/5))
        Serial.println();
        Serial.print("temp 2  = ");
        Serial.print(t2);
        dtostrf(t2, 3, 1, char_temp2);    // Перевод из int в char
        client.publish("sensor/temp2", char_temp2); //отправляем в Брокер значения
      }
    }

    if ((oldh2 != h2)  and (h2 > 20) and (h2 != 100) ) {  ////проверка ошибки данных  (h2 > 20) and (h2 > 20)) and (h2 != 100) )
      Serial.println();
      Serial.print("hum 2  = ");
      Serial.print(h2);
      dtostrf(h2, 3, 0, char_hum2);    // Перевод из int в char
      client.publish("sensor/hum2", char_hum2);
    }

  }
}




/////////////////////////////////////////////////////////////программа  ИК датчика
void IRsensor() {
  if (irrecv.decode(&results)) {
    Serial.print("0x");
    Serial.println(results.value, HEX);
    Serial.print("0x");
    Serial.println(results.value, HEX);
    dtostrf((results.value), 3, 0, char_IR);
    client.publish("sensor/IR", char_IR); /////////////////Датчик ИК//
    digitalWrite(38, HIGH);
    delay(50);
    digitalWrite(38, LOW);
    irrecv.resume();// Receive the next value
  }
}



///////////////////////////////////////////////Функция бегущих огней при тревоге

void alarm () {
  int n = 22;
  if (client.connect("arduino_client")) {}
  else {
    for (n = 22; n < 40; n = n + 2) { //перебираем номера дискретных выходов  поочереди
      digitalWrite(n, HIGH); //зажигание следующего светодиода
      digitalWrite(BUZZER, HIGH); //зажигание следующего светодиода
      delay(alarm_delay); //задержка 500мсек delay(50); //задержка 500мсек
      digitalWrite(n, LOW);
      digitalWrite(BUZZER, LOW); //зажигание следующего светодиода
    }
  }
}

//////////////////////////////////////////////Функция звукового сигнала при тревоге
void soundalarm () {
  
  
  int n = 0;
  for (n = 0; n < 3; n = n + 1) 
  { //перебираем номера дискретных выходов  поочереди 40
    digitalWrite(BUZZER, HIGH); //сигнал
    digitalWrite(22, HIGH); //зажигание  светодиода
    delay(1000); //задержка 500мсек
    digitalWrite(BUZZER, LOW); digitalWrite(22, LOW);
    delay(alarm_delay); 
  }}

 ////////////////////////////////////////////////////////sendSMS
void sendSMS(char* str){//////
  EthernetClient client;
  IPAddress smsRU(89,188,102,66);
  //char smsRU[] = "www.sms.ru";

  if(client.connect(smsRU, 80)){
    char mess[250] = {"GET https://sms.ru/sms/send?api_id=47F5BCF0-9A3E-9AF5-F2ED-38BD965FBC3F&to=79038211135&msg=Кухонный+контроллер+потерял+сервер+MOJERDOMO&json=1"};
    //char mess[250] = {"GET https://sms.ru/sms/send?api_id=47F5BCF0-9A3E-9AF5-F2ED-38BD965FBC3F&to=79038211135&msg=Тревога!+Движение+в+квартире+(Контроллер)&json=1"};
    //char mess[250] = {"GET https://sms.ru/sms/send?api_id=47F5BCF0-9A3E-9AF5-F2ED-38BD965FBC3F&to=79038211135&msg=Тревога!+Превышение+значения+датчика!+(Контроллер)&json=1"};
    strcat(mess, str);
    strcat(mess, "\n\n");
    Serial.println("HTTP  SMS.ru connected successful log:79038211135");
    client.print(mess);
  }
  else{
    Serial.println("HTTP SMS.ru connection failed log:79038211135");
  }
  client.stop();
}
/////////////////////////////////////////////////////// /     ///sendSMS


  
  

void loop() {
  client.loop();
  DigitalINfunction();
  IRsensor();
  httpwebserwer();
 if (ThreadRECONNECT.shouldRun()) {
    if (!client.connect("RelayClient")) { reconnect();} // Проверим, пришло ли время reconnect// Запускаем функцию переподключения
  }
  if (ThreadDALAS.shouldRun()) {
    DALASfunction(); // Проверим, пришло ли время  getTemperature
  }
  if (ThreadDHT.shouldRun()) {
    ThreadDHT.run(); // Проверим, пришло ли время  DHT_sensor();
  }
  if (ThreadAnalogINfunction.shouldRun()) {
    AnalogINfunction(); // Проверим, пришло ли время  MQ2;
  }


  ////////////////////////////////////////////////////////////////////////
}
