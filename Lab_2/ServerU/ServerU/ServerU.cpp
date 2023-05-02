// ServerU.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "string.h"
#include <locale>
#include "time.h"
#include "Winsock2.h"
#include <queue>

#pragma comment(lib, "WS2_32.lib")

using namespace std;

#define forever for (;;)

string GetErrorMsgText(int code)
{
	string msgText;

	switch (code)
	{
	case WSAEINTR:				 msgText = "������ ������� ��������\n";						  break;
	case WSAEACCES:				 msgText = "���������� ����������\n";						  break;
	case WSAEFAULT:				 msgText = "��������� �����\n";								  break;
	case WSAEINVAL:				 msgText = "������ � ���������\n";							  break;
	case WSAEMFILE:				 msgText = "������� ����� ������ �������\n";				  break;
	case WSAEWOULDBLOCK:		 msgText = "������ �������� ����������\n";					  break;
	case WSAEINPROGRESS:		 msgText = "�������� � �������� ��������\n";				  break;
	case WSAEALREADY: 			 msgText = "�������� ��� �����������\n";					  break;
	case WSAENOTSOCK:   		 msgText = "����� ����� �����������\n";						  break;
	case WSAEDESTADDRREQ:		 msgText = "��������� ����� ������������\n";				  break;
	case WSAEMSGSIZE:  			 msgText = "��������� ������� �������\n";				      break;
	case WSAEPROTOTYPE:			 msgText = "������������ ��� ��������� ��� ������\n";		  break;
	case WSAENOPROTOOPT:		 msgText = "������ � ����� ���������\n";					  break;
	case WSAEPROTONOSUPPORT:	 msgText = "�������� �� ��������������\n";					  break;
	case WSAESOCKTNOSUPPORT:	 msgText = "��� ������ �� ��������������\n";				  break;
	case WSAEOPNOTSUPP:			 msgText = "�������� �� ��������������\n";					  break;
	case WSAEPFNOSUPPORT:		 msgText = "��� ���������� �� ��������������\n";			  break;
	case WSAEAFNOSUPPORT:		 msgText = "��� ������� �� �������������� ����������\n";	  break;
	case WSAEADDRINUSE:			 msgText = "����� ��� ������������\n";						  break;
	case WSAEADDRNOTAVAIL:		 msgText = "����������� ����� �� ����� ���� �����������\n";	  break;
	case WSAENETDOWN:			 msgText = "���� ���������\n";								  break;
	case WSAENETUNREACH:		 msgText = "���� �� ���������\n";							  break;
	case WSAENETRESET:			 msgText = "���� ��������� ����������\n";					  break;
	case WSAECONNABORTED:		 msgText = "����������� ����� �����\n";						  break;
	case WSAECONNRESET:			 msgText = "����� �������������\n";							  break;
	case WSAENOBUFS:			 msgText = "�� ������� ������ ��� �������\n";				  break;
	case WSAEISCONN:			 msgText = "����� ��� ���������\n";							  break;
	case WSAENOTCONN:			 msgText = "����� �� ���������\n";							  break;
	case WSAESHUTDOWN:			 msgText = "������ ��������� send: ����� �������� ������\n";  break;
	case WSAETIMEDOUT:			 msgText = "���������� ���������� ��������  �������\n";		  break;
	case WSAECONNREFUSED:		 msgText = "���������� ���������\n";						  break;
	case WSAEHOSTDOWN:			 msgText = "���� � ����������������� ���������\n";			  break;
	case WSAEHOSTUNREACH:		 msgText = "��� �������� ��� �����\n";						  break;
	case WSAEPROCLIM:			 msgText = "������� ����� ���������\n";						  break;
	case WSASYSNOTREADY:		 msgText = "���� �� ��������\n";							  break;
	case WSAVERNOTSUPPORTED:	 msgText = "������ ������ ����������\n";					  break;
	case WSANOTINITIALISED:		 msgText = "�� ��������� ������������� WS2_32.DLL\n";		  break;
	case WSAEDISCON:			 msgText = "����������� ����������\n";						  break;
	case WSATYPE_NOT_FOUND:		 msgText = "����� �� ������\n";								  break;
	case WSAHOST_NOT_FOUND:		 msgText = "���� �� ������\n";								  break;
	case WSATRY_AGAIN:			 msgText = "������������������ ���� �� ������\n";			  break;
	case WSANO_RECOVERY:		 msgText = "�������������� ������\n";						  break;
	case WSANO_DATA:			 msgText = "��� ������ ������������ ����\n";				  break;
	case WSA_INVALID_HANDLE:	 msgText = "��������� ���������� �������  � �������\n";		  break;
	case WSA_INVALID_PARAMETER:	 msgText = "���� ��� ����� ���������� � �������\n";			  break;
	case WSA_IO_INCOMPLETE:		 msgText = "������ �����-������ �� � ���������� ���������\n"; break;
	case WSA_IO_PENDING:		 msgText = "�������� ���������� �����\n";					  break;
	case WSA_NOT_ENOUGH_MEMORY:	 msgText = "�� ���������� ������\n";						  break;
	case WSA_OPERATION_ABORTED:	 msgText = "�������� ����������\n";							  break;
	case WSAEINVALIDPROCTABLE:	 msgText = "��������� ������\n";							  break;
	case WSAEINVALIDPROVIDER:	 msgText = "������ � ������ �������\n";						  break;
	case WSAEPROVIDERFAILEDINIT: msgText = "���������� ���������������� ������\n";			  break;
	case WSASYSCALLFAILURE:		 msgText = "��������� ���������� ���������� ������\n";		  break;
	default:					 msgText = "Error\n";										  break;
	};

	return msgText;
}

