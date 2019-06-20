
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


Socket::Socket()
{
    port = 8080
	iWsaStartup = WSAStartup(MAKEWORD(2, 2), &winSockData);
	if (iWsaStartup != 0)
	{
		cout << "WSaStartup failed " << endl;
		SockLog.logg_error("", "", 1);
		exit(0);
	}
	cout << "WSaStartup Success " << endl;

	//step2 Fillthe structure
	TCPServerAdd.sin_family = AF_INET;
	TCPServerAdd.sin_addr.s_addr = inet_addr("127.0.0.1");
	TCPServerAdd.sin_port = htons(port);
}
void Socket::createSocket()
{
	TCPServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (TCPServerSocket == INVALID_SOCKET)
	{
		cout << "TCP serversocket creation failed " << WSAGetLastError() << endl;
		SockLog.logg_error("", "", 2);
		exit(0);
	}
	cout << "TCP serversocket creation success " << endl;
}
void Socket::bindSocket()
{
	iBind = bind(TCPServerSocket, (SOCKADDR*)& TCPServerAdd, sizeof(TCPServerAdd));
	if (iBind == SOCKET_ERROR)
	{
		cout << "Binding failed. ERROR -> " << WSAGetLastError() << endl;
		SockLog.logg_error("", "", 3);
		exit(0);
	}
	cout << "Binding success " << endl;
}
void Socket::startListen()
{
	iListen = listen(TCPServerSocket, 2);
	if (iListen == SOCKET_ERROR)
	{
		cout << "listen function failed. ERROR -> " << WSAGetLastError() << endl;
		SockLog.logg_error("", "", 4);
		exit(0);
	}
	cout << "listen function success " << endl;
}
void Server::acceptConnection()
{
	Server server;
	sAcceptSocket = accept(TCPServerSocket, (SOCKADDR*)& TCPClientAdd, &iTCPClientAdd);
	if (sAcceptSocket == INVALID_SOCKET)
	{
		cout << "Accept function failed. ERROR -> " << WSAGetLastError() << endl;
		Log.logg_error("", "", 5);
		exit(0);
	}
	cout << "Accept function success " << endl;
	cout << sAcceptSocket << endl;
	socket_data.port = port;
	socket_data.sAcceptSocket = sAcceptSocket;
	socket_data.TCPClientAdd = TCPClientAdd;
	socket_data.TCPServerAddr = TCPServerAdd;
	hThread = CreateThread(NULL, 0, clientThread, (LPVOID)&socket_data, 0, &threadid);
	if (hThread == NULL)
	{
		Log.logg_error("", "", 6);
	}

}
Socket::~Socket()
{
	iCloseSocket = closesocket(TCPServerSocket);
	if (iCloseSocket == SOCKET_ERROR)
	{
		cout << "closing socket failed. ERROR -> " << WSAGetLastError() << endl;
		SockLog.logg_error("", "", 7);
		exit(0);
	}
	cout << "closing socket  success " << endl;

	//step10 cleanup
	iWsaCleanup = WSACleanup();
	if (iWsaCleanup == SOCKET_ERROR)
	{
		cout << "clean up failed. ERROR -> " << WSAGetLastError() << endl;
		SockLog.logg_error("", "", 8);
		exit(0);
	}
	cout << "cleanup success " << endl;

}
