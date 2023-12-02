char response_200[] =
  "HTTP/1.1 200 OK\r\n"
  "Server: MWebD\r\n"
  "Connection: close\r\n"
  "Content-Type: text/html\r\n"
  "Content-Length: 194\r\n"
  "\r\n"
  "<!doctype html><html><head><title>200 OK</title></head><body><h1>200 OK</h1><p>This is an example page to signify that the server is operating correctly.</p><hr><p><i>MWebD</i></p></body></html>";

char response_413[] =
  "HTTP/1.1 413 Content Too Large\r\n"
  "Server: MWebD\r\n"
  "Connection: close\r\n"
  "Content-Type: text/html\r\n"
  "Content-Length: 185\r\n"
  "\r\n"
  "<!doctype html><html><head><title>413 Content Too Large</title></head><body><h1>413 Content Too Large</h1><p>Your request is larger than 16 KiB.</p><hr><p><i>MWebD</i></p></body></html>";

char response_500[] =
  "HTTP/1.1 500 Internal Server Error\r\n"
  "Server: MWebD\r\n"
  "Connection: close\r\n"
  "Content-Type: text/html\r\n"
  "Content-Length: 206\r\n"
  "\r\n"
  "<!doctype html><html><head><title>500 Internal Server Error</title></head><body><h1>500 Internal Server Error</h1><p>An error occurred while processing your request.</p><hr><p><i>MWebD</i></p></body></html>";

char response_501[] =
  "HTTP/1.1 501 Not Implemented\r\n"
  "Server: MWebD\r\n"
  "Connection: close\r\n"
  "Content-Type: text/html\r\n"
  "Content-Length: 186\r\n"
  "\r\n"
  "<!doctype html><html><head><title>501 Not Implemented</title></head><body><h1>501 Not Implemented</h1><p>Your request's method is not recognized.</p><hr><p><i>MWebD</i></p></body></html>";
