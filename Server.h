#ifndef SERVER_H
#define SERVER_H

#define LEN 512
SOCKET TCPServerSocket;
struct sockaddr_in TCPServerAdd;
#define MAX 500
#pragma warning(disable:4996)
#pragma comment(lib, "Ws2_32.lib")
using namespace std;
string SenderBuffer;
int iSend;
struct sockaddr_in TCPClientAdd;
int iTCPClientAdd = sizeof(TCPClientAdd);
HANDLE hThread;

struct socket_info
{

	sockaddr_in TCPClientAdd;
	sockaddr_in TCPServerAddr;
	int sAcceptSocket;
	int port;
};
class Server
{
public:
	string filepath;
	char RecvBuffer[LEN];
	Logger Log;
	DWORD threadid;
	int iRecv;
	int irecvBuffer;
	int err;
	int port;
	SOCKET sAcceptSocket;
	struct socket_info socket_data;
    Server();
	void acceptConnection();
};
