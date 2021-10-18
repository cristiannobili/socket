#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"

#define BUFFERSIZE 512
#define PORT 8080
#define IPADDRESS "127.0.0.1"

int main(void)
{
   // Socket creation
   int server_fd, new_socket, valread;
   char buffer[1024] = {0};
   char *hello = "Received from server";

   server_fd = serverConnect(IPADDRESS, PORT, "Failed to connect server");

   while (buffer != "close")
   {
      if (acceptNewIncomingSocket(server_fd, buffer))
      {
         printf("%s\n", buffer);
         send(new_socket, hello);
         printf("Hello message from server sent\n");
      }
      else
      {
         return 0;
      }
   }

   closeConnection(server_fd);
   printf("\n");
   system("pause");
   return (0);
}