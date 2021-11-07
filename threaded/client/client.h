int client_connect(char* ipaddress, unsigned short port);
int client_send(char buffer[], int size);
int client_receive(char* buffer, int size);
void client_close();