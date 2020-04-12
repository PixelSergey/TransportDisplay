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
#include "rapidjson/document.h"

// NodeMCU uses 0x3F as the address
// Set screen to width 16 and height 2
LiquidCrystal_I2C lcd(0x3F, 16, 2);
HTTPClient http;
WiFiClient wclient;
rapidjson::Document doc;

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

    http.begin(wclient, "http://api.digitransit.fi/routing/v1/routers/hsl/index/graphql");
    http.addHeader("Content-Type", "application/graphql");
    int httpCode = http.POST(request);
    String payload = http.getString();
    Serial.println(httpCode);
    Serial.println(payload);
    http.end();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Received data!");
    lcd.setCursor(0, 1);
    lcd.print("Check serial");
    doc.Parse(json_str);
    const Value& data = doc["data"];
    const Value& stop = data["stop"];
    String name = stop["name"].GetString();
    Serial.println(name);
    const Value& stoptimesWithoutPatterns = stop["stoptimesWithoutPatterns"];
    int serviceDay = stoptimesWithoutPatterns[0]["serviceDay"].GetInt();
    Serial.println(serviceDay); 
    int scheduledArrival = stoptimesWithoutPatterns[0]["scheduledArrival"].GetInt();
    Serial.println(scheduledArrival);
    int realtimeArrival = stoptimesWithoutPatterns[0]["realtimeArrival"].GetInt();
    Serial.println(realtimeArrival);
    const Value& trip = stoptimesWithoutPatterns[0]["trip"];
    const Value& route = trip["route"];
    Serial.println(route["shortName"].GetString());

}

void loop(){
    
}