string SetErrorMsgText(string msgText, int code)
{
	return  msgText + GetErrorMsgText(code);
};


struct CA
{
	char ipaddr[15];	
	char resource[20];	// ��� �������
	enum STATUS
	{
		NOINIT = 0,	
		INIT,
		ENTER,
		WAIT
	} status;
};

SOCKET cS;			// ����� ��� ������ ������� � ��������
SOCKADDR_IN clnt;	// ��������� ������ �������

SOCKET sS;			// ���������� ������
SOCKADDR_IN serv;	// ��������� ������ �������

CA InitCA(const char ipaddr[15], const char resource[20])	
{
	CA ca;	
	memcpy(ca.ipaddr, ipaddr, sizeof(ipaddr));	// ��� ����������� ����� ������ �� ��������� �������������� � �������������� ����������
	memcpy(ca.resource, resource, sizeof(resource));
	ca.status = CA::INIT;	

	cout << "Section initialized" << endl;
	return ca;
}

bool EnterCA(CA & ca)
{
	cout << "Enter: " << inet_ntoa(clnt.sin_addr) << ":" << clnt.sin_port<< "\t" ;
	if (ca.status == CA::INIT || ca.status == CA::WAIT)
	{
		ca.status = CA::ENTER;
		cout << "OK" << endl;
		return true;
	}
	cout << "FAIL" << endl;
	return false;
}

bool LeaveCA(CA & ca)
{
	cout << "Leave: " << inet_ntoa(clnt.sin_addr) << ":" << clnt.sin_port << "\t";
	if (ca.status == CA::ENTER)
	{
		ca.status = CA::WAIT;
		cout << "OK" << endl;
		return true;
	}

	return false;
}

bool CloseCA(CA & ca)
{
	memset(&ca, 0, sizeof(ca));			// ��� ��������� ����� ������ ��������� ��������
	cout << "Section uninitialized" << endl;
	return true;
}


int _tmain(int argc, _TCHAR* argv[])
{	
	setlocale(LC_ALL, "rus");
	WSADATA wsaData;
	serv.sin_family = AF_INET;			// ������������ IP-���������
	serv.sin_port = htons(2000);		// ���� 2000
	serv.sin_addr.s_addr = INADDR_ANY;	// ����� ����������� IP-�����

	memset(&clnt, 0, sizeof(clnt));		// �������� ������
	int lclnt = sizeof(clnt);			// ������ SOCKADDR_IN

	char ibuf[50];
	char ibuf1[2];						// ����� �����
	int libuf = 0;						// ���������� �������� ����
	int lobuf = 0;						// ���������� ������������ ����
	bool close = false;
	clock_t start, stop;
	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("Startup: ", WSAGetLastError());

		if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("socket: ", WSAGetLastError());

		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
			throw SetErrorMsgText("bind: ", WSAGetLastError());

		CA ca = InitCA("127.0.0.1", "output.txt");

		do
		{
			cout << "IP-address: " << inet_ntoa(clnt.sin_addr) << endl;

			start = clock();
			while (true)
			{
				if ((libuf = recvfrom(sS, ibuf, sizeof(ibuf), NULL, (sockaddr*)&clnt, &lclnt)) == SOCKET_ERROR)
					throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
				
				if ((ibuf[0] == 'e') && (!close))		// ���� ������ ������ �� ���� � ������
				{
					if (EnterCA(ca))
					{
						ibuf1[0] = 'o';
						if ((lobuf = sendto(sS, ibuf1, 1, NULL, (sockaddr*)&clnt, sizeof(clnt))) == SOCKET_ERROR)
							throw SetErrorMsgText("sendto: ", WSAGetLastError());
						close = true;
					}
				}
				else if (ibuf[0] == 'l')				// ���� ������ ������ �� ����� �� ������
				{
					if(LeaveCA(ca))
					{
						ibuf1[0] = 'l';
						if ((lobuf = sendto(sS, ibuf1, 1, NULL, (sockaddr*)&clnt, sizeof(clnt))) == SOCKET_ERROR)
							throw SetErrorMsgText("sendto: ", WSAGetLastError());
						close = false;
					}
				}
				else if ((ibuf[0] == 'e') && (close))	// ���� ������ ������ �� ���� � ������, �� ��� ��� ������ ������ ��������
				{
					ibuf1[0] = 'n';
					if ((lobuf = sendto(sS, ibuf1, 1, NULL, (sockaddr*)&clnt, sizeof(clnt))) == SOCKET_ERROR)
						throw SetErrorMsgText("sendto: ", WSAGetLastError());
					cout << "Denai: " << inet_ntoa(clnt.sin_addr) << ":" << clnt.sin_port << endl;
				}

				if (strcmp(ibuf, "") == 0)				// ���� ������ ����������
					break;
			}
			stop = clock();
			cout << "Time for sendto and recvfrom: " << (double)((stop - start) / CLK_TCK) << endl;
		} while (true);

		if (closesocket(sS) == SOCKET_ERROR)
			throw SetErrorMsgText("closesocket: ", WSAGetLastError());

		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
	}
	catch (string errorMsgText)
	{
		cout << endl << errorMsgText << endl;
	}

	system("pause");
	return 0;
}