#include <SPI.h>
#include <Ethernet.h>
#include <Flash.h>
#include <SD.h>
#include <TinyWebServer.h>
#include <OneWire.h>
#include "./pin.h"

#define DEBUG 1

boolean file_handler(TinyWebServer& web_server);
boolean index_handler(TinyWebServer& web_server);
boolean pins_handler(TinyWebServer& web_server);
boolean digital_pin_handler(TinyWebServer& web_server);
// Helpers
void parse_path_string(const char* str, int size, char **messages);
void get_request_data(TinyWebServer &web_server, char *str);

// Types of actions our Arduino can take
enum ActionTypes {
  GETTEMP
};

// Hardcoded host where the angular files are stored
const char *HOST = "10.0.1.2:9000";

// Pins we're interested in using
const int numPins = 3;
Pin pins[numPins] = {
  Pin(9, OUTPUT_T),
  Pin(8, OUTPUT_T),
  Pin(7, ONEWIRE),
};
// Our OneWire interface (the temp sensor)
OneWire ds(7);

// Our mac address
static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Our static IP
byte ip[] = { 10, 0, 1, 32 };

// Send the GET / index.html
boolean index_handler(TinyWebServer& web_server) {
  web_server.print(F("<html><head><title>Temperature sensor</title></head>"));
  web_server.print(F("<body>"));
  web_server.print(F("<script>window.ip=\""));
  web_server.print(ip_to_str(ip));
  web_server.print(F("\"</script>"));
  web_server.print(F("<script id=\"appscript\" src=\"http://"));
  web_server.print(HOST);
  web_server.print(F("/scripts/scripts.js\"></script>"));
  web_server.print(F("</body></html>"));
  return true;
}

// API
// GET /pins
boolean pins_handler(TinyWebServer& web_server) {
  web_server.send_error_code(200);
  web_server.send_content_type("application/javascript");
  web_server.end_headers();
  pinsToString(web_server);
  return true;
}

// POST /pins/update
boolean digital_pin_handler(TinyWebServer& web_server) {
    const char* action_str_len = web_server.get_header_value("X-Action-Len");
    int len = atoi(action_str_len);

    char* data = (char*)malloc(len);
    if (data) memset(data, 0, len);
    get_request_data(web_server, len, data);

    // DIGITAL
#if DEBUG
    Serial.print(F("digital_pin_handler: "));
    Serial.print(data);
    Serial.print(F(" ("));
    Serial.print(len);
    Serial.print(F(")\n"));
#endif

    int sLen = strlen(data);

    char ch;
    int valueInt, pinInt, actionInt, modeInt;
    ActionTypes actionT;
    float currTemp;

    int i = 0;
    while(i < sLen) {
      // If we run across
      if (data[i] == 'p') {
        // We are parsing a new pin
        pinInt = (int)(data[++i] - '0');
        Pin *p = select_pin(pinInt);
#if DEBUG
        Serial.print(F("Pin: "));
        Serial.print(pinInt);
        Serial.print(F("\n"));
#endif
        while(data[i++] != 'p' && i < sLen) {
          // We're in a pin object
          switch (data[i]) {
              case 'v':
                i++;
                valueInt = (int)(data[i] - '0');
#if DEBUG
                Serial.print(F("Value: "));
                Serial.print(valueInt);
                Serial.print(F("\n"));
#endif          
                p->setState(valueInt);
                i++;
                break;
              case 'a':
                i++;
                actionInt = (int)(data[i] - '0');
                actionT = (ActionTypes)(actionInt);
#if DEBUG
                Serial.print(F("Action: "));
                Serial.print(actionT);
                Serial.print(F("\n"));
#endif
                switch (actionT) {
                    case GETTEMP:
                      currTemp = getTemp(ds);
                      p->setCurrentValue(currTemp);
                      break;
#if DEBUG
                    default:
                      Serial.print(F("Unknown action: "));
                      Serial.print(actionT);
                      Serial.print(F("\n"));
#endif
                }
                break;
              case 'm':
                i++;
                modeInt = (int)(data[i] - '0');
#if DEBUG
                Serial.print(F("Mode: "));
                Serial.print(modeInt);
                Serial.print(F("\n"));
#endif  
                p->setMode(modeInt); 
                break;
              default:
#if DEBUG
                Serial.print(F("Unknown: "));
                Serial.print(data[i]);
                Serial.print(F("\n"));
#endif
          }
        }
      }
    }
  free(data);

  web_server.send_error_code(200);
  web_server.send_content_type("application/javascript");
  web_server.end_headers();

  pinsToString(web_server);

#if DEBUG
    printProgStats();
#endif

  return true;
}

TinyWebServer::PathHandler handlers[] = {
  {"/pins/update", TinyWebServer::POST, &digital_pin_handler},
  {"/pins", TinyWebServer::GET, &pins_handler},
  {"/", TinyWebServer::GET, &index_handler },
  {NULL},
};

const char* headers[] = {
  "X-Action-Len",
  NULL
};
TinyWebServer web = TinyWebServer(handlers, headers);

const char* ip_to_str(const uint8_t* ipAddr)
{
  static char buf[16];
  sprintf(buf, "%d.%d.%d.%d\0", ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);
  return buf;
}

