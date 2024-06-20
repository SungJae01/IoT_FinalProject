/* 
    Title : 넌 내꺼야! 나만의 비바리움 스마트 사육장
    IoT 시스템응용및프로그래밍 Final Project
    Member : 박성재, 김서율, 김지찬, 유지석
    Date : 2024.05.10 ~ 2024.06.11
    
      - 개발 일지 -

    1일차 : 2024.05.10
    장소 : 온라인, 디스코드, LIVE SHARE
    한 일 : 아두이노 메소드 코딩

    2일차 : 2024.06.05
    장소 : 미래혁신관
    한 일: 아두이노, 어플, 데이터베이스 연결 / 아두이노 센서 및 엑츄에이터 작동 테스트

    3일차 : 2024.06.09
    장소 : 미래혁신관
    한 일 : 아두이노에서 측정한 센서값을 데이터베이스에 저장, 어플에서 실시간 모니터링이 가능하게 UI 디자인

    4일차 : 2024.06.11 완성!
    장소 : 미래혁신관
    한 일 : 사육장 환경 조성, 사육장과 센서 위치에 맞춰 가공해온 아크릴판에 센서 및 엑츄에이터 조립
          영상촬영, 깃허브, 보고서 제출
*/

#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W) || defined(ARDUINO_GIGA)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif __has_include(<WiFiNINA.h>) || defined(ARDUINO_NANO_RP2040_CONNECT)
#include <WiFiNINA.h>
#elif __has_include(<WiFi101.h>)
#include <WiFi101.h>
#elif __has_include(<WiFiS3.h>) || defined(ARDUINO_UNOWIFIR4)
#include <WiFiS3.h>
#elif __has_include(<WiFiC3.h>) || defined(ARDUINO_PORTENTA_C33)
#include <WiFiC3.h>
#elif __has_include(<WiFi.h>)
#include <WiFi.h>
#endif
#include <Adafruit_NeoPixel.h>
#include "RTC.h"

#include <FirebaseClient.h>       //FirebaseClient 라이브러리
#include "arduino_secrets.h"      // 와이파이 SSID, PASSWORD 값이 적힌 헤더파일
#include <DHT11.h>

//아두이노 핀 정하기
#define DHT11_PIN 13
#define ULTRASONIC_TRIG 9       //(OUTPUT)
#define ULTRASONIC_ECHO 8       //(INPUT)
#define WATERPUMP1 5
#define WATERPUMP2 4
#define FAN 11
#define HEATER 3
#define HUMIDIFIER 2
#define WATERLEVEL A1
#define SOILMOISTURE A0
#define RELAYLED 7
#define neoLED 6
#define NUMPIXEL 2
#define BRIGHTNESS 255

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXEL, neoLED, NEO_GRB + NEO_KHZ800);


DHT11 dht11(DHT11_PIN);

char ssid[] = WIFI_SSID;        // your network SSID (name)
char pass[] = WIFI_PASSWORD;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(80);

/* --- Firebase Setup --- */
void authHandler();

void printError(int code, const String &msg);

void printResult(AsyncResult &aResult);

DefaultNetwork network; // initilize with boolean parameter to enable/disable network reconnection

UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASSWORD);

FirebaseApp app;

#if defined(ESP32) || defined(ESP8266) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFiClientSecure.h>
WiFiClientSecure ssl_client;
#elif defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_UNOWIFIR4) || defined(ARDUINO_GIGA) || defined(ARDUINO_PORTENTA_C33) || defined(ARDUINO_NANO_RP2040_CONNECT)
#include <WiFiSSLClient.h>
WiFiSSLClient ssl_client;
#endif

using AsyncClient = AsyncClientClass;

AsyncClient aClient(ssl_client, getNetwork(network));

RealtimeDatabase Database;

AsyncResult aResult_no_callback;

RTCTime currentTime;

/* --- Firebase Setup --- */

/* 변수 */
  int temperature;      //온도  
  int humidity;         //습도
  float soilMoisture;     //토양 습도
  float water_gauge;       //초음파
  int hours, minutes, seconds, year, dayofMon;
  String dayofWeek, month;

/* 변수 */



