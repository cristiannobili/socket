int clientConnect(char* ipAddress, int port, char* errorMessage);
int serverConnect(char* ipAddress, int port, char* errorMessage);
void send(int socket, char* message);
void receive(int socket, char* buffer, int len);
void closeConnection(int socket);
int acceptNewIncomingSocket(int server_fd, char *buffer);
