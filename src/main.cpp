#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "ESP 32";
const char *password = "123456789";

IPAddress local_ip(192, 168, 49, 15);
IPAddress gateway(192, 168, 49, 1);
IPAddress subnet(255, 255, 255, 0);
WebServer server(80);

bool LedStatus = LOW;

int LED_PIN = 32;

// Function prototype for HTMLGui
String HTMLGui(int led);

// Event handler function for the root path
void event_OnConnect()
{
  server.send(200, "text/html", HTMLGui(LedStatus));
}

// Event handler function to turn LED on
void event_LedOn()
{
  LedStatus = HIGH;
  digitalWrite(LED_PIN, HIGH);
  Serial.println("Đèn LED bật");
  server.send(200, "text/html", HTMLGui(LedStatus));
}
void event_LedOff()
{
  LedStatus = LOW;
  digitalWrite(LED_PIN, LOW);
  Serial.println("Đèn LED tắt");
  server.send(200, "text/html", HTMLGui(LedStatus));
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  server.on("/", event_OnConnect);
  server.on("/ledon", event_LedOn);
  server.on("/ledoff", event_LedOff);

  server.begin();
  Serial.println("Server đang khởi động ...");
}

void loop()
{
  // 192.168.49.15/ : Connect
  // 192.168.49.15/ledon : Turn on LED
  // 192.168.49.15/ledoff : Turn off LED
  server.handleClient();
}

String HTMLGui(int led)
{
  String str = "<!DOCTYPE html>\n";
  str += "<html lang=\"en\">\n";
  str += "<head>\n";
  str += "<meta charset=\"UTF-8\">\n";
  str += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  str += "<title>ESP32</title>\n";
  str += "<style>\n";
  str += "html {text-align: center; margin: 0px auto;}\n";
  str += ".container {background-color: #20e9ede5; width: 300px; border-radius: 10px; margin: auto;}\n";
  str += "h1 {color: white; margin: 50px 30px;}\n";
  str += "p {color: red; font-size: 15px;}\n";
  str += ".button {display: block; width: 80px; color: white; padding: 13px 20px; text-decoration: none; font-size: 25px; margin: 0px auto 35px; cursor: pointer; border-radius: 4px;}\n";
  str += ".button-off {background-color: gray;}\n";
  str += ".button-off:active {background-color: black;}\n";
  str += ".button-on {background-color: green;}\n";
  str += ".button-on:active {background-color: red;}\n";
  str += "</style>\n";
  str += "</head>\n";
  str += "<body>\n";
  str += "<div class=\"container\">\n";
  str += "<h1>ESP32 WEBSERVER</h1>\n";
  if (led)
  {
    str += "<p>LED STATUS: ON</p><a href=\"/ledoff\" class=\"button button-on\">OFF</a>\n";
  }
  else
  {
    str += "<p>LED STATUS: OFF</p><a href=\"/ledon\" class=\"button button-off\">ON</a>\n";
  }
  str += "</div>\n";
  str += "</body>\n";
  str += "</html>\n";
  return str;
}
