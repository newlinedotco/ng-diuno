#define DEBUG 1

#include <SPI.h>
// #include <Ardunio.h>
#include <Ethernet.h>
#include <Flash.h>
// #include <SD.h>
#include <string.h>
#include <TinyWebServer.h>
#include <aJSON.h>
#include <OneWire.h>
#include "./pin.h"

boolean file_handler(TinyWebServer& web_server);
boolean index_handler(TinyWebServer& web_server);
boolean pins_handler(TinyWebServer& web_server);
boolean setup_handler(TinyWebServer& web_server);
boolean digital_pin_handler(TinyWebServer& web_server);
// boolean analog_pin_handler(TinyWebServer& web_server);
// String parse_path_string(String pathstring, int size);
void parse_path_string(const char* str, int size, char **messages);
void get_request_data(TinyWebServer &web_server, char *str);

const char *HOST = "localhost:9000";
Pin pins[11] = {
  Pin(9, DIGITAL),
  Pin(8, DIGITAL),
  Pin(7, DIGITAL),
};

// OneWire ds(2); // Temp sensor

int numPins = 3;

// const char *HOST = "fathomless-river-4136.herokuapp.com";
// Sd2Card card; 
// SdVolume volume;
// SdFile root;
// SdFile file;

static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Don't forget to modify the IP to an available one on your home network
byte ip[] = { 10, 0, 1, 32 };

// boolean file_handler(TinyWebServer& web_server) {
//   char* filename = TinyWebServer::get_file_from_path(web_server.get_path());
//   Serial << "Reading from filename: " << filename;
//   send_file_name(web_server, filename);
//   free(filename);
//   return true;
// }

// void send_file_name(TinyWebServer& web_server, const char* filename) {
//   if (!filename) {
//     web_server.send_error_code(404);
//     web_server.end_headers();
//     web_server << F("Could not parse URL");
//   } else {
//     TinyWebServer::MimeType mime_type
//       = TinyWebServer::get_mime_type_from_filename(filename);
//     web_server.send_error_code(200);
//     if (file.open(&root, filename, O_READ)) {
//     web_server.send_content_type(mime_type);
//     web_server.end_headers();
//       Serial << F("Read file "); Serial.println(filename);
//       web_server.send_file(file);
//       file.close();
//     } else {
//     web_server.send_content_type("text/plain");
//     web_server.end_headers();
//     Serial << F("Could not find file: "); Serial.println(filename);
//       web_server << F("Could not find file: ") << filename << "\n";
//     }
//   }
// }

/*
boolean index_handler(TinyWebServer& web_server) {
  char* filename = TinyWebServer::get_file_from_path("/INDEX001.HTM");
  send_file_name(web_server, filename);
  free(filename);
  return true;
}
*/

boolean index_handler(TinyWebServer& web_server) {
  web_server << "<html><head><title>Web server</title></head>";
  web_server << "<body>";
  web_server << "<script id=\"appscript\" src=\"http://" << HOST << "/scripts/scripts.js\"></script>";
  web_server << "</body></html>";
  return true;
}

// API
// 

// GET /pins
boolean pins_handler(TinyWebServer& web_server) {
  web_server.send_error_code(200);
  web_server.send_content_type("application/javascript");
  web_server.end_headers();

  // UpdatePinsState();
  // Client& client = web_server.get_client();
  pinsToString(web_server);
  return true;
}

