#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "server.h"

#define BUFFERSIZE 512
#define PORT 8080
#define IPADDRESS "127.0.0.1"

int main(int argc, char const *argv[])
{
   server_connect(IPADDRESS, PORT);
}