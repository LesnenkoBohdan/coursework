/*
* Курсова робота
* Студента 2 курсу ЛЕСНЕНКО Богдана Олександровича
* Завдання №
* Тема завдання: Гра Морський Бій
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

WSAData WSADat; // Свойства WinSock (результат функции WSAStartup)
SOCKET Sock, Client; // Клиентский сокет

//Отримати назву серверу
string GetServerName(){	return ServerName;}
//Отримати назву клієнта
string GetClientName(){	return ClientName;}

//Функція переведення вказівника в строку 
string CharToStr(char *char_array) {
	string s;
	s.assign(char_array);
return s;
}
//Відправлення повідомлення від Сервера до клієнта
bool SendMessageToClient(string msg) 
{
	bool retCode = false;
	strcpy(Message, msg.c_str());
	if (send(Client, Message, strlen(Message)+1, 0) != SOCKET_ERROR) retCode = true; else retCode = false;
	return retCode;
}
//Отримання повідомлення від клієнта
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
//Відправлення повідомлення від Клієнта до Сервера 
bool SendMessageToServer(string msg)
{
	bool retCode = false;
	strcpy(Message, msg.c_str());
	if (send(Sock, Message, strlen(Message)+1, 0) != SOCKET_ERROR) retCode = true; else retCode = false;
	return retCode;
}
//Отримання повідомлення від Сервера
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
//Функція - зупинка Сервера
void StopServer(void) {
	// Закрытие сокетов и окончание работы с WinSock
	closesocket(Sock);
	closesocket(Client);
	WSACleanup();
}
//Функція - запуск Сервера
void RunServer(void) {
	int retCode = WSAStartup(0x0202, &WSADat); // Инициализация WinSock
								// 0x0202 - версия WinSock. Может быть 1.0, 1.1, 2.0, 2.2
								// WSADat - структура, куда будут занесены рез. инициализации
	gethostname(PCName, 30); // Получение имени текущего ПК
	sockaddr_in sin;
	sin.sin_family = AF_INET; // Тип адреса
	sin.sin_addr.s_addr = 0;  // IP-адрес сервера (пори создании сервера можно 0)
	sin.sin_port = htons(PORT); // Номер порта сервера
	Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Создание сокета
	bind(Sock, (sockaddr*)&sin, sizeof(sin)); // Связывание созданного сокета с адресом sin
	// ***** Ожидание клиента
//	cout << "Hostname = "<< hostname << endl;
	cout << "Чекаємо на клієнта..." << endl;

	listen(Sock, SOMAXCONN); // Прослушивание сокета сервером (для подключения клиента)
	Client = accept(Sock, (sockaddr*)&sin, 0); // Ожидание клиента
	DWORD timeout = 2000; // таймаут для отримання повідомлення
	setsockopt(Client, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)); // встановлення таймаута на отримання повідомлення
	recv(Client, ClientName, 30, 0); // Получение имени компьютера клиента
	send(Client, PCName, strlen(PCName) + 1, 0); // Отправка имени этого компьютера (сервера)
	cout << "Клієнта ( " << GetClientName() << " ) підключено!" << endl;
}
//Функція - запуск Клієнта
void RunClient(void) {
	int retCode = WSAStartup(0x0202, &WSADat); // Инициализация WinSock
								 // 0x0202 - версия WinSock. Может быть 1.0, 1.1, 2.0, 2.2
								 // WSADat - структура, куда будут занесены рез. инициализации
//	char hostnameServer[30] = "127.0.0.1"; char* hns;
	gethostname(PCName, 30); // Получение имени текущего ПК
//	cout << "IP сервера...(по умолчанию 127,0,0,1)" << endl;
//	const char* SIP = (hostnameServer)?hostnameServer:SERVERIP;
	sockaddr_in sin;
	sin.sin_family = AF_INET; // Тип адреса
	sin.sin_addr.s_addr = inet_addr(SERVERIP); // IP-адрес сервера (пори создании сервера можно 0)
	sin.sin_port = htons(PORT); // Номер порта сервера
	Sock = socket(AF_INET, SOCK_STREAM, 0); // Создание сокета
	DWORD timeout = 2000; // зміна таймаута
	setsockopt(Sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)); // встановлення таймаута
	// ***** Подключение к серверу
	cout << "Підключення до сервера..." << endl;

	if (connect(Sock, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR) {
		cout << "Помилка підключення до сервера!\n";
		system("pause");
		goto End;
	}
	send(Sock, PCName, strlen(PCName) + 1, 0); // Отправка имени этого компьютера (клиента)
	recv(Sock, ServerName, 30, 0); // Получение имени компьютера сервера
	cout << "Підключено до сервера \"" << GetServerName() << "\" проведено!" << endl;
	return;

End: {}
	//StopClient();
}
//Функція - зупинка Клієнта
void StopClient(void) {// Закрытие сокетов и окончание работы с WinSock
	closesocket(Sock);
	WSACleanup();
}