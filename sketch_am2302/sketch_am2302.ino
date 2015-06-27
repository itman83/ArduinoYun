/*
  Arduino Yunの検証プログラム
  AM2302 Temperature / Humidity Sensor (DHT22)で室内の温度・湿度情報を取得する
  Tran Duc Quy　次世代システム研究室　GMOインターネット株式会社
*/
#include "DHT.h"          // DHT & AM2302センサーのライブラリ

// AM2302センサーへ接続するデータPin
#define DHTPIN 8 //センサーを接続しているピンの番号
#define DHTTYPE DHT22       // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);   // LED pins

// Variables
float humidity;
float temperature;

//設定
void setup() {
  // Setup serial monitor
  Serial.begin(9600);

  // Wait 3 seconds
  delay(3000);

  //AM2302の初期化
  dht.begin();
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
  }

  // 3秒後に繰り返す
  delay(3000);
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