void setup() {


/* --- WiFi Setup --- */

  //직렬을 초기화하고 포트가 열릴 때까지 기다립니다.
  Serial.begin(9600);
  RTC.begin();  
  while (!Serial) {
    ; // 직렬 포트가 연결될 때까지 기다립니다. 기본 USB 포트에만 필요
  }
  // 와이파이를 연결합니다.
  connectWifi();

/* --- WiFi Setup --- */

/* --- Sensor Setup --- */
  // 온습도
  // 토양 습도
  // 초음파 센서 (물탱크 물양 체크)
  pinMode(ULTRASONIC_TRIG, OUTPUT);
  pinMode(ULTRASONIC_ECHO, INPUT);
/* --- Sensor Setup --- */


/* --- Actuator Setup --- */
  pinMode(FAN, OUTPUT);
  pinMode(HEATER, OUTPUT);
  pinMode(HUMIDIFIER, OUTPUT);
  pinMode(WATERPUMP1, OUTPUT);
  pinMode(WATERPUMP2, OUTPUT);
  pinMode(RELAYLED, OUTPUT);
  pinMode(neoLED, OUTPUT);
  digitalWrite(WATERPUMP1, HIGH);
  digitalWrite(WATERPUMP2, HIGH);
  digitalWrite(RELAYLED, HIGH);
/* --- Actuator Setup --- */

/* --- LED Setup --- */
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.setPixelColor(0, 0, 255, 0);          //  Neopixel 색상 설정 ( 첫번째 소자위치 , 색상설정(Red) , 0 , 0 )
  strip.setPixelColor(1, 0, 255, 0);          //  ( 두번째 소자위치 , 0 , 색상설정(Green) , 0 )
  strip.show();
/* --- LED Setup --- */

/* --- RTC Setup --- */
    
  String timeStamp = __TIMESTAMP__;
  getCurTime(timeStamp,&dayofWeek,&dayofMon,&month,&hours,&minutes,&seconds,&year);
  RTCTime startTime(dayofMon, convertMonth(month) , year, hours, minutes, seconds, 
                    convertDOW(dayofWeek), SaveLight::SAVING_TIME_ACTIVE); 
  RTC.setTime(startTime);
/* --- RTC Setup --- */


}

void loop() {
    
    authHandler();
    Database.loop();
    RTC.getTime(currentTime);

    int result = dht11.readTemperatureHumidity(temperature, humidity);
    int soilMoisture = analogRead(SOILMOISTURE);
    int waterLevel = analogRead(WATERLEVEL);
    if (result == 0) {
      setHumidity(humidity);
      setTemperature(temperature);
    } else {
      // Print error message based on the error code.
      Serial.println(DHT11::getErrorString(result));
    }
    
    setSoilMoisture(soilMoisture);
    setWaterTankValue();
    soilMoisture = getSoilMoisture();
    water_gauge = getWaterTankValue();

    // 모바일에서 설정한 환경값을 함수를 통해 읽어와 펜모터, 히터 작동
    if(temperature > getTargetTemperature() || humidity > getTargetHumidity() ){
      onFan();
    } else {
      offFan();
    }
      
    if(temperature < getTargetTemperature()){
      onHeater();
    } else{
      offHeater();
    }
    
    // 아두이노에서 측정하는 센서값을 시리얼 모니터에 출력
    Serial.print("물컵 물의 양 : ");
    Serial.println(waterLevel);
    Serial.print("온도 : ");
    Serial.println(temperature);
    Serial.print("습도 : ");
    Serial.println(humidity);
    Serial.print("현재 시간 : ");
    Serial.println(getCurrentTime());

    // 물컵 물양이 줄어들면 워터펌프1 작동
    if(waterLevel <  500){
      onWaterPump1();
    }
    // 토양이 건조해지면 워터펌프2 작동
    if(soilMoisture < 760){
      onWaterPump2();
    }

    // 모바일 설정 시간에 맞춰 LED 켜고 끄기
    if(getCurrentTime() == getEndTime().toInt() && getLedTrigger() == true){
      offLed();
    } else if(getLedTrigger() == true){
      onLed();
    } else if(getLedTrigger() == false){
      offLed();
    }
    if(getCurrentTime() == getStartTime().toInt() && getLedTrigger() == false){
      onLed();
    }
    

    // 가습기 모듈 추후 추가 예정
    // if(humidity < getTargetHumidity()){
    //   onHumidifier();
    // } else
    // {
    //   offHumidifier();
    // }
}


