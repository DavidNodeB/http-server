#pragma once 
#include <winsock2.h>

WSADATA wsaData; 
const WORD ws2Version = MAKEWORD(2, 2);
const char *serverIP = "127.0.0.1"; // localhost
const int port = 5001;