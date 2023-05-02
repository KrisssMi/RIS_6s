#include "stdafx.h"
#include "Winsock2.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <ctime>
#include <wchar.h>

#pragma comment(lib, "WS2_32.lib")

typedef HANDLE HDFS;

using namespace std;

string GetErrorMsgText (int code)
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

string SetErrorMsgText (string msgText, int code)
{
	return msgText + GetErrorMsgText(code);
};

#include <iostream>
#include <fstream>
#include <WinSock2.h>
#include <ctime>
#include <string>


int main()
{
	setlocale(LC_ALL, "rus");
	WSADATA wsaData;

	SOCKET cC;						// ��������� �����
	SOCKADDR_IN serv;				// ��������� ������ �������
	serv.sin_family = AF_INET;		// ������������ IP-���������
	serv.sin_port = htons(2000);	// TCP-���� 2000
	serv.sin_addr.s_addr = inet_addr("127.0.0.1"); // ����� �������

	SOCKADDR_IN clnt;				// ��������� ������ �������
	memset(&clnt, 0, sizeof(clnt)); // �������� ������
	int lclnt = sizeof(clnt);		// ������ SOCKADDR_IN
	int lcserv = sizeof(serv);

	char ibuf[1];					// ����� ������
	int lobuf = 0;					// ���������� ������������ ����
	int libuf = 0;					// ���������� �������� ����
	clock_t start, stop;
	
	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("Startup: ", WSAGetLastError());

		if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("socket: ", WSAGetLastError());

		int count;					// ���������� ������� �����������
		cout << "Client\n ";

		start = clock();
		while (true)
		{
			string obuf = "enter";
			if ((lobuf = sendto(cC, obuf.c_str(), strlen(obuf.c_str()) + 1, NULL, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
				throw SetErrorMsgText("sendto: ", WSAGetLastError());	
			if ((libuf = recvfrom(cC, ibuf, 1, NULL, (sockaddr*)&serv, &lcserv)) == SOCKET_ERROR)
				throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
			if (ibuf[0] == 'o')		// ��� ������ 
			{
				cout << "Now open" << endl;
				Sleep(5000);

				string file_name = "Z:/output.txt";

				ofstream fout(file_name, ios::app);	// �������� ����� ��� ������
				for (int i = 0; i < 5; i++)	
				{
					cout << "Write " << "Client 1" << i + 1 << endl;
					fout << "Client 1";
					fout << "\n";
				}
				fout << "\n";

				fout.close();

				obuf = "leave";
				if ((lobuf = sendto(cC, obuf.c_str(), strlen(obuf.c_str()) + 1, NULL, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)	
					throw SetErrorMsgText("sendto: ", WSAGetLastError());
				if ((libuf = recvfrom(cC, ibuf, 1, NULL, (sockaddr*)&serv, &lcserv)) == SOCKET_ERROR)
					throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
				if (ibuf[0] == 'l') cout << "Leave" << endl;
				Sleep(2000);
			}
			if (ibuf[0] == 'n')	// ������ n - ������ �����
			{
				cout << "Wait..." << endl;
				Sleep(2000);
			}
		}

		string obuf = "";
		if ((lobuf = sendto(cC, obuf.c_str(), strlen(obuf.c_str()) + 1, NULL, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
			throw SetErrorMsgText("sendto: ", WSAGetLastError());

		stop = clock();
		cout << "Time for sendto and recvfrom: " << (double)((stop - start) / CLK_TCK) << endl;

		if (closesocket(cC) == SOCKET_ERROR)
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