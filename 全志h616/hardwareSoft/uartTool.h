int myserialOpen (const char *device, const int baud);
void serialSendstring(const int fd,const char *s);
void serialGetstring(const int fd,char *buffer);
