#define DEBUG 1

#include <SPI.h>
#include <Ardunio.h>
#include <Ethernet.h>
#include <Flash.h>
// #include <SD.h>
#include <string.h>
#include <TinyWebServer.h>
#include <aJSON.h>
#include "./pin.h"

boolean file_handler(TinyWebServer& web_server);
boolean index_handler(TinyWebServer& web_server);
boolean pins_handler(TinyWebServer& web_server);
boolean digital_pin_handler(TinyWebServer& web_server);
// String parse_path_string(String pathstring, int size);
void parse_path_string(const char* str, int size, char **messages);
void get_request_data(TinyWebServer &web_server, char *str);

// HELPERS
void setLedEnabled(int pin, int state);
boolean getPinState(int i);

const char *HOST = "localhost:9000";
Pin pins[11] = {
  Pin(11, DIGITAL),
  Pin(12, DIGITAL),
  Pin(13, DIGITAL),
};
int numPins = 3;

boolean has_filesystem = true;
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

// POST /dpin/int
boolean digital_pin_handler(TinyWebServer& web_server) {
  // handle pins
  // GET PATH
  String pathstring = web_server.get_path();
#if DEBUG
  Serial << F("Parsed page from ") << pathstring << "\n";
#endif

  int size = count_forward_slashes(pathstring);
#if DEBUG
  Serial << "Count: " << size << "\n";
#endif
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
    Serial << "JSON data ->" << data << "<-\n";
#endif
    aJsonObject *root = aJson.parse(data);
    aJsonObject* pinName = aJson.getObjectItem(root, "13");

    Serial << "13 is: [" << (pinName->valueint) << "]\n";
    int pinInt = atoi(pinName->name);
    if (pinName->valueint == 1){
      Serial << "Writing " << pinInt << " writing as LOW\n";
      digitalWrite(pinInt, LOW);
    }else{
      Serial << "Writing " << pinInt << " writing as HIGH\n";
      digitalWrite(pinInt, HIGH);
    }
  // }

  web_server.send_error_code(200);
  web_server.send_content_type("application/javascript");
  web_server.end_headers();

  // web_server << "{}";
  pinsToString(web_server);

  // for(int j=0; j<size; j++){
  //   free(parsed[j]);
  // }
  // free(parsed);
  free(data);
  // String pathstring = web_server.get_path();
  // String pinStr;
  // boolean keep = false;

  // int pin = atoi(pinStr.c_str());
  // boolean pinState = getPinState(pin);
  // Serial << "Pinstate (" << pin << "): " << pinState << "\n";
  // setLedEnabled(pin, !pinState);
  // pinState = !pinState;

  // pinsToString(web_server);

  // web_server << F("{\"pin\":\"");
  // web_server << pin;
  // web_server << F("\",\"state\":");
  // web_server << pinState;
  // web_server << F("}");
  return true;
}

// LED helpers
void setLedEnabled(int pin, int state) {
  // digitalPinStates[pin] = state;
  Serial << "Writing " << pin << " as " << state << "\n";
  // digitalWrite(pin, state);
  // delay(1000);
  // digitalWrite(pin, !state);
}
boolean getPinState(int i) { 
  return true;
}

TinyWebServer::PathHandler handlers[] = {
  {"/pins/" "*", TinyWebServer::POST, &digital_pin_handler},
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

// Setup arduino pins
int SetupArduino(Pin **newPins)
{
  int count = 3;

//   newPins = (Pin **)realloc(newPins, sizeof(Pin *) * 11);
//   for (int i = 0; i < 11; i++)
//   {
//     Pin *p = (Pin *)malloc(sizeof(Pin *));
//     // Pin p = Pin(i+2);
//     p->setPin(i+2);
//     // p->SetIO(OUTPUT_TYPE);
//     // p->SetState(LOW_STATE);
//     // p->SetType(DIGITAL);
//     newPins[i] = p;
//     count++;
//   }

  return count;
}

void setup() {

#if DEBUG
  Serial.begin(9600);
#endif
  // pinMode(10, OUTPUT); // set the SS pin as an output (necessary!)
  // digitalWrite(10, HIGH); // but turn off the W5100 chip!
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);

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

#if DEBUG
  for(int i=0; i < numPins; i++){
    pins[i].Print();
  }
#endif

  // if (!card.init(SPI_FULL_SPEED, 4)) {
  //   Serial << F("card failed\n");
  //   has_filesystem = false;
  // }
  // // initialize a FAT volume
  // if (!volume.init(&card)) {
  //   Serial << F("vol.init failed!\n");
  //   has_filesystem = false;
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

void UpdatePinsState() {
  for(int i=0; i<numPins; i++){
    uint8_t stateInt = pins[i].getState();
    Serial << "Updating " << pins[i].getPin() << " state to " << stateInt << "\n";
  }
}

bool pinsToString(TinyWebServer& web_server) {
  UpdatePinsState();

  web_server << F("{\"pins\": ");
  web_server << F("{\"digital\":{");
  int len = numPins;
  for(int i=0; i<len; i++){
    // digitalPins[i]
    web_server << F("\"");
    web_server << pins[i].getPin();
    web_server << F("\":\"");
    web_server << pins[i].getState();
    web_server << F("\"");
    if ((i+1) < len) web_server << F(",");
  }
  web_server << F("}}");
  web_server << F("}");
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

void get_request_data(TinyWebServer& web_server, int length_str, char* str)
{
  char ch;
  Client& client = web_server.get_client();
  if(client.available()) {
    for(int i=0; i<length_str; i++){
      ch = (char)client.read();
    // }
    // for(char ch = (char)client.read(); ch != NULL && ch != '\n'; ch = (char)client.read()) {
      if (ch != NULL && ch != '\n') {
#if DEBUG
        Serial << ch;
#endif
        str[i] = ch;
      }
    }
  }
}
