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

int main(void)
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
   int server_fd, new_socket, valread;
   char buffer[1024] = {0};
   char *hello = "Received from serr";
   server_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (server_fd < 0)
   {
      return manageError("socket creation failed.\n", server_fd);
   }

   // Server address construction
   struct sockaddr_in address;
   int address_len = sizeof(address);
   memset(&address, 0, address_len);
   address.sin_family = AF_INET;
   address.sin_addr.s_addr = inet_addr(IPADDRESS); // server IP
   address.sin_port = htons(PORT);                 // Server port


   // Create connection
   if (bind(server_fd, (struct sockaddr *)&address,
            sizeof(address)) < 0)
   {
      return manageError("Failed to bind.\n", server_fd);
   }

   if (listen(server_fd, 3) < 0)
   {
      return manageError("Failed to listen.\n", server_fd);
   }

#ifdef _WIN32
      if ((new_socket = accept(server_fd , (struct sockaddr *)&address, &address_len)) < 0)
      {
         return manageError("Failed to Accept.\n", server_fd);
      }
#else
      if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                               (socklen_t *)&address_len)) < 0)
      {
         return manageError("Failed to Accept.\n", server_fd);
      }
#endif

   while (buffer != "close")
   {
      memset(buffer, 0, sizeof(buffer));
#ifdef _WIN32
      valread = recv(new_socket, buffer, 1024, 0);
#else
      valread = read(new_socket, buffer, 1024);
#endif

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