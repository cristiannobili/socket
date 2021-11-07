#include <winsock.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "client.h"
#pragma comment(lib, "ws2_32.lib") //Winsock Library

#define BUFFERSIZE 512

SOCKET socket_id;

void ClearWinSock()
{
    WSACleanup();
}

int manageError(char *errorMessage, int socket)
{
    printf(errorMessage);
    if (socket > -1)
    {
        closesocket(socket);
    }
    ClearWinSock();
    return 0;
}

int client_connect(char *ipaddress, unsigned short port)
{
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("error at WSASturtup\n");
        return 0;
    }
    socket_id = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_id < 0)
    {
        return manageError("socket creation failed.\n", socket_id);
    }
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ipaddress); 
    address.sin_port = htons(port);            

    // Connection to the server
    if (connect(socket_id, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        return manageError("Failed to connect.\n", socket_id);
    }

    unsigned long arg = 1;
    if (ioctlsocket(socket_id, FIONBIO, &arg) == SOCKET_ERROR) {
        return manageError("Failed to connect.\n", socket_id);
    }

    printf("Client connected to server.\n");
    return socket_id;
}

int client_send(char buffer[], int size) {
    int n;
    if (n = send(socket_id, buffer, (size_t)size, 0) < 0) {
        return manageError("Error sending message", socket_id); 
    }
    return n;
}

int client_receive(char* buffer, int size) {
    int n;
    if (n = recv(socket_id, buffer, size, 0) <= 0) {
        return manageError("Error sending message", socket_id); 
    }
    return n;
}

void client_close() {
    closesocket(socket_id);
    ClearWinSock();
}