/* --- 메소드 작성 구간 --- */

// 온도값 데이터베이스에 저장 함수
void setTemperature(float temperature){
  // 온도 값을 읽어 데이터베이스에 저장
  // Set float
  status = Database.set<number_t>(aClient, "/temp", number_t(temperature, 2));
  if (status)
    Serial.println("Set temperature is ok");
  else
    printError(aClient.lastError().code(), aClient.lastError().message());
}

// 습도값 데이터베이스에 저장 함수
void setHumidity(float humidity){
  // 습도 값을 읽어 데이터베이스에 저장
  // Set float
  status = Database.set<number_t>(aClient, "/humidity", number_t(humidity, 2));
  if (status)
    Serial.println("Set humidity is ok");
  else
    printError(aClient.lastError().code(), aClient.lastError().message());

}

// 토양습도값 데이터베이스에 저장 함수
void setSoilMoisture(float soilmoisture){
  // 토양 수분 값을 읽어 데이터베이스에 저장
  soilMoisture = map(soilmoisture, 0, 1023, 0, 100);
  status = Database.set<number_t>(aClient, "/soilMoisture", number_t(100 - soilMoisture, 2));
  if (status)
    Serial.println("Set soilmoisture is ok");
  else
    printError(aClient.lastError().code(), aClient.lastError().message());

}

// 초음파센서 거리값 데이터베이스에 저장 함수
float setWaterTankValue(){
  // 초음파센서 값을 읽어 데이터베이스에 저장
  float cycletime;
  float distance;
  float WaterTankValue;
  
  digitalWrite(ULTRASONIC_TRIG, HIGH);
  delay(10);
  digitalWrite(ULTRASONIC_TRIG, LOW);
  
  cycletime = pulseIn(ULTRASONIC_ECHO, HIGH); 
  
  distance = ((340 * cycletime) / 10000) / 2;

  WaterTankValue = map(distance, 0, 14, 0, 100);
                        //측정값의 최소, 측정값 최대, 0, 100

  status = Database.set<number_t>(aClient, "/water_gauge", number_t(1.00 - WaterTankValue/100, 2));
  if (status){
    //Serial.println(WaterTankValue);
    Serial.print("Set water_gauge is ok : ");
    Serial.println(1.00 - WaterTankValue/100);
  }
  else
    printError(aClient.lastError().code(), aClient.lastError().message());
  
  return WaterTankValue;
}

// 온도값 데이터베이스에서 불러오기
float getTargetTemperature(){

  Serial.print("Get temperature... ");
  float db_TargetTemperature = Database.get<double>(aClient, "/target_temp");
  if (aClient.lastError().code() == 0)
    Serial.println(db_TargetTemperature);
  else
    printError(aClient.lastError().code(), aClient.lastError().message());

  return db_TargetTemperature;
}

// 습도값 데이터베이스에서 불러오기
float getTargetHumidity(){

  Serial.print("Get TargetHumidity... ");
  float db_TargetHumidity = Database.get<double>(aClient, "/target_humidity");
  if (aClient.lastError().code() == 0)
    Serial.println(db_TargetHumidity);
  else
    printError(aClient.lastError().code(), aClient.lastError().message());

  return db_TargetHumidity;
}


// 습도값 데이터베이스에서 불러오기
float getHumidity(){

  Serial.print("Get humidity... ");
  float db_Humidity = Database.get<double>(aClient, "/humidity");
  if (aClient.lastError().code() == 0)
    Serial.println(db_Humidity);
  else
    printError(aClient.lastError().code(), aClient.lastError().message());

  return db_Humidity;
}

//토양습도값 데이터베이스에에서 불러오기
float getSoilMoisture(){

    Serial.print("Get SoilMoisture... ");
  float db_SoilMoisture = Database.get<double>(aClient, "/soilMoisture");
  if (aClient.lastError().code() == 0)
    Serial.println(db_SoilMoisture);
  else
    printError(aClient.lastError().code(), aClient.lastError().message());

  return db_SoilMoisture;
}

