
#include "Socket.h"
#include "Logger.h"
#include "Server.h"
#include<iostream>
#include<WinSock2.h>
#include<fstream>
#include<istream>
#include<string>//getline
#include<cstring>
#include<fileapi.h>
#define STR "C:\\Users\\Dell\\Desktop\\PHASE2PROJECT\\HttpClass\\FileRepositary\\"
using namespace std;
int main()
{
    Server server;
    while(1)
    {
        server.acceptConnection();
    }
}
