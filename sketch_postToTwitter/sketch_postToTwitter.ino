#include <Bridge.h>
#include <Process.h>
#include <Temboo.h>

/*
  Arduino Yunの検証プログラム
  AM2302 Temperature / Humidity Sensor (DHT22)で室内の温度・湿度情報を取得し、
  データを保存するため、Google DocsやTwitterへ配信する
  Tran Duc Quy　次世代システム研究室　GMOインターネット株式会社
*/
#include "DHT.h"          // DHT & AM2302センサーのライブラリ
#include "accounts.h"     //アカウント情報

// AM2302センサーへ接続するデータPin
#define DHTPIN 8 //センサーを接続しているピンの番号
#define DHTTYPE DHT22       // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);   // LED pins

// Variables
float humidity; //湿度
float temperature; //温度
unsigned long time; //時刻
int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 10;  // Maximum number of times the Choreo should be executed
//The Process commands allow you to run Linux processes on OpenWrt-Yun through the Arduino
Process date;

//設定
void setup() {
  // Setup serial monitor
  Serial.begin(9600);

  // Wait 3 seconds
  delay(3000);

  //AM2302の初期化
  dht.begin();
  
  //Brigdeを開始する
  Bridge.begin();

  //時刻を取得
  time = millis();
  //時刻を初期化
  if (!date.running()) {
    date.begin("date");
    date.addParameter("+%F-%T");
    date.run();
  }
}

//メイン処理
void loop() {

  //センサーから温度値を読み出します。読込み失敗なら"NAN"を返す
  humidity = dht.readHumidity();
  //センサーから湿度値を読み出します。読込み失敗なら"NAN"を返す
  temperature = dht.readTemperature();

  // 温度と湿度が取得できたかチェックする
  if (isnan(temperature) || isnan(humidity)) {
    //シリアルモニターに出力する
    Serial.println(F("Failed to read from DHT"));
  }
  else {
    //シリアルモニターに出力する
    printToSerialMonitor(temperature, humidity);
    
    //Twitterへデータを配信する
    postToTwitter(temperature, humidity);
  }
  Serial.println("Waiting to next post...");
  // 10分後に繰り返す
  delay(600000);
}

//シリアルモニターに出力する
void printToSerialMonitor(float temperature, float humidity) {
  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.print(F(" %\t"));
  Serial.print(F("Temperature: "));
  Serial.print(temperature);
  Serial.println(F(" C"));
}
