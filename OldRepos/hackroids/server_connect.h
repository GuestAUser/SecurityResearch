#include <iostream>
#include <array>
#include <string>
#include <stdlib.h>
#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
using namespace std;
#define DEFAULT_PORT "27015" //windows default port

#pragma comment(lib, "Ws2_32.lib") //import all windows libs to winsock run smooth

void ServerCreate(string name) {
// Declare variables
    cout << "Creating Server...";
    SOCKET l_socket;
    struct sockaddr_in hostServer;
    hostent* localHost;
    char* localIP;
    char hostN[255];

    l_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    localHost = gethostbyname("");

    localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);

    hostServer.sin_family = AF_INET;
    hostServer.sin_addr.s_addr = inet_addr(localIP);
    hostServer.sin_port = htons(5150);
    cout << "debug"; //REMOVE LATER
    bind( l_socket,(SOCKADDR*) &hostServer, sizeof(hostServer) );
    struct addrinfo *serverInf = NULL,
                *ptr = NULL,
                hints;
    cout << "debug"; //remove later
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    gethostname(hostN, 255);
    closesocket(l_socket);
    //fazer dps
}