// 초음파센서 거리값 데이터베이스에서 불러오기
float getWaterTankValue(){

    Serial.print("Get water_gauge... ");
  float db_WaterTankValue = Database.get<double>(aClient, "/water_gauge");
  if (aClient.lastError().code() == 0)
    Serial.println(db_WaterTankValue);
  else
    printError(aClient.lastError().code(), aClient.lastError().message());

  return db_WaterTankValue;
}

// 현재 시간 받아오기
int getCurrentTime(){

    int hour = currentTime.getHour();             //시
    int min = currentTime.getMinutes();           //분
    int time = (hour * 100) + min;

    return time;
}

// 모바일에서 설정한 LED 켜지는 시간 불러오기
String getStartTime(){

    Serial.print("Get start time... ");
  String db_start_time = Database.get<String>(aClient, "/led_start_time");
  db_start_time.replace(":","");
  if (aClient.lastError().code() == 0){
    Serial.println(db_start_time);
  }
  else
    printError(aClient.lastError().code(), aClient.lastError().message());

  return db_start_time;
}

// 모바일에서 설정한 LED 꺼지는 시간 불러오기
String getEndTime(){

    Serial.print("Get end time... ");
  String db_end_time = Database.get<String>(aClient, "/led_end_time");
  db_end_time.replace(":","");
  if (aClient.lastError().code() == 0){
    Serial.println(db_end_time);
  }
  else
    printError(aClient.lastError().code(), aClient.lastError().message());

  return db_end_time;
}


bool getLedTrigger(){
    Serial.print("Get bool... ");
    bool ledTrigger = Database.get<bool>(aClient, "/led_trigger");
    if (aClient.lastError().code() == 0)
        Serial.println(ledTrigger);
    else
        printError(aClient.lastError().code(), aClient.lastError().message());

    return ledTrigger;
}

// LED 켜기
void onLed(){
  digitalWrite(RELAYLED, LOW);
   strip.show();
  status = Database.set<bool>(aClient, "/led_trigger", true);
  if (status)
    Serial.println("LED is on!");
  else
    printError(aClient.lastError().code(), aClient.lastError().message());

}

// LED 끄기
void offLed(){
  status = Database.set<bool>(aClient, "/led_trigger", false);
  if (status)
    Serial.println("LED is off!");
  else
    printError(aClient.lastError().code(), aClient.lastError().message());
  digitalWrite(RELAYLED, HIGH);
}

// 펜 켜기
void onFan(){
  analogWrite(FAN, 255);
  status = Database.set<bool>(aClient, "/current_fan", true);
  if (status)
    Serial.println("Fan is on!");
  else
    printError(aClient.lastError().code(), aClient.lastError().message());
}

// 펜 끄기
void offFan(){

  analogWrite(FAN, 0);
  status = Database.set<bool>(aClient, "/current_fan", false);
  if (status)
    Serial.println("Fan is off!");
  else
    printError(aClient.lastError().code(), aClient.lastError().message());
}

// 히터 켜기
void onHeater(){
  analogWrite(HEATER, 60);
  status = Database.set<bool>(aClient, "/current_heater", true);
  if (status)
    Serial.println("Heater is on!");
  else
    printError(aClient.lastError().code(), aClient.lastError().message());
}

//히터 끄기
void offHeater(){
  analogWrite(HEATER, 0);
  status = Database.set<bool>(aClient, "/current_heater", false);
  if (status)
    Serial.println("Heater is off!");
  else
    printError(aClient.lastError().code(), aClient.lastError().message());
}

// 가습기 켜기
// void onHumidifier(){
//   digitalWrite(HUMIDIFIER, HIGH);
//   status = Database.set<bool>(aClient, "/current_humidifier", true);
//   if (status)
//     Serial.println("Humidifier is on!");
//   else
//     printError(aClient.lastError().code(), aClient.lastError().message());
// }

// 가습기 끄기
// void offHumidifier(){
//   digitalWrite(HUMIDIFIER, LOW);
//   status = Database.set<bool>(aClient, "/current_humidifier", false);
//   if (status)
//     Serial.println("Humidifier is off!");
//   else
//     printError(aClient.lastError().code(), aClient.lastError().message());
// }

//물컵 물양유지 워터펌프
void onWaterPump1(){
  digitalWrite (WATERPUMP1, LOW);
  delay(3000);
  digitalWrite (WATERPUMP1, HIGH);
}

