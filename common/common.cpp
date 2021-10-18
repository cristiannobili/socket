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

#pragma comment(lib, "ws2_32.lib") //Winsock Library

void clearWinSock()
{
#ifdef _WIN32
   WSACleanup();
#endif
}

void init()
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
}

struct sockaddr_in address;

int manageError(char *errorMessage, int socket)
{
   printf(errorMessage);
   closesocket(socket);
   clearWinSock();
   return 0;
}

int clientConnect(char *ipAddress, int port, char *errorMessage)
{
   init();
   int clientSocket;
   clientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (clientSocket < 0)
   {
      return manageError(errorMessage, clientSocket);
   }
   // Server address construction
   struct sockaddr_in address;
   memset(&address, 0, sizeof(address));
   address.sin_family = AF_INET;
   address.sin_addr.s_addr = inet_addr(ipAddress); // server IP
   address.sin_port = htons(port);                 // Server port

   // Connection to the server
   if (connect(clientSocket, (struct sockaddr *)&address, sizeof(address)) < 0)
   {
      return manageError(errorMessage, clientSocket);
   }
   return clientSocket;
}

int serverConnect(char *ipAddress, int port, char *errorMessage)
{
   int server_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (server_fd < 0)
   {
      return manageError(errorMessage, server_fd);
   }

   int address_len = sizeof(address);
   memset(&address, 0, address_len);
   address.sin_family = AF_INET;
   address.sin_addr.s_addr = inet_addr(ipAddress);
   address.sin_port = htons(port);
   if (bind(server_fd, (struct sockaddr *)&address,
            sizeof(address)) < 0)
   {
      return manageError(errorMessage, server_fd);
   }
   if (listen(server_fd, 3) < 0)
   {
      return manageError(errorMessage, server_fd);
   }
   return server_fd;
}

int acceptNewIncomingSocket(int server_fd, char* buffer)
{
   int valread, remoteSocket;
   memset(buffer, 0, sizeof(buffer));
   int address_len = sizeof(address);
#ifdef _WIN32
   if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &address_len)) < 0)
   {
      return manageError("Failed to Accept.\n", server_fd);
   }
   valread = recv(new_socket, buffer, 1024, 0);
#else
   if ((remoteSocket = accept(server_fd, (struct sockaddr *)&address,
                              (socklen_t *)&address_len)) < 0)
   {
      return manageError("Failed to Accept.\n", server_fd);
   }
   valread = read(remoteSocket, buffer, 1024);
#endif
   return valread;
}

void send(int socket, char *message)
{
   send(socket, message, strlen(message), 0);
}

void receive(int socket, char *buffer, int len)
{
   int value;
#ifdef _WIN32
   value = recv(socket, buffer, len, 0);
#else
   value = read(socket, buffer, len);
#endif
}

void closeConnection(int socket)
{
   closesocket(socket);
   clearWinSock();
}