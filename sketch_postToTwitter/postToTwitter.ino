//Twitterへデータを配信する
void postToTwitter(float temperature, float humidity)
{
  // only try to send the tweet if we haven't already sent it successfully
  if (numRuns <= maxRuns) {

    Serial.println("Running SendATweet - Run #" + String(numRuns++) + "...");

    // define the text of the tweet we want to send
    String tweetText("My Arduino Yun has been running for " + String(millis()) + " milliseconds. Data collected from AM2302 sensor: temperature=" +  String(temperature)  + " degree C, humidity=" +  String(humidity) + "%");
    //Output to Serial monitor
    Serial.println("Twist data=" + tweetText); 
    
    TembooChoreo StatusesUpdateChoreo;

    // invoke the Temboo client
    // NOTE that the client must be reinvoked, and repopulated with
    // appropriate arguments, each time its run() method is called.
    StatusesUpdateChoreo.begin();

    // set Temboo account credentials
    StatusesUpdateChoreo.setAccountName(TEMBOO_ACCOUNT);
    StatusesUpdateChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    StatusesUpdateChoreo.setAppKey(TEMBOO_APP_KEY);

    // add the Twitter account information
    StatusesUpdateChoreo.addInput("ConsumerKey", TWITTER_API_KEY);
    StatusesUpdateChoreo.addInput("ConsumerSecret", TWITTER_API_SECRET);
    StatusesUpdateChoreo.addInput("AccessToken", TWITTER_ACCESS_TOKEN);
    StatusesUpdateChoreo.addInput("AccessTokenSecret", TWITTER_ACCESS_TOKEN_SECRET);

    // and the tweet we want to send
    StatusesUpdateChoreo.addInput("StatusUpdate", tweetText);
        
    // identify the Temboo Library choreo to run (Twitter > Tweets > StatusesUpdate)
    StatusesUpdateChoreo.setChoreo("/Library/Twitter/Tweets/StatusesUpdate");
    
    // tell the Process to run and wait for the results. The
    // return code (returnCode) will tell us whether the Temboo client
    // was able to send our request to the Temboo servers
    unsigned int returnCode = StatusesUpdateChoreo.run();

    // a return code of zero (0) means everything worked
    if (returnCode == 0) {
      Serial.println("Success! Tweet sent!");
    } else {
      // a non-zero return code means there was an error
      // read and print the error message
      while (StatusesUpdateChoreo.available()) {
        char c = StatusesUpdateChoreo.read();
        Serial.print(c);
      }
    }
    StatusesUpdateChoreo.close();
  }
}