// POST /pins/digital
boolean digital_pin_handler(TinyWebServer& web_server) {
  // handle pins
  // GET PATH
  // String pathstring = web_server.get_path();
// #if DEBUG
//   Serial << F("Parsed page from ") << pathstring << "\n";
// #endif

//   int size = count_forward_slashes(pathstring);
// #if DEBUG
//   Serial << F("Count: ") << size << "\n";
// #endif
  // char** parsed = (char**)malloc(sizeof(char *) * size);
  // parse_path_string(pathstring.c_str(), size, parsed);

  // if (strcmp(parsed[1], "digital") == 0) {
    // Serial << "Digital " << parsed[0] << "\n";
    // READ DATA
    // const char* length_str = web_server.get_header_value("Content-Length");
    // Client& client = web_server.get_client();
    const char* length_str = web_server.get_header_value("Content-Length");
    int len = atoi(length_str);

    char* data = (char*)malloc(len);
    if (data) memset(data, 0, sizeof(len));
    get_request_data(web_server, len, data);

    // DIGITAL
#if DEBUG
    Serial.print(data);// << "JSON data ->" << data << "<-\n";
#endif
    aJsonObject* root = aJson.parse(data);
    free(data);
    aJsonObject* pins = aJson.getObjectItem(root, "pins");

    int arrSize = aJson.getArraySize(pins);
#if DEBUG
    Serial.print(F("\nArray of pins: "));// << "Array of pins: " << arrSize << "\n";
    Serial.print(arrSize);
    Serial.print(F("\n"));
#endif

    aJsonObject* pinObj;
    aJsonObject* pinName;
    aJsonObject* pinValue;
    aJsonObject* pinMode;
    aJsonObject* pinAction;

    for(int i=0; i<arrSize; i++){
      pinObj = aJson.getArrayItem(pins, i);

      pinName = aJson.getObjectItem(pinObj, "pin");
      pinValue = aJson.getObjectItem(pinObj, "value");
      // pinMode = aJson.getObjectItem(pinObj, "mode");
      pinAction = aJson.getObjectItem(pinObj, "action");
      
#if DEBUG
      Serial.print(F("PIN:"));// << "Working with pin " << pinName->valueint << "\n";
      Serial.print(pinName->valueint);
      Serial.print(F("\n"));
#endif
      Pin *p = select_pin(pinName->valueint);
      if (pinMode != NULL) {
#if DEBUG
      Serial.print(F("MODE:"));// << "Working with pin " << pinName->valueint << "\n";
      Serial.print(pinMode->valueint);
      Serial.print(F("\n"));
      // Serial << "Setting pin " << (pinName->valueint) << " to pinMode: " << (pinMode->valueint) << "\n";
#endif
        p->setMode(pinMode->valueint); 
      }
      if (pinAction != NULL) {
#if DEBUG
        Serial.print(F("ACTION:"));// << "Working with pin " << pinName->valueint << "\n";
        Serial.print(pinAction->valuestring);
        Serial.print(F("\n"));
        // Serial << "pinAction is not null: " << pinAction->valuestring << "\n";
#endif
        if (strcmp(pinAction->valuestring, "getTemp") == 0) {

          OneWire ds(p->getPin());
          float currTemp = getTemp(ds);
          p->setCurrentValue(currTemp);
        }
      }
      if (pinValue != NULL) {
#if DEBUG
      Serial << "Setting pin " << (pinName->valueint) << " to pinValue: " << (pinValue->valueint) << "\n";
#endif      
        p->setState(pinValue->valueint == 1 ? HIGH : LOW);
      }
    }

  aJson.deleteItem(root);
  web_server.send_error_code(200);
  web_server.send_content_type("application/javascript");
  web_server.end_headers();

  pinToString(web_server, pinName->valueint);

  // for(int j=0; j<size; j++){
  //   free(parsed[j]);
  // }
  // free(parsed);

#if DEBUG
    printProgStats();
#endif

  return true;
}

