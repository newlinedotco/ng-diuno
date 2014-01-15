 boolean file_handler(TinyWebServer& web_server) {
   char* filename = TinyWebServer::get_file_from_path(web_server.get_path());
   Serial << "Reading from filename: " << filename;
   send_file_name(web_server, filename);
   free(filename);
   return true;
 }

 void send_file_name(TinyWebServer& web_server, const char* filename) {
   if (!filename) {
     web_server.send_error_code(404);
     web_server.end_headers();
     web_server << F("Could not parse URL");
   } else {
     TinyWebServer::MimeType mime_type
       = TinyWebServer::get_mime_type_from_filename(filename);
     web_server.send_error_code(200);
     if (file.open(&root, filename, O_READ)) {
     web_server.send_content_type(mime_type);
     web_server.end_headers();
       Serial << F("Read file "); Serial.println(filename);
       web_server.send_file(file);
       file.close();
     } else {
     web_server.send_content_type("text/plain");
     web_server.end_headers();
     Serial << F("Could not find file: "); Serial.println(filename);
       web_server << F("Could not find file: ") << filename << "\n";
     }
   }
 }

boolean index_handler(TinyWebServer& web_server) {
  char* filename = TinyWebServer::get_file_from_path("/INDEX001.HTM");
  send_file_name(web_server, filename);
  free(filename);
  return true;
}


/ parsing path
 String pathstring = web_server.get_path();
#if DEBUG
 Serial << F("Parsed page from ") << pathstring << "\n";
#endif

 int size = count_forward_slashes(pathstring);
#if DEBUG
 Serial << F("Count: ") << size << "\n";
#endif
 char** parsed = (char**)malloc(sizeof(char *) * size);
 parse_path_string(pathstring.c_str(), size, parsed);

 if (strcmp(parsed[1], "digital") == 0) {
   Serial << "Digital " << parsed[0] << "\n";
   READ DATA
   const char* length_str = web_server.get_header_value("Content-Length");
   Client& client = web_server.get_client();

// for(int j=0; j<size; j++){
//   free(parsed[j]);
// }
// free(parsed);