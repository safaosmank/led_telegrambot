
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>
// Replace with your network credentials
const char* ssid = "";
const char* password = "";
// Initialize Telegram BOT
#define BOTtoken ""  // your Bot Token (Get from Botfather)
//#define CHAT_ID ""
#define CHAT_ID ""
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;
const int blueledPin = 27;
const int greenledPin = 26;

bool blueledState = LOW;
bool greenledState = LOW;
// Handle new receive messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
      }
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);
    String from_name = bot.messages[i].from_name;
    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/blueled_on to turn GPIO ON \n";
      welcome += "/blueled_off to turn GPIO OFF \n";
      welcome += "/greenled_on to turn GPIO ON \n";
      welcome += "/greenled_off to turn GPIO OFF \n";
      welcome += "/state to request current GPIO state \n";
      bot.sendMessage(chat_id, welcome, "");
    }
    if (text == "/blueled_on") {
      bot.sendMessage(chat_id, "blueLED state set to ON", "");
      blueledState = HIGH;
      digitalWrite(blueledPin, blueledState);
    }
    if (text == "/blueled_off") {
      bot.sendMessage(chat_id, "blueLED state set to OFF", "");
      blueledState = LOW;
      digitalWrite(blueledPin, blueledState);
    }
    if (text == "/greenled_on") {
      bot.sendMessage(chat_id, "greenLED state set to ON", "");
      greenledState = HIGH;
      digitalWrite(greenledPin, greenledState);
    }
    if (text == "/greenled_off") {
      bot.sendMessage(chat_id, "greenLED state set to OFF", "");
      greenledState = LOW;
      digitalWrite(greenledPin, greenledState);
    }
    if (text == "/state") {
      if (digitalRead(blueledPin)){
        bot.sendMessage(chat_id, "blueLED is ON", "");
      } 
      else{
      bot.sendMessage(chat_id, "blueLED is OFF", "");}
      if (digitalRead(greenledPin)){
        bot.sendMessage(chat_id, "greenLED is ON", "");
      }
      else
        //bot.sendMessage(chat_id, "blueLED is OFF", "");
       {
        bot.sendMessage(chat_id, "greenLED is OFF", "");
       }
      
    }
}
}
void setup() {
  Serial.begin(115200);
  #ifdef ESP8266
    client.setInsecure();
 #endif
  pinMode(blueledPin, OUTPUT);
  pinMode(greenledPin, OUTPUT);
  digitalWrite(blueledPin, blueledState);
  digitalWrite(greenledPin, greenledState);
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    
    Serial.println("Connecting to WiFi.."); 
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}
void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}    
