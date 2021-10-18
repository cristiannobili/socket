
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"

#define BUFFERSIZE 512
#define PORT 8080
#define IPADDRESS "127.0.0.1"

int main(int argc, char const *argv[])
{
	int client_fd, valread;
   char *hello; 
   if (argc > 1) {
      hello = (char*)malloc(sizeof(char) * strlen(argv[1]));
      strcpy(hello, argv[1]);
      printf("the message is: %s\n", hello);
   } else {
      hello = "Hello from client";
   }
   char buffer[BUFFERSIZE] = {0};
   client_fd = clientConnect(IPADDRESS, PORT, "Socket creation failed");
   if (!client_fd) {
      return client_fd;
   }	
   send(client_fd, hello);   
	printf("Hello message from client sent\n");
   receive(client_fd, buffer, BUFFERSIZE);
	printf("%s\n",buffer );
	system("pause");
	return 0;
}