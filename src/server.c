#include <stdio.h>
#include <winsock2.h>

int main(int argc, char *argv[])
{
    WSADATA wsaData; 
    SOCKET serverSocket; 
    struct sockaddr_in serverAddr;  
    WORD ws2Version = MAKEWORD(2, 2);
    char *serverIP = "127.0.0.1"; // localhost
    int serverSize = sizeof(serverAddr); // size of serverAddr in bytes
    int port = 5001; 

    if (WSAStartup(ws2Version, &wsaData) != 0) {
        printf("SERVER_ERROR: %d\n", WSAGetLastError()); 
        WSACleanup();
        return 1; // error and exit
    }

    printf("SERVER: socket initialized\n"); 

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
    {
        printf("SERVER_ERROR: %d\n", WSAGetLastError()); 
        WSACleanup(); 
        return 1;
    }

    printf("SERVER: socket created\n"); 

    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // accept any incoming connections
    serverAddr.sin_port = htons(port); 

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        printf("SERVER_ERROR: %d\n", WSAGetLastError()); 
        return 1; 
    }

    printf("SERVER: socket binded\n");  

    if (listen(serverSocket, 5) == SOCKET_ERROR)
    {
        printf("SERVER_ERROR: %d\n", WSAGetLastError()); 
        WSACleanup();
        return 1; 
    }

    printf("SERVER: socket listening\n"); 

    if (accept(serverSocket, (struct sockaddr *)&serverAddr, &serverSize) == INVALID_SOCKET)
    {
        printf("SERVER_ERROR: %d\n", WSAGetLastError()); 
        WSACleanup(); 
        return 1; 
    }

    printf("SERVER: client accepted"); 
    
    // messages

    WSACleanup(); 
    getchar(); 
    return 0; 
}