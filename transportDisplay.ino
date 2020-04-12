/**
 *  Main program file. Contains all logic.
 *  Copyright (C) 2020 PixelSergey, hax0kartik
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

// The project uses the following LiquidCrystal_I2C library:
// https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// NodeMCU uses 0x3F as the address
// Set screen to width 16 and height 2
LiquidCrystal_I2C lcd(0x3F, 16, 2);
HTTPClient http;
HTTPClient http2;
WiFiClient wclient;
WiFiClient wclient2;

// Request text easily converted to this format using https://tomeko.net/online_tools/cpp_text_escape.php?lang=en
char request[] =
"{\n"
"  stop(id: \"HSL:1291402\") {\n"
"    gtfsId\n"
"    name\n"
"    stoptimesWithoutPatterns(numberOfDepartures: 1){\n"
"      serviceDay\n"
"      scheduledArrival\n"
"      scheduledDeparture\n"
"      realtimeArrival\n"
"      realtimeDeparture\n"
"      trip {\n"
"        route {\n"
"          shortName\n"
"        }\n"
"      }\n"
"    }\n"
"  }\n"
"}";

void setup(){
    Serial.begin(9600);
    
    lcd.begin();
    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print("Connecting to");
    lcd.setCursor(0, 1);
    lcd.print(WIFI_SSID);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    char* loading[] = {"-", "/", "|", "/"};    
    int i = 0;
    while(WiFi.status() != WL_CONNECTED){
        lcd.setCursor(15, 1);
        lcd.print(loading[i]);
        i++;
        if(i>=4) i=0;
        delay(400);
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connected!");

    delay(1000);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connecting to");
    lcd.setCursor(0, 1);
    lcd.print("HSL...");

    http2.begin(wclient2, "http://api.digitransit.fi/routing/v1/routers/hsl/index/graphql");
    http2.addHeader("Content-Type", "application/graphql");
    int httpCode2 = http2.POST(request);
    String payload2 = http2.getString();
    Serial.println(httpCode2);
    Serial.println(payload2);
    http2.end();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Received data!");
    lcd.setCursor(0, 1);
    lcd.print("Check serial");

    const size_t capacity = JSON_ARRAY_SIZE(1) + 4*JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 200;
    DynamicJsonDocument doc(capacity);
    const char* json = payload2.c_str();
    deserializeJson(doc, json);
    JsonObject data_stop = doc["data"]["stop"];    
    JsonObject times = data_stop["stoptimesWithoutPatterns"][0];
    long serviceDay = times["serviceDay"]; // 1586638800
    long scheduledArrival = times["scheduledArrival"]; // 41520
    long scheduledDeparture = times["scheduledDeparture"]; // 41520
    long realtimeArrival = times["realtimeArrival"]; // 41520
    long realtimeDeparture = times["realtimeDeparture"]; // 41520
    const char* route = times["trip"]["route"]["shortName"]; // "10"
    Serial.println(serviceDay);
    Serial.println(scheduledArrival);
    Serial.println(scheduledDeparture);
    Serial.println(realtimeArrival);
    Serial.println(realtimeDeparture);
    Serial.println(route);
}

void loop(){
    http.begin(wclient, "http://worldtimeapi.org/api/ip");
    //http.addHeader("Accept", "application/json");
    //http.addHeader("Accept-Charset", "utf-8");
    int httpCode = http.GET();
    String payload = http.getString();
    Serial.println(httpCode);
    Serial.println(payload);
    http.end();
    const size_t capacity = JSON_OBJECT_SIZE(15) + 350;
    DynamicJsonDocument doc(capacity);
    const char* json = payload.c_str();
    deserializeJson(doc, json);
    long unixtime = doc["unixtime"];
    Serial.println(unixtime);
    delay(3000);
}
