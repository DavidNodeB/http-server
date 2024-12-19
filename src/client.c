#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#define BUFFER_SIZE 512

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char *serverIP = "127.0.0.1";
    char *clMessage = "CLIENT: message received from server";
    int clMessageLen = strlen(clMessage);
    char clbuffer[BUFFER_SIZE]; // default size
    int port = 5001;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("CLIENT_ERROR: %d\n", WSAGetLastError());
        return 1;
    }

    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("CLIENT_ERROR: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    serverAddr.sin_port = htons(port);

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("CLIENT_ERROR: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    printf("CLIENT: connected to server.\n");

    if (send(clientSocket, clMessage, clMessageLen, 0) == SOCKET_ERROR) {
        printf("CLIENT_ERROR: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    printf("CLIENT: message sent\n");

    // Correct recv buffer size
    if (recv(clientSocket, clbuffer, BUFFER_SIZE, 0) == SOCKET_ERROR) {
        printf("SERVER_ERROR: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    printf("CLIENT: received message: %s\n", clbuffer);

    closesocket(clientSocket);
    WSACleanup();
    getchar(); 
    return 0;
}
