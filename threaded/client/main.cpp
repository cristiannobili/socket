#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "client.h"

#define BUFFERSIZE 512
#define PORT 8080
#define IPADDRESS "127.0.0.1"

int main(int argc, char const *argv[])
{
   if (client_connect(IPADDRESS, PORT))
   {
      int valread;
      char message[BUFFERSIZE];
      char buffer[1024] = {0};

      while (strcmp(message, "close"))
      {
         memset(message, 0, sizeof(message));
         printf("Please insert a message: ");
         std::cin.getline(message, 50);
         client_send(message, strlen(message));
         printf("Hello message sent\n");
         valread = client_receive(buffer, BUFFERSIZE);
         printf("%s\n", buffer);
         printf("%s\n", message);
      }
   }
}