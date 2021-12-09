/*
* Курсова робота
* Студента 2 курсу ЛЕСНЕНКО Богдана Олександровича
* Завдання №
* Тема завдання: Гра Морський Бій
*/
#ifndef _SB_TCP_H_
#define _SB_TCP_H_

#include <string>
//Функція - відправка повідомлення Клієнту
bool SendMessageToClient(string msg);
//Функція - отримання повідомлення від Клієнта
string ReceiveMessageFromClient();
//Функція - відправка повідомлення Серверу
bool SendMessageToServer(string msg);
//Функція - отримання повідомлення від Сервера
string ReceiveMessageFromServer();
//Запуск Сервера
void RunServer(void);
//Зупинка Сервера
void StopServer(void);
//Запуск Клієнта
void RunClient(void);
//Зупинка Клієнта
void StopClient(void);
//Отримати назву Сервера
string GetServerName(); 
//Отримати назву Клієнта
string GetClientName();
#endif // !_SB_TCP_H_

