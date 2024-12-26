#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <stdlib.h> 

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
    char *token, *method, *path, *version, *connection, *host, *acceptstr; 

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

    printf("%s\n", recvBuffer);

    token = strdup(recvBuffer); 
    method = strtok(recvBuffer, " "); 
    path = strtok(NULL, " "); 
    version = strtok(NULL, " "); 
    host = strstr(token, "Host: ");
    acceptstr = strstr(token, "Accept: "); 
    connection = strstr(token, "Connection: "); 

    if (token != NULL) {
        // skip over identifier
        host += 6;
        acceptstr += 8;
        connection += 12; 
        
        // find the end of the line
        char* hostend = strpbrk(host, " \r\n"); 
        char* acceptend = strpbrk(acceptstr, ",");
        char* connectionend = strpbrk(connection, " ");
        
        // terminate the line
        *hostend = '\0'; 
        *acceptend = '\0'; 
        *connectionend = '\0';        
    } 
    
    printf("%s\n", method); 
    printf("%s\n", path); 
    printf("%s\n", version); 
    printf("%s\n", host); 
    printf("%s\n", acceptstr); 
    printf("%s\n", connection);  

    // close and cleanup
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    getchar(); 
    return 0;
}