float getTemp(OneWire sensor){
  //returns the temperature from one DS18S20 in DEG Celsius
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;

  if ( !sensor.search(addr)) {
     //no more sensors on chain, reset search
     sensor.reset_search();
     return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
     Serial.println("CRC is not valid!");
     return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
     Serial.print("Device is not recognized");
     return -1000;
  }

  sensor.reset();
  sensor.select(addr);
  sensor.write(0x44,1); // start conversion, with parasite power on at the end
  delay(1000);

  byte present = sensor.reset();
  sensor.select(addr);  
  sensor.write(0xBE); // Read Scratchpad

  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = sensor.read();
  }

  sensor.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  int16_t raw = (data[1] << 8) | data[0];
  raw = raw << 3;
  if (data[7] == 0x10) {
    raw = (raw & 0xFFF0) + 12 - data[6];
  }

  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  // float tempRead = ((MSB << 8) | LSB); //using two's compliment
  // float TemperatureSum = tempRead / 16;

  return fahrenheit;
}

void setup() {

#if DEBUG
  Serial.begin(9600);
#endif
  // pinMode(10, OUTPUT); // set the SS pin as an output (necessary!)
  // digitalWrite(10, HIGH); // but turn off the W5100 chip!

#if DEBUG
  Serial.print(F("Setting up pins...\n"));
#endif

  // numPins = SetupArduino(pins);
  for(int i=0; i < numPins; i++){
    pins[i].InitializeState();
  }

  UpdatePinsState();

#if DEBUG
  for(int i=0; i < numPins; i++){
    pins[i].Print();
  }
#endif

  // if (!card.init(SPI_FULL_SPEED, 4)) {
  //   Serial << F("card failed\n");
  // }
  // // initialize a FAT volume
  // if (!volume.init(&card)) {
  //   Serial << F("vol.init failed!\n");
  // }
  // root.openRoot(volume);
  // root.ls();

#if DEBUG
  Serial.print(F("Setting up the Ethernet card at: "));
  Serial.print(ip_to_str(ip));
  Serial.print(F("\n"));
#endif
  Ethernet.begin(mac, ip);

  // Start the web server.
#if DEBUG
  Serial.print(F("Web server starting...\n"));
#endif

  web.begin();

#if DEBUG  
  Serial.print(F("Ready to accept HTTP requests.\n\n"));
#endif

#if DEBUG
    printProgStats();
#endif
}

void loop() {
  web.process();
}

// Iterate over pins and update each of their states
void UpdatePinsState() {
  for(int i=0; i<numPins; i++){ pins[i].getState(); }
}

/*
*   Pin to string on the webserver helper
*   This takes a web_server instance and
*   writes the JSON array associated with
*   each of the pins
*/
bool pinsToString(TinyWebServer& web_server) {
  UpdatePinsState();
  web_server.print(F("{\"pins\":["));
  int len = numPins;
  for(int i=0; i<len; i++){
    web_server.print(F("{\"pin\":"));
    web_server.print(pins[i].getPin());
    web_server.print(F(",\"value\":"));
    web_server.print(pins[i].getState());
    web_server.print(F("}"));
    if ((i+1) < len) web_server.print(F(","));
  }
  web_server.print(F("]}"));
  return true;
}

bool pinToString(TinyWebServer& web_server, int pin_number) {
  UpdatePinsState();

  Pin *p = select_pin(pin_number);
  web_server.print(F("{\"pin\":"));
  web_server.print(p->getPin());
  web_server.print(F(",\"value\":"));
  web_server.print(p->getCurrentValue());
  web_server.print(F("}"));
  return true;
}

/*
*   Count the number of '/'
*   characters in the pathstring
*/
int count_forward_slashes(String pathstring) {
  int p = 0;
  int count = 0;
  while (pathstring[p] != '\0') {
    if (pathstring[p] == '/') count++;
    p++;
  }
  return count;
}

/*
*   Parse the requested URL into
*   an array of strings
*/
void parse_path_string(const char* cStr, int size, char** messages) { 
  char* str = strdup(cStr);
  char *p;
  int i = 0;
#if DEBUG
    Serial << "string: " << str << "\n";
#endif

  for(str = strtok_r(str,"/",&p); str; str = strtok_r(NULL, "/", &p))
  {
    int len = strlen(str);
    char *msg = (char *)malloc((len + 1) * sizeof(char)); // sizeof(char) == 1
    strncpy(msg, str, len + 1);

    messages[i] = msg;
    i++;
  }
}

/*
*   Read the remaining data from a request
*   based upon the 'X-Action-Len' header
*/
void get_request_data(TinyWebServer& web_server, int length_str, char* str)
{
  char ch;
  Client& client = web_server.get_client();
  if(client.available()) {
    for(int i=0; i<length_str; i++){
      ch = (char)client.read();
      if (ch != NULL && ch != '\n') {
        str[i] = ch;
      }
    }
  }
  str[length_str] = '\0';
}

/*
*   Select the pin object from the 
*   pin array based upon the pinNumber
*/
Pin *select_pin(uint8_t pinNumber) {
  for(int i=0; i<numPins; i++){
    if (pins[i].getPin() == pinNumber) return &pins[i];
  }
  return NULL;
}

/*
*   Get the free ram
*/
int freeRam() {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void printProgStats() {
#if DEBUG
  int freeR = freeRam();
  Serial.print(F("Free ram: "));
  Serial.print(freeR);
  Serial.print(F("\n"));
#endif
}