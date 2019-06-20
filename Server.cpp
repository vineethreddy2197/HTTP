#include "Server.h"
#include "Socket.h"
#include<iostream>
#include<WinSock2.h>
#include<fstream>
#include<istream>
#include<string>//getline
#include<cstring>
#include<fileapi.h>
#define FLEN 20
#define STR "C:\\Users\\Dell\\Desktop\\PHASE2PROJECT\\HttpClass\\FileRepositary\\"
using namespace std;


DWORD WINAPI clientThread(LPVOID lpParam)
{
	Logger log;
	string filepath = STR;
	cout << "Thread created successfully";
	char RecvBuffer[LEN];
	int iRecv;
	int irecvBuffer = strlen(RecvBuffer) + 1;
	//int err;
	struct socket_info* s = (socket_info*)lpParam;
	struct sockaddr_in* addr_in = (struct sockaddr_in*) & s->TCPClientAdd;
	char* ip = inet_ntoa(addr_in->sin_addr);
	cout << s->sAcceptSocket << endl;
	//step8 receiving data from client
	while (1)
	{
		cout << "thread id" << GetCurrentThreadId() << endl;

		iRecv = recv(s->sAcceptSocket, RecvBuffer, irecvBuffer, 0);
		irecvBuffer = strlen(RecvBuffer) + 1;

		if (iRecv <= 0)
		{
			cout << "DATA receiving failed. ERROR -> " << WSAGetLastError() << endl;
			CloseHandle(GetCurrentThread());
			log.logg_error(ip, "Error: Data Receive Failed", 0);
			cout << "thread closed" << endl;
			break;
		}
		cout << "DATA RECEIVED  " << RecvBuffer << endl;

		//getting the filename from the client request
		char filename[FLEN];
		int i, j, c = 0, flag = 0;
		for (i = 0; i < irecvBuffer; i++)
		{
			if (RecvBuffer[i] == '/')
			{
				flag = 1;
				for (j = i + 1; RecvBuffer[j] != ' '; j++)
				{
					filename[c++] = RecvBuffer[j];
				}


			}
			if (flag == 1)
				break;
		}
		filename[c] = '\0';

		if (strlen(filename) == 0)  //if url doesnot contain file then default file is index.html
		{
			strcpy(filename, "index.html");
		}

		filepath.append(filename);
		cout << endl << "-------" << filename << "------" << endl;

		WIN32_FIND_DATAA FindFileData;
		HANDLE hFile = FindFirstFileA(filepath.c_str(), &FindFileData); //finding the file in directory
		if (hFile == INVALID_HANDLE_VALUE)
		{
			SenderBuffer = "HTTP/1.1 404 NOT FOUND\nContent-Type: text/plain\nContent-Length:25 \n\nError 404.File Not Found.";
			log.logg_error(ip, "Error: File Handle Error", 0);
		}

		else
		{
			//reading the content from the file

			ifstream infile(filepath);
			string s;
			string temp;
			while (!infile.eof())
			{
				getline(infile, temp);
				s = s + temp;
			}

			cout << "----------CONTENT OF FILE IS---------" << endl;
			cout << endl << s << endl << endl;


			std::string size = std::to_string(s.length());
			SenderBuffer = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
			SenderBuffer.append(size);
			SenderBuffer.append("\n\n");
			SenderBuffer.append(s);


		}//else

		int iSenderBuffer = strlen(SenderBuffer.c_str()) + 1;


		//step7 send data to client
		iSend = send(s->sAcceptSocket, SenderBuffer.c_str(), iSenderBuffer, 0);
		if (iSend == SOCKET_ERROR)
		{
			log.logg(ip, filename, 0);
			log.logg_error(ip, "Error: Data Sending Failed", 0);
			cout << "sending failed. ERROR -> " << WSAGetLastError() << endl;
			exit(0);
		}

		cout << "Data sending success " << SenderBuffer << endl;
		log.logg(ip, filename, 1);
		FindClose(hFile);
		if (s->sAcceptSocket == INVALID_SOCKET)
		{
			cout << "Socket closded" << endl;
			log.logg_error(ip, "Error: Socket Closed", 0);
			closesocket(s->sAcceptSocket);
			CloseHandle(GetCurrentThread());

		}
	}
}
Server::Server()
{
    irecvBuffer = strlen(RecvBuffer) + 1;
    filepath = STR;
    port = 8080;
    Socket sock;
	sock.createSocket();
	sock.bindSocket();
	sock.startListen();
}
void Server::acceptConnection()
{
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
