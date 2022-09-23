/****************************************************************************************************************************
  Async_HttpBasicAuth.ino
  
  For RP2040W with CYW43439 WiFi
  
  AsyncWebServer_RP2040W is a library for the RP2040W with CYW43439 WiFi
  
  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncWebServer_RP2040W
  Licensed under GPLv3 license
 *****************************************************************************************************************************/

// See the list of country codes in
// https://github.com/earlephilhower/cyw43-driver/blob/02533c10a018c6550e9f66f7699e21356f5e4609/src/cyw43_country.h#L59-L111
// To modify https://github.com/earlephilhower/arduino-pico/blob/master/variants/rpipicow/picow_init.cpp
// Check https://github.com/khoih-prog/AsyncWebServer_RP2040W/issues/3#issuecomment-1255676644

#if !( defined(ARDUINO_RASPBERRY_PI_PICO_W) )
  #error For RASPBERRY_PI_PICO_W only
#endif

#define _RP2040W_AWS_LOGLEVEL_     1

///////////////////////////////////////////////////////////////////

#include <pico/cyw43_arch.h>

///////////////////////////////////////////////////////////////////

#include <AsyncWebServer_RP2040W.h>

char ssid[] = "your_ssid";        // your network SSID (name)
char pass[] = "12345678";         // your network password (use for WPA, or use as key for WEP), length must be 8+

int status = WL_IDLE_STATUS;

AsyncWebServer    server(80);

const char* www_username = "admin";
const char* www_password = "rp2040w";

void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("Local IP Address: ");
  Serial.println(ip);

  // print your board's country code 
  // #define CYW43_COUNTRY(A, B, REV) ((unsigned char)(A) | ((unsigned char)(B) << 8) | ((REV) << 16))
  uint32_t myCountryCode = cyw43_arch_get_country_code(); 
  char countryCode[3] = { 0, 0, 0 };
  
  countryCode[0] = myCountryCode & 0xFF;
  countryCode[1] = (myCountryCode >> 8) & 0xFF;

  Serial.print("Country code: "); Serial.println(countryCode);
}

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  delay(200);

  Serial.print("\nStart Async_HTTPBasicAuth on "); Serial.print(BOARD_NAME);
  Serial.print(" with "); Serial.println(SHIELD_TYPE);
  Serial.println(ASYNCTCP_RP2040W_VERSION);
  Serial.println(ASYNC_WEBSERVER_RP2040W_VERSION);

  ///////////////////////////////////
  
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  Serial.print(F("Connecting to SSID: "));
  Serial.println(ssid);

  status = WiFi.begin(ssid, pass);

  delay(1000);
   
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED)
  {
    delay(500);
        
    // Connect to WPA/WPA2 network
    status = WiFi.status();
  }

  printWifiStatus();
  
  ///////////////////////////////////

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    if (!request->authenticate(www_username, www_password))
    {
      return request->requestAuthentication();
    }
    
    request->send(200, "text/plain", "Login OK");
  });

  server.begin();

  Serial.print(F("Async_HttpBasicAuth started @ IP : "));
  Serial.println(WiFi.localIP());

  Serial.print(F("Open http://"));
  Serial.print(WiFi.localIP());
  Serial.println(F("/ in your browser to see it working"));

  Serial.print(F("Login using username = "));
  Serial.print(www_username);
  Serial.print(F(" and password = "));
  Serial.println(www_password);
}

void heartBeatPrint()
{
  static int num = 1;

  Serial.print(F("."));

  if (num == 80)
  {
    Serial.println();
    num = 1;
  }
  else if (num++ % 10 == 0)
  {
    Serial.print(F(" "));
  }
}

void check_status()
{
  static unsigned long checkstatus_timeout = 0;

#define STATUS_CHECK_INTERVAL     10000L

  // Send status report every STATUS_REPORT_INTERVAL (60) seconds: we don't need to send updates frequently if there is no status change.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    heartBeatPrint();
    checkstatus_timeout = millis() + STATUS_CHECK_INTERVAL;
  }
}

void loop()
{
  check_status();
}
