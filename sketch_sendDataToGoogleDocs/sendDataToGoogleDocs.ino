// Google Docsにデータを送信するメソッド
void sendToGoogleDocs(float temperature, float humidity) {
  // Format data
  String data = genGoogleDocsData(temperature, humidity);
  //Exe Temboo api
  runAppendRow(data);
}

//配信データを生成する
String genGoogleDocsData(float temperature, float humidity) {
  // Restart the date process:
  if (!date.running())  {
    date.begin("date");
    date.addParameter("+%F-%T");
    date.run();
  }

  // Get date
  String timeString = date.readString();

  // Format data
  String data =""; 
  data = data + timeString + "," + String(temperature) + "," + String(humidity);
  Serial.println("Upload to Google docs: " + data);
  return data;

}

//TembooのAPIを実行する
void runAppendRow(String data) {
  if (numRuns <= maxRuns) {
    Serial.println("Running AppendRow - Run #" + String(numRuns++));

    TembooChoreo AppendRowChoreo;

    // Invoke the Temboo client
    AppendRowChoreo.begin();

    // Set Temboo account credentials
    AppendRowChoreo.setAccountName(TEMBOO_ACCOUNT);
    AppendRowChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    AppendRowChoreo.setAppKey(TEMBOO_APP_KEY);

    // Set Choreo inputs
    AppendRowChoreo.addInput("ClientSecret", GOOGLE_DOCS_CLIENT_SECRET);
    AppendRowChoreo.addInput("RefreshToken", GOOGLE_DOCS_REFRESH_TOKEN);
    AppendRowChoreo.addInput("RowData", data);
    AppendRowChoreo.addInput("SpreadsheetTitle", SPREADSHEET_TITLE);
    AppendRowChoreo.addInput("ClientID", GOOGLE_DOCS_CLIENT_ID);

    // Identify the Choreo to run
    AppendRowChoreo.setChoreo("/Library/Google/Spreadsheets/AppendRow");

    // Run the Choreo; when results are available, print them to serial
    AppendRowChoreo.run();

    while (AppendRowChoreo.available()) {
      char c = AppendRowChoreo.read();
      Serial.print(c);
    }
    AppendRowChoreo.close();
  }
}
