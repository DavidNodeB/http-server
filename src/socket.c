#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <stdlib.h> 
#include "socket.h"

int main(int argc, char *argv[]) {
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int serverSize = sizeof(serverAddr); // size of serverAddr in bytes
    char *message = "Hello client from server";
    int messageLen = strlen(message);
    char recvBuffer[512];
    int clientSize = sizeof(clientAddr);
    char *token, *method, *path, *version, *connection, *host, *acceptStr; 

    if (WSAStartup(ws2Version, &wsaData) != 0) {
        printf("SERVER_ERROR: %d\n", WSAGetLastError());
        return 1; // error and exit
    }

    printf("SERVER: socket initialized\n");

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
        printf("SERVER_ERROR: %d\n", WSAGetLastError());
        return 1;
    }

    printf("SERVER: socket created\n");

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // accept any incoming connections
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("SERVER_ERROR: %d\n", WSAGetLastError());
        return 1;
    }

    printf("SERVER: socket binded\n");

    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        printf("SERVER_ERROR: %d\n", WSAGetLastError());
        return 1;
    }

    printf("SERVER: socket listening\n");

    // need clientSocket or else you can't send message or receive
    if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientSize)) == INVALID_SOCKET) {
        printf("SERVER_ERROR: %d\n", WSAGetLastError());
        return 1;
    }

    printf("SERVER: client accepted\n");

    if (send(clientSocket, message, messageLen, 0) == SOCKET_ERROR) {
        printf("SERVER_ERROR: %d\n", WSAGetLastError());
        return 1;
    }

    printf("SERVER: message sent\n");

    if (recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0) == SOCKET_ERROR) {
        printf("SERVER_ERROR: %d\n", WSAGetLastError());
        return 1;
    }

    token = strdup(recvBuffer); 

    if (token != NULL) 
    {
        method = strtok(token, " ");
        path = strtok(NULL, " ");
        version = strtok(NULL, "\n");
        printf("Method: %s\n", method);
        printf("Path: %s\n", path);
        printf("Version: %s\n", version);
    }

    char *header = strtok(NULL, "\r\n"); 

    while (header != NULL) {
        if (strstr(header, "Host: ")) {
            host = header + 6;
            printf("Host: %s\n", host);
        }

        if (strstr(header, "Accept: ")) {
            acceptStr = header + 8; 
            printf("Accept: %s\n", acceptStr);
        }

        if (strstr(header, "Connection: ")) {
            connection = header + 12;
            printf("Connection: %s\n", connection);
        }

        header = strtok(NULL, "\r\n"); 
    }

    // close and cleanup
    free(token); 
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    getchar(); 
    return 0;
}
