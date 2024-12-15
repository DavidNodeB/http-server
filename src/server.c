#include <stdio.h>
#include <winsock2.h>

int main(int argc, char *argv[])
{
    WSADATA wsaData; 
    SOCKET serverSocket; 
    struct sockaddr_in serverAddr;  
    int serverSize = sizeof(serverAddr); 
    char *serverIP = "127.0.0.1"; 
    int port = 5001; 

    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
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
    serverAddr.sin_addr.s_addr = inet_addr(serverIP); 
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