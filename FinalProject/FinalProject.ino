/* 
    Title : 넌 내꺼야! 나만의 비바리움 스마트 사육장
    IoT 시스템응용및프로그래밍 Final Project
    Member : 박성재, 김서율, 김지찬, 유지석
    Date : 2024.05.10 ~
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

#include <FirebaseClient.h>       //FirebaseClient 라이브러리
#include "arduino_secrets.h"      // 와이파이 SSID, PASSWORD 값이 적힌 헤더파일
#include <DHT11.h>
DHT11 dht11(2);

//아두이노 핀 정하기
#define DHT11_PIN 
#define SOILMOISTURE_PIN
#define ULTRASONIC_PIN
#define RELAY_1
#define RELAY_2
#define RELAY_3
#define RELAY_4

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

/* --- Firebase Setup --- */

/* 변수 */
  int temperature;      //온도  
  int humidity;         //습도
  float soilMoisture;     //토양 습도
  float water_gauge;       //초음파


/* 변수 */

void setup() {


/* --- WiFi Setup --- */

  //직렬을 초기화하고 포트가 열릴 때까지 기다립니다.
  Serial.begin(9600);
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
/* --- Sensor Setup --- */


/* --- Actuator Setup --- */
  // 릴레이 (가습기 모듈, 적외선 램프, LED, 팬)
  // 
/* --- Actuator Setup --- */




}

void loop() {
  // 1. setup에서 WiFi 연결 완
  // 2. 온도, 습도, 토양 습도, 초음파 거리 값 읽기
  // 3. 데이터베이스에 센서를 통해 읽어온 온도, 습도, 토양 습도, 초음파 거리 값 저장
  // 4. 조건에 따라 엑츄에이터들 작동
  // 5. 데이터베이스에서 보내온 값을 읽고 엑츄에티어 작동 조건에 반영 or 엑추에이터 작동 or ---
  // 6. 실시간 시간 값을 받아와 LED On/Off
    authHandler();
    Database.loop();

    //센서값 데이터베이스에 저장
    int result = dht11.readTemperatureHumidity(temperature, humidity);
    if (result == 0) {
      setHumidity(humidity);
      setTemperature(temperature);
    } else {
      // Print error message based on the error code.
      Serial.println(DHT11::getErrorString(result));
    }
    Serial.println("------실시간 센서 값------");
    humidity = getHumidity();
    temperature = getTemperature();
    soilMoisture = getSoilMoisture();
    water_gauge = getWaterTankValue();
    Serial.println("-----------------------");
    Serial.println("");
    Serial.println("------모바일 설정 값------");
    Serial.println(get_moblie_humidity());
    Serial.println(get_moblie_temp());
    Serial.println("-----------------------");
    Serial.println("");
    delay(5000);
}


/* --- 메소드 작성 구간 --- */

// 온도값 읽기 함수
// float readTemperature(){
//   // 온도 값을 읽어 
//   float temperature = dht11.readTemperature();
//   return temperature;
// }

// // 습도값 읽기 함수
// float readHumidity(){
//   // 습도 값을 읽어 
//   float humidity = dht11.readHumidity();
//   return humidity;
// }

// // 토양습도값 읽기 함수
// float readSoilMoisture(){
//   // 토양 수분 값을 읽어 
//   int moisturesensorValue = asnalogRead(a1);
//   int sensorMin = 0;  // 센서로부터 읽은 최소값
//   int sensorMax = 1023; // 센서로부터 읽은 최대값
//   int soilMoisture = map(moisturesensorValue, sensorMin, sensorMax, 0, 100);
//   return soilMoisture;
// }

// // 초음파 센서 거리 읽기 함수
// float readWaterTankValue(){
  
//   float cycletime;
//   float distance;
//   float WaterTankValue;
  
//   digitalWrite(ULTRASONIC_TRIG, HIGH);
//   delay(10);
//   digitalWrite(ULTRASONIC_TRIG, LOW);
  
//   cycletime = pulseIn(ULTRASONIC_ECHO, HIGH); 
  
//   distance = ((340 * cycletime) / 10000) / 2;

//   WaterTankValue = map(distance, 0, 20, 0, 100);
//                                 //측정값의 최소, 측정값 최대, 0, 100
  
//   return WaterTankValue;
// }

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
void setSoilMoisture(float soilMoisture){
  // 토양 수분 값을 읽어 데이터베이스에 저장
}

// 초음파센서 거리값 데이터베이스에 저장 함수
void setWaterTankValue(float waterTankValue){
  // 초음파센서 값을 읽어 데이터베이스에 저장
}

// 온도값 데이터베이스에서 불러오기
float getTemperature(){

  Serial.print("Get temperature... ");
  float db_Temperature = Database.get<double>(aClient, "/temp");
  if (aClient.lastError().code() == 0)
    Serial.println(db_Temperature);
  else
    printError(aClient.lastError().code(), aClient.lastError().message());

  return db_Temperature;
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
  float db_SoilMoisture = Database.get<double>(aClient, "/humidity_GND");
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

// 모바일에서 설정한 습도 값 (조건문 안에 들어가는)
float get_moblie_humidity(){

    Serial.print("Get target humidity... ");
  float db_target_humidity = Database.get<double>(aClient, "/target_humidity");
  if (aClient.lastError().code() == 0)
    Serial.print("");
  else
    printError(aClient.lastError().code(), aClient.lastError().message());

  return db_target_humidity;
}

// 모바일에서 설정한 온도 값 (조건문 안에 들어가는)
float get_moblie_temp(){

    Serial.print("Get target temp... ");
  float db_target_temp = Database.get<double>(aClient, "/target_temp");
  if (aClient.lastError().code() == 0)
    Serial.print("");
  else
    printError(aClient.lastError().code(), aClient.lastError().message());

  return db_target_temp;
}

// float getSetTime(num){
//   int setTime[] = [a, b];
  
//   setTime[0] = ~~~  //켜지는 시간
//   setTime[1] = ~~~  // 꺼지는 시간
//   if(num == 0){
//     return setTime[0];
//   }else if(num == 1){
//     return setTime[1];
//   }
// }

// void currentTime(){
//   // wifi 현재 시간
//   return currentTime;
// }

// void onLed(){
//   digitalWrite (RELAY_Led, HIGH);
//   // LED 켜기 (릴레이 HIGH)
// }

// void offLed(){
//   digitalWrite (RELAY_Led, LOW);
//   // LED 끄기 (릴레이 LOW)
// }

// void onFan(){
//   digitalWrite (RELAY_Fan, HIGH);
//   // 펜 켜기 (릴레이 HIGH)
// }

// void offFan(){
//   digitalWrite (RELAY_Fan, LOW);
//   // 펜 끄기 (릴레이 LOW)
// }

// void onUVB(){
//   digitalWrite (RELAY_UVB, HIGH);
// }

// void offUVB(){
//   digitalWrite (RELAY_UVB, LOW);
// }

// void onWaterPump(){
//   digitalWrite (RELAY_WaterPump, HIGH);
// }

// void offWaterPump(){
//   digitalWrite (RELAY_WaterPump, LOW);
// }

//센서값에 의존하지 않고 모바일에서 요청하면 엑츄에이터를 작동시키는 함수들 작성


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