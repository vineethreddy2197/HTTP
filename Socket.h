#ifndef SOCKET_H
#define SOCKET_H

class Socket
{
public:
	WSADATA winSockData;
	int iWsaStartup;
	int iWsaCleanup;
	int iCloseSocket;

	int iBind;
	int iListen;
	Logger SockLog;
	int port;

	Socket();
	void createSocket();
	void bindSocket();
	void startListen();
	~Socket();
};
