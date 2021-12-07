/*
* ������� ������
* �������� 2 ����� �������� ������� ��������������
* �������� �
* ���� ��������: ��� �������� ���
*/
#ifndef _SB_TCP_H_
#define _SB_TCP_H_

#include <string>
//������� - �������� ����������� �볺���
bool SendMessageToClient(string msg);
//������� - ��������� ����������� �� �볺���
string ReceiveMessageFromClient();
//������� - �������� ����������� �������
bool SendMessageToServer(string msg);
//������� - ��������� ����������� �� �������
string ReceiveMessageFromServer();
//������ �������
void RunServer(void);
//������� �������
void StopServer(void);
//������ �볺���
void RunClient(void);
//������� �볺���
void StopClient(void);
//�������� ����� �������
string GetServerName(); 
//�������� ����� �볺���
string GetClientName();
#endif // !_SB_TCP_H_

