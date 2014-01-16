% Powering Human Interaction
% powering the arduino with angular

## Ari Lerner, Fullstack.io

- Author of [ng-book](https://ng-book.com)
- Author of a few others ([D3 on Angular](http://leanpub/d3angularjs), [Riding Rails with AngularJS](https://leanpub.com/angularjs-rails))
- Teacher at [HackReactor](http://hackreactor.com)
- Cofounder of [Fullstack.io](http://fullstack.io)
- Background in distributed computing and infrastructure (really)

## Why Angular?

## Why Arduino?

- Embedded systems
- Wearable computing
- Low-power systems

## Examples

- Nest
- Pebble
- Phillips Hue
- Bellini Wemo

## Overview

- Running an HTTP server on the arduino
- Requesting the HTML from the arduino
- Loading angular app
- ...

## Running an HTTP server

An HTTP server written in C... it's hard... 

---------------------

```c
void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<h1>Hi from the Arduino</h1>");
          break;
        }
        if (c == '\n') { currentLineIsBlank = true; } 
        else if (c != '\r') { currentLineIsBlank = false; }
      }
    }
    delay(1);
    client.stop();
  }
}
```

## Enter TinyWebServer

Super simplification of the c code

-----------------------

```c
#include <TinyWebServer.h>

boolean index_handler(TinyWebServer& web_server);

boolean index_handler(TinyWebServer& web_server) {
  web_server.print(F("<html><head><title>Web server</title></head>"));
  web_server.print(F("<body></body></html>"));
  return true;
}
```

----------------------------

```c
// Handlers
TinyWebServer::PathHandler handlers[] = {
  {"/pins/digital", TinyWebServer::POST, &digital_pin_handler},
  {"/pins", TinyWebServer::GET, &pins_handler},
  {"/", TinyWebServer::GET, &index_handler },
  {NULL},
};

// Headers to process
const char* headers[] = {
  "Content-Length",
  "X-Action-Len",
  NULL
};
TinyWebServer web = TinyWebServer(handlers, headers);
// ...
void loop() {
  web.process();
};
```

## Connecting to the internet

- Ethernet shield
- Wifi shield

--------------------------
```c
#include <Ethernet.h>
byte ip[] = { 192, 168, 0, 67 };
// ...
Ethernet.begin(mac, ip);
```

_DHCP_ is also supported

## But where's the angular?

## Options

## Embed directly in Arduino code

### Pros

- Simple

### Cons

- Hard to update
- Space limitations (32kb is not a lot of space)

## Read/Send from SD card

### Pros

- Simple
- Tons of space (4+ gigs)

### Cons

- Hard to update

## Load from remote server

### Pros

- Simple
- Easy to update

### Cons

- Need internet connection

### Browser

-------------------------
