#include <winsock.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFERSIZE 512
#define PORT 8080
#define IPADDRESS "127.0.0.1"
#pragma comment(lib, "ws2_32.lib")

void ClearWinSock()
{
   WSACleanup();
}

int manageError(char *errorMessage, int server_fd)
{
   printf(errorMessage);
   closesocket(server_fd);
   ClearWinSock();
   return 0;
}

int main(void)
{
   WSADATA wsaData;
   int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
   if (iResult != 0)
   {
      printf("error at WSASturtup\n");
      return 0;
   }
   int server_fd, new_socket, valread;
   char buffer[1024] = {0};
   char *hello = "Received from serr";
   server_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (server_fd < 0)
   {
      return manageError("socket creation failed.\n", server_fd);
   }
   struct sockaddr_in address;
   int address_len = sizeof(address);
   memset(&address, 0, address_len);
   address.sin_family = AF_INET;
   address.sin_addr.s_addr = inet_addr(IPADDRESS);
   address.sin_port = htons(PORT);
   if (bind(server_fd, (struct sockaddr *)&address,
            sizeof(address)) < 0)
   {
      return manageError("Failed to bind.\n", server_fd);
   }
   if (listen(server_fd, 3) < 0)
   {
      return manageError("Failed to listen.\n", server_fd);
   }
   if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &address_len)) < 0)
   {
      return manageError("Failed to Accept.\n", server_fd);
   }
   while (buffer != "close")
   {
      memset(buffer, 0, sizeof(buffer));
      valread = recv(new_socket, buffer, BUFFERSIZE, 0);
      printf("%s\n", buffer);
      send(new_socket, hello, strlen(hello), 0);
      printf("Hello message sent\n");
   }
   closesocket(server_fd);
   ClearWinSock();
   printf("\n");
   system("pause");
   return (0);
}