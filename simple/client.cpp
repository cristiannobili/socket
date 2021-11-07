#ifdef _WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#define BUFFERSIZE 512
#define PORT 8080
#define IPADDRESS "127.0.0.1"
#pragma comment(lib, "ws2_32.lib") //Winsock Library

void ClearWinSock()
{
#ifdef _WIN32
   WSACleanup();
#endif
}

int manageError(char *errorMessage, int server_fd)
{
   printf(errorMessage);
   closesocket(server_fd);
   ClearWinSock();
   return 0;
}

int main(int argc, char const *argv[])
{
#ifdef _WIN32
   WSADATA wsaData;
   int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
   if (iResult != 0)
   {
      printf("error at WSASturtup\n");
      return 0;
   }
#endif

   // Socket creation
   int client_fd, valread;
   char message[50];

   char buffer[1024] = {0};
   client_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (client_fd < 0)
   {
      return manageError("socket creation failed.\n", client_fd);
   }
   // Server address construction
   struct sockaddr_in address;
   memset(&address, 0, sizeof(address));
   address.sin_family = AF_INET;
   address.sin_addr.s_addr = inet_addr(IPADDRESS); // server IP
   address.sin_port = htons(PORT);                 // Server port

   // Connection to the server
   if (connect(client_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
   {
      return manageError("Failed to connect.\n", client_fd);
   }

   printf("Client connected to server.\n");
   
   while (message != "close")
   {
      memset(message, 0, sizeof(message));
      printf("Please insert a message: ");
      std::cin.getline(message, 50);
      send(client_fd, message, strlen(message), 0);
      printf("Hello message sent\n");
#ifdef _WIN32
      valread = recv(client_fd, buffer, 1024, 0);
#else
      valread = read(client_fd, buffer, 1024);
#endif

      printf("%s\n", buffer);
   }

   closesocket(client_fd);
   ClearWinSock();
   printf("\n");
   system("pause");
   return (0);
}