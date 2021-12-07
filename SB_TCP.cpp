/*
* ������� ������
* �������� 2 ����� �������� ������� ��������������
* �������� �
* ���� ��������: ��� �������� ���
*/

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <windows.h>
#include <mstcpip.h>


using namespace std;

#include "SB_TCP.h"

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "mswsock.lib")

#define PORT 666
#define SERVERIP "127.0.0.1"

char PCName[30], ClientName[30], Message[200], ServerName[30];

WSAData WSADat; // �������� WinSock (��������� ������� WSAStartup)
SOCKET Sock, Client; // ���������� �����

//�������� ����� �������
string GetServerName(){	return ServerName;}
//�������� ����� �볺���
string GetClientName(){	return ClientName;}

//������� ����������� ��������� � ������ 
string CharToStr(char *char_array) {
	string s;
	s.assign(char_array);
return s;
}
//³���������� ����������� �� ������� �� �볺���
bool SendMessageToClient(string msg) 
{
	bool retCode = false;
	strcpy(Message, msg.c_str());
	if (send(Client, Message, strlen(Message)+1, 0) != SOCKET_ERROR) retCode = true; else retCode = false;
	return retCode;
}
//��������� ����������� �� �볺���
string ReceiveMessageFromClient()
{
	bool retCode = false;
	string msg;
	if (recv(Client, Message, 200, 0) != SOCKET_ERROR)
	{
		retCode = true;
		msg = CharToStr(Message);
	}
	else retCode = false;
	return msg;
}
//³���������� ����������� �� �볺��� �� ������� 
bool SendMessageToServer(string msg)
{
	bool retCode = false;
	strcpy(Message, msg.c_str());
	if (send(Sock, Message, strlen(Message)+1, 0) != SOCKET_ERROR) retCode = true; else retCode = false;
	return retCode;
}
//��������� ����������� �� �������
string ReceiveMessageFromServer()
{
	bool retCode = false; 
	string msg;
	if (recv(Sock, Message, 200, 0) != SOCKET_ERROR) { 
		retCode = true; 
	    msg = CharToStr(Message); 
	}
	else { retCode = false; }	
	return msg;
}
//������� - ������� �������
void StopServer(void) {
	// �������� ������� � ��������� ������ � WinSock
	closesocket(Sock);
	closesocket(Client);
	WSACleanup();
}
//������� - ������ �������
void RunServer(void) {
	int retCode = WSAStartup(0x0202, &WSADat); // ������������� WinSock
								// 0x0202 - ������ WinSock. ����� ���� 1.0, 1.1, 2.0, 2.2
								// WSADat - ���������, ���� ����� �������� ���. �������������
	gethostname(PCName, 30); // ��������� ����� �������� ��
	sockaddr_in sin;
	sin.sin_family = AF_INET; // ��� ������
	sin.sin_addr.s_addr = 0;  // IP-����� ������� (���� �������� ������� ����� 0)
	sin.sin_port = htons(PORT); // ����� ����� �������
	Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // �������� ������
	bind(Sock, (sockaddr*)&sin, sizeof(sin)); // ���������� ���������� ������ � ������� sin
	// ***** �������� �������
//	cout << "Hostname = "<< hostname << endl;
	cout << "������ �� �볺���..." << endl;

	listen(Sock, SOMAXCONN); // ������������� ������ �������� (��� ����������� �������)
	Client = accept(Sock, (sockaddr*)&sin, 0); // �������� �������
	DWORD timeout = 2000; // ������� ��� ��������� �����������
	setsockopt(Client, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)); // ������������ �������� �� ��������� �����������
	recv(Client, ClientName, 30, 0); // ��������� ����� ���������� �������
	send(Client, PCName, strlen(PCName) + 1, 0); // �������� ����� ����� ���������� (�������)
	cout << "�볺��� ( " << GetClientName() << " ) ���������!" << endl;
}
//������� - ������ �볺���
void RunClient(void) {
	int retCode = WSAStartup(0x0202, &WSADat); // ������������� WinSock
								 // 0x0202 - ������ WinSock. ����� ���� 1.0, 1.1, 2.0, 2.2
								 // WSADat - ���������, ���� ����� �������� ���. �������������
//	char hostnameServer[30] = "127.0.0.1"; char* hns;
	gethostname(PCName, 30); // ��������� ����� �������� ��
//	cout << "IP �������...(�� ��������� 127,0,0,1)" << endl;
//	const char* SIP = (hostnameServer)?hostnameServer:SERVERIP;
	sockaddr_in sin;
	sin.sin_family = AF_INET; // ��� ������
	sin.sin_addr.s_addr = inet_addr(SERVERIP); // IP-����� ������� (���� �������� ������� ����� 0)
	sin.sin_port = htons(PORT); // ����� ����� �������
	Sock = socket(AF_INET, SOCK_STREAM, 0); // �������� ������
	DWORD timeout = 2000; // ���� ��������
	setsockopt(Sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)); // ������������ ��������
	// ***** ����������� � �������
	cout << "ϳ��������� �� �������..." << endl;

	if (connect(Sock, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR) {
		cout << "������� ���������� �� �������!\n";
		system("pause");
		goto End;
	}
	send(Sock, PCName, strlen(PCName) + 1, 0); // �������� ����� ����� ���������� (�������)
	recv(Sock, ServerName, 30, 0); // ��������� ����� ���������� �������
	cout << "ϳ�������� �� ������� \"" << GetServerName() << "\" ���������!" << endl;
	return;

End: {}
	//StopClient();
}
//������� - ������� �볺���
void StopClient(void) {// �������� ������� � ��������� ������ � WinSock
	closesocket(Sock);
	WSACleanup();
}