//토양 수분유지 워터펌프
void onWaterPump2(){
  digitalWrite (WATERPUMP2, LOW);
  delay(3000);
  digitalWrite (WATERPUMP2, HIGH);
}

//RTC 메소드
DayOfWeek convertDOW(String dow){
  if (dow == String("Mon")) return DayOfWeek::MONDAY;
  if (dow == String("Tue")) return DayOfWeek::TUESDAY;
  if (dow == String("Wed")) return DayOfWeek::WEDNESDAY;
  if (dow == String("Thu")) return DayOfWeek::THURSDAY;
  if (dow == String("Fri")) return DayOfWeek::FRIDAY;
  if (dow == String("Sat")) return DayOfWeek::SATURDAY;
  if (dow == String("Sun")) return DayOfWeek::SUNDAY;
}

Month convertMonth(String m){
 if (m == String("Jan")) return Month::JANUARY;
  if (m == String("Feb")) return Month::FEBRUARY;
  if (m == String("Mar")) return Month::MARCH;
  if (m == String("Apr")) return Month::APRIL;
  if (m == String("May")) return Month::MAY;
  if (m == String("Jun")) return Month::JUNE;
  if (m == String("Jul")) return Month::JULY;
  if (m == String("Aug")) return Month::AUGUST;
  if (m == String("Sep")) return Month::SEPTEMBER;
  if (m == String("Oct")) return Month::OCTOBER;
  if (m == String("Nov")) return Month::NOVEMBER;
  if (m == String("Dec")) return Month::DECEMBER;
}

void getCurTime(String timeSTR,String* d_w,int* d_mn, String* mn,int* h,int* m,int* s,int* y){
  
  *d_w = timeSTR.substring(0,3);
  *mn = timeSTR.substring(4,7);
  *d_mn = timeSTR.substring(8,11).toInt();
  *h = timeSTR.substring(11,13).toInt();
  *m = timeSTR.substring(14,16).toInt();
  *s = timeSTR.substring(17,19).toInt();
  *y = timeSTR.substring(20,24).toInt();

}

// 와이파이, 데이터베이스 연결 및 에러 코드 출력 함수
void connectWifi(){
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);

    Serial.println("Initializing app...");

#if defined(ESP32) || defined(ESP8266) || defined(PICO_RP2040)
    ssl_client.setInsecure();
#if defined(ESP8266)
    ssl_client.setBufferSizes(4096, 1024);
#endif
#endif

    initializeApp(aClient, app, getAuth(user_auth), aResult_no_callback);

    authHandler();

    app.getApp<RealtimeDatabase>(Database);

    Database.url(DATABASE_URL);

    // In case setting the external async result to the sync task (optional)
    // To unset, use unsetAsyncResult().
    aClient.setAsyncResult(aResult_no_callback);

    Serial.println("Synchronous Get... ");

    // Library does not provide JSON parser library, to get JSON object, get the string and deseialize it.

    
}

void authHandler()
{
    // Blocking authentication handler with timeout
    unsigned long ms = millis();
    while (app.isInitialized() && !app.ready() && millis() - ms < 120 * 1000)
    {
        // The JWT token processor required for ServiceAuth and CustomAuth authentications.
        // JWT is a static object of JWTClass and it's not thread safe.
        // In multi-threaded operations (multi-FirebaseApp), you have to define JWTClass for each FirebaseApp,
        // and set it to the FirebaseApp via FirebaseApp::setJWTProcessor(<JWTClass>), before calling initializeApp.
        JWT.loop(app.getAuth());
        printResult(aResult_no_callback);
    }
}

void printResult(AsyncResult &aResult)
{
    if (aResult.isEvent())
    {
        Firebase.printf("Event task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.appEvent().message().c_str(), aResult.appEvent().code());
    }

    if (aResult.isDebug())
    {
        Firebase.printf("Debug task: %s, msg: %s\n", aResult.uid().c_str(), aResult.debug().c_str());
    }

    if (aResult.isError())
    {
        Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());
    }
}

void printError(int code, const String &msg)
{
    Firebase.printf("Error, msg: %s, code: %d\n", msg.c_str(), code);
}

/* --- 메소드 작성 구간 --- */