boolean setup_handler(TinyWebServer& web_server)
{
  // GET REQUEST DATA
      const char* length_str = web_server.get_header_value("Content-Length");
      int len = atoi(length_str);

      char* data = (char*)malloc(len);
      if (data) memset(data, 0, sizeof(len));
      get_request_data(web_server, len, data);

      // DIGITAL
  #if DEBUG
      Serial.print(data);// << "JSON data ->" << data << "<-\n";
  #endif
      aJsonObject* root = aJson.parse(data);
      free(data);
      aJsonObject* pins = aJson.getObjectItem(root, "pins");

      int arrSize = aJson.getArraySize(pins);
  #if DEBUG
      Serial.print(F("\nArray of pins: "));// << "Array of pins: " << arrSize << "\n";
      Serial.print(arrSize);
      Serial.print(F("\n"));
  #endif

      aJsonObject* pinObj;
      aJsonObject* pinName;
      aJsonObject* pinMode;

      for(int i=0; i<arrSize; i++){
        pinObj = aJson.getArrayItem(pins, i);

        pinName = aJson.getObjectItem(pinObj, "pin");
        pinValue = aJson.getObjectItem(pinObj, "value");
        pinMode = aJson.getObjectItem(pinObj, "mode");
        
  #if DEBUG
        Serial.print(F("PIN:"));// << "Working with pin " << pinName->valueint << "\n";
        Serial.print(pinName->valueint);
        Serial.print(F("\n"));
  #endif
        Pin *p = select_pin(pinName->valueint);
        if (pinMode != NULL) {
  #if DEBUG
        Serial.print(F("MODE:"));// << "Working with pin " << pinName->valueint << "\n";
        Serial.print(pinMode->valueint);
        Serial.print(F("\n"));
        // Serial << "Setting pin " << (pinName->valueint) << " to pinMode: " << (pinMode->valueint) << "\n";
  #endif
          p->setMode(pinMode->valueint); 
        }
        if (pinAction != NULL) {
  #if DEBUG
          Serial.print(F("ACTION:"));// << "Working with pin " << pinName->valueint << "\n";
          Serial.print(pinAction->valuestring);
          Serial.print(F("\n"));
          // Serial << "pinAction is not null: " << pinAction->valuestring << "\n";
  #endif
          if (strcmp(pinAction->valuestring, "getTemp") == 0) {

            OneWire ds(p->getPin());
            float currTemp = getTemp(ds);
            p->setCurrentValue(currTemp);
          }
        }
        if (pinValue != NULL) {
  #if DEBUG
        Serial << "Setting pin " << (pinName->valueint) << " to pinValue: " << (pinValue->valueint) << "\n";
  #endif      
          p->setState(pinValue->valueint == 1 ? HIGH : LOW);
        }
      }

    aJson.deleteItem(root);
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
  // {"/pins/analog", TinyWebServer::GET, &analog_pin_handler},
  {"/pins/digital", TinyWebServer::POST, &digital_pin_handler},
  {"/pins/setup", TinyWebServer::POST, &setup_handler},
  {"/pins", TinyWebServer::GET, &pins_handler},
  {"/", TinyWebServer::GET, &index_handler },
  {NULL},
};

const char* headers[] = {
  "Content-Length",
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

byte present = sensor.reset();
sensor.select(addr);  
sensor.write(0xBE); // Read Scratchpad


for (int i = 0; i < 9; i++) { // we need 9 bytes
  data[i] = sensor.read();
}

sensor.reset_search();

byte MSB = data[1];
byte LSB = data[0];

float tempRead = ((MSB << 8) | LSB); //using two's compliment
float TemperatureSum = tempRead / 16;

return ((TemperatureSum*9)/5) + 32;

}

void setup() {

#if DEBUG
  Serial.begin(9600);
#endif
  // pinMode(10, OUTPUT); // set the SS pin as an output (necessary!)
  // digitalWrite(10, HIGH); // but turn off the W5100 chip!

  /*
  char *number;
  PinType type;
  PinSetting setting;
  PinState state;
  */
#if DEBUG
  Serial << F("Setting up pins...\n");
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
  Serial << F("Setting up the Ethernet card...\n");
#endif
  Ethernet.begin(mac, ip);

  // Start the web server.
#if DEBUG
  Serial << F("Web server starting...\n");
#endif

  web.begin();

#if DEBUG  
  // Serial << F("Free RAM: ") << FreeRam() << "\n";
  Serial << F("Ready to accept HTTP requests.\n\n");
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

  // web_server << F("{\"pins\": ");
  web_server << F("{\"digital\":[");
  int len = numPins;
  for(int i=0; i<len; i++){
    // digitalPins[i]
    web_server << F("{\"pin\":");
    web_server << pins[i].getPin();
    web_server << F(",\"value\":");
    web_server << pins[i].getCurrentValue();
    web_server << F("}");
    if ((i+1) < len) web_server << F(",");
  }
  web_server << F("]}");
  // web_server << F("}");
  return true;
}

bool pinToString(TinyWebServer& web_server, int pin_number) {
  UpdatePinsState();

  // web_server << F("{\"pins\": ");
  // web_server << F("{\"digital\":[");
  int len = numPins;
  Pin *p = select_pin(pin_number);
  // for(int i=0; i<len; i++){
    // digitalPins[i]
    web_server << F("{\"pin\":");
    web_server << p->getPin();
    web_server << F(",\"value\":");
    web_server << p->getCurrentValue();
    web_server << F("}");
  // }
  // web_server << F("}");
  // web_server << F("}");
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
    // char *msg = strdup(str);
#if DEBUG
    Serial << "Writing " << i << " as " << msg << " (" << str << ")\n";
#endif
    messages[i] = msg;
    i++;
  }
}

/*
*   Read the remaining data from a request
*   based upon the 'Content-Length' header
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
  Serial << F("Free ram: ") << freeR << F("\n");
#endif
}