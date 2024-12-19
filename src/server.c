#include <stdio.h>
#include <winsock2.h>
#include <string.h>

int main(int argc, char *argv[]) {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    WORD ws2Version = MAKEWORD(2, 2);
    char *serverIP = "127.0.0.1"; // localhost
    int serverSize = sizeof(serverAddr); // size of serverAddr in bytes
    char *message = "Hello client from server";
    int messageLen = strlen(message);
    char recvBuffer[512];
    int clientSize = sizeof(clientAddr);
    int port = 5001;

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

    printf("SERVER: received message: %s\n", recvBuffer);

    // close and cleanup
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    getchar(); 
    return 0;
}
