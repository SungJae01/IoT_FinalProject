/* 
    Title : 넌 내꺼야! 나만의 비바리움 스마트 사육장
    IoT 시스템응용및프로그래밍 Final Project
    Member : 박성재, 김서율, 김지찬, 유지석
    Date : 2024.05.10 ~
*/

#include "WiFiS3.h"               // 와이파이 라이브러리
#include "arduino_secrets.h"      // 와이파이 SSID, PASSWORD 값이 적힌 헤더파일

//아두이노 핀 정하기
#define TEMPERATURE_PIN 
#define HUMIDITY_PIN
#define SOILMOISTURE_PIN
#define ULTRASONIC_PIN
#define RELAY_1
#define RELAY_2
#define RELAY_3
#define RELAY_4

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(80);

void setup() {
/* 변수 */
  float temperature;      //온도  
  float humidity;         //습도
  float soilMoisture;     //토양 습도
  float ultrasonic;       //초음파


/* 변수 */

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


/* --- MySQL Setup --- */
  //여기는 데이터베이스 Setup 구간

/* --- MySQL Setup --- */

}

void loop() {
  1. setup에서 WiFi 연결 완
  2. 온도, 습도, 토양 습도, 초음파 거리 값 읽기
  3. 데이터베이스에 센서를 통해 읽어온 온도, 습도, 토양 습도, 초음파 거리 값 저장
  4. 조건에 따라 엑츄에이터들 작동
  5. 데이터베이스에서 보내온 값을 읽고 엑츄에티어 작동 조건에 반영 or 엑추에이터 작동 or ---
  6. 실시간 시간 값을 받아와 LED On/Off

}


/* --- 메소드 작성 구간 --- */

// 온도값 읽기 함수
float readTemperature(){
  // 온도 값을 읽어 
  
  return temperature;
}

// 습도값 읽기 함수
float readHumidity(){
  // 습도 값을 읽어 
  
  return humidity;
}

// 토양습도값 읽기 함수
float readSoilMoisture(){
  // 토양 수분 값을 읽어 
  
  return soilMoisture;
}

// 초음파 센서 거리 읽기 함수
float readWaterTankValue(){
  // 물탱크 물양 값을 읽어 
  // 0 ~ 100% 값으로 변환해서
  return waterTankValue;
}

// 온도값 데이터베이스에 저장 함수
float setTemperature(float temperature){
  // 온도 값을 읽어 데이터베이스에 저장
}

// 습도값 데이터베이스에 저장 함수
float setHumidity(float humidity){
  // 습도 값을 읽어 데이터베이스에 저장
}

// 토양습도값 데이터베이스에 저장 함수
float setSoilMoisture(float soilMoisture){
  // 토양 수분 값을 읽어 데이터베이스에 저장
}

// 초음파센서 거리값 데이터베이스에 저장 함수
float setWaterTankValue(float waterTankValue){
  // 초음파센서 값을 읽어 데이터베이스에 저장
}

void onLed(){
  // LED 켜기 (릴레이 HIGH)
}

void offLed(){
  // LED 끄기 (릴레이 LOW)
}

void onFan(){
  // 펜 켜기 (릴레이 HIGH)
}

void offFan(){
  // 펜 끄기 (릴레이 LOW)
}

센서값에 의존하지 않고 모바일에서 요청하면 엑츄에이터를 작동시키는 함수들 작성


// 와이파이 연결 함수
void connectWifi(){
  // WiFi 모듈을 확인하십시오:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("WiFi 모듈과의 통신이 실패했습니다!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("펌웨어를 업그레이드해주세요");
  }

  // WiFi 네트워크에 연결을 시도합니다:
  while (status != WL_CONNECTED) {
    Serial.print("SSID에 연결을 시도하는 중: ");
    Serial.println(ssid);
    // WPA/WPA2 네트워크에 연결합니다. 개방형 또는 WEP 네트워크를 사용하는 경우 이 줄을 변경하십시오.
    status = WiFi.begin(ssid, pass);

    // 연결될 때까지 10초 동안 기다립니다.
    delay(10000);
  }
  // 들어오는 연결 수신을 시작하도록 서버에 지시합니다.
  server.begin();
  // 이제 연결되었으므로 상태를 인쇄하세요.
  printWifiStatus();
}

/* --- 메소드 작성 구간 --- */