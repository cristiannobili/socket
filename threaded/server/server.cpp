#include <winsock.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <thread>

#ifndef WIN32
#include <socket>
#else
//
#endif 

#include "server.h"

#define BUFFERSIZE 512
#define PORT 8080
#define IPADDRESS "127.0.0.1"
#pragma comment(lib, "ws2_32.lib")

SOCKET socket_id;
int clients;

void ClearWinSock()
{
   WSACleanup();
}

void manageError(char *errorMessage, int socket_id)
{
   printf(errorMessage);
   closesocket(socket_id);
   ClearWinSock();
}

void client_manage(int connection_socket_id, int* clients)
{
   (*clients)++;
   bool closed = false;
   char *hello = "Message received!";
   printf("Connection accepted in a new thead from socket: %d\n", connection_socket_id);
   printf("Currently active clients: %d\n", *clients);
   while (!closed)
   {
      char buffer[BUFFERSIZE] = {0};
      int valread = recv(connection_socket_id, buffer, BUFFERSIZE, 0);
      if (valread > 0) {
         printf("Message from socket %d: %s\n", connection_socket_id, buffer);
         send(connection_socket_id, hello, strlen(hello), 0);
         printf("Hello message sent\n");
      } else {
         closed = true;
         printf("Socket %d disconnected\n", connection_socket_id);
      }
   }
   closesocket(connection_socket_id);
   (*clients)--;
}

void server_connect(char *ipaddress, unsigned short port)
{
   int new_socket;
   WSADATA wsaData;
   int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
   if (iResult != 0)
   {
      printf("error at WSASturtup\n");
      return;
   }

   socket_id = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (socket_id < 0)
   {
      manageError("socket creation failed.\n", socket_id);
      return;
   }

   // Server address construction
   struct sockaddr_in address;
   int address_len = sizeof(address);
   memset(&address, 0, address_len);
   address.sin_family = AF_INET;
   address.sin_addr.s_addr = inet_addr(IPADDRESS); // server IP
   address.sin_port = htons(PORT);                 // Server port

   // Create connection
   if (bind(socket_id, (struct sockaddr *)&address,
            sizeof(address)) < 0)
   {
      manageError("Failed to bind.\n", socket_id);
      return;
   }

   if (listen(socket_id, 3) < 0)
   {
      manageError("Failed to listen.\n", socket_id);
      return;
   }
   clients = 0;
   while (true)
   {
      if ((new_socket = accept(socket_id, (struct sockaddr *)&address, &address_len)) < 0)
      {
         manageError("Failed to Accept.\n", socket_id);
         return;
      }
      std::thread *new_thread = new std::thread(client_manage, new_socket, &clients);
      new_thread->detach();
      delete new_thread;
   }
}

void server_close()
{
   closesocket(socket_id);
   ClearWinSock();
}