char response_200[] = 
  "HTTP/1.1 200 OK\r\n"
  "Server: MWebD\r\n"
  "Connection: close\r\n"
  "Content-Length: 194\r\n"
  "\r\n"
  "<!doctype html><html><head><title>200 OK</title></head><body><h1>200 OK</h1><p>This is an example page to signify that the server is operating correctly.</p><hr><p><i>MWebD</i></p></body></html>";

char response_413[] = 
  "HTTP/1.1 413 Content Too Large\r\n"
  "Server: MWebD\r\n"
  "Connection: close\r\n"
  "Content-Length: 211\r\n"
  "\r\n"
  "<!doctype html><html><head><title>413 Content Too Large</title></head><body><h1>413 Content Too Large</h1><p>The server rejected your request as it is larger than 16 KiB.</p><hr><p><i>MWebD</i></p></body></html>";

char response_500[] = 
  "HTTP/1.1 500 Internal Server Error\r\n"
  "Server: MWebD\r\n"
  "Connection: close\r\n"
  "Content-Length: 206\r\n"
  "\r\n"
  "<!doctype html><html><head><title>500 Internal Server Error</title></head><body><h1>500 Internal Server Error</h1><p>An error occurred while processing your request.</p><hr><p><i>MWebD</i></p></body></html>";
