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
	case WSAEINTR:				 msgText = "Работа функции прервана\n";						  break;
	case WSAEACCES:				 msgText = "Разрешение отвергнуто\n";						  break;
	case WSAEFAULT:				 msgText = "Ошибочный адрес\n";								  break;
	case WSAEINVAL:				 msgText = "Ошибка в аргументе\n";							  break;
	case WSAEMFILE:				 msgText = "Слишком много файлов открыто\n";				  break;
	case WSAEWOULDBLOCK:		 msgText = "Ресурс временно недоступен\n";					  break;
	case WSAEINPROGRESS:		 msgText = "Операция в процессе развития\n";				  break;
	case WSAEALREADY: 			 msgText = "Операция уже выполняется\n";					  break;
	case WSAENOTSOCK:   		 msgText = "Сокет задан неправильно\n";						  break;
	case WSAEDESTADDRREQ:		 msgText = "Требуется адрес расположения\n";				  break;
	case WSAEMSGSIZE:  			 msgText = "Сообщение слишком длинное\n";				      break;
	case WSAEPROTOTYPE:			 msgText = "Неправильный тип протокола для сокета\n";		  break;
	case WSAENOPROTOOPT:		 msgText = "Ошибка в опции протокола\n";					  break;
	case WSAEPROTONOSUPPORT:	 msgText = "Протокол не поддерживается\n";					  break;
	case WSAESOCKTNOSUPPORT:	 msgText = "Тип сокета не поддерживается\n";				  break;
	case WSAEOPNOTSUPP:			 msgText = "Операция не поддерживается\n";					  break;
	case WSAEPFNOSUPPORT:		 msgText = "Тип протоколов не поддерживается\n";			  break;
	case WSAEAFNOSUPPORT:		 msgText = "Тип адресов не поддерживается протоколом\n";	  break;
	case WSAEADDRINUSE:			 msgText = "Адрес уже используется\n";						  break;
	case WSAEADDRNOTAVAIL:		 msgText = "Запрошенный адрес не может быть использован\n";	  break;
	case WSAENETDOWN:			 msgText = "Сеть отключена\n";								  break;
	case WSAENETUNREACH:		 msgText = "Сеть не достижима\n";							  break;
	case WSAENETRESET:			 msgText = "Сеть разорвала соединение\n";					  break;
	case WSAECONNABORTED:		 msgText = "Программный отказ связи\n";						  break;
	case WSAECONNRESET:			 msgText = "Связь восстановлена\n";							  break;
	case WSAENOBUFS:			 msgText = "Не хватает памяти для буферов\n";				  break;
	case WSAEISCONN:			 msgText = "Сокет уже подключен\n";							  break;
	case WSAENOTCONN:			 msgText = "Сокет не подключен\n";							  break;
	case WSAESHUTDOWN:			 msgText = "Нельзя выполнить send: сокет завершил работу\n";  break;
	case WSAETIMEDOUT:			 msgText = "Закончился отведенный интервал  времени\n";		  break;
	case WSAECONNREFUSED:		 msgText = "Соединение отклонено\n";						  break;
	case WSAEHOSTDOWN:			 msgText = "Хост в неработоспособном состоянии\n";			  break;
	case WSAEHOSTUNREACH:		 msgText = "Нет маршрута для хоста\n";						  break;
	case WSAEPROCLIM:			 msgText = "Слишком много процессов\n";						  break;
	case WSASYSNOTREADY:		 msgText = "Сеть не доступна\n";							  break;
	case WSAVERNOTSUPPORTED:	 msgText = "Данная версия недоступна\n";					  break;
	case WSANOTINITIALISED:		 msgText = "Не выполнена инициализация WS2_32.DLL\n";		  break;
	case WSAEDISCON:			 msgText = "Выполняется отключение\n";						  break;
	case WSATYPE_NOT_FOUND:		 msgText = "Класс не найден\n";								  break;
	case WSAHOST_NOT_FOUND:		 msgText = "Хост не найден\n";								  break;
	case WSATRY_AGAIN:			 msgText = "Неавторизированный хост не найден\n";			  break;
	case WSANO_RECOVERY:		 msgText = "Неопределенная ошибка\n";						  break;
	case WSANO_DATA:			 msgText = "Нет записи запрошенного типа\n";				  break;
	case WSA_INVALID_HANDLE:	 msgText = "Указанный дескриптор события  с ошибкой\n";		  break;
	case WSA_INVALID_PARAMETER:	 msgText = "Один или более параметров с ошибкой\n";			  break;
	case WSA_IO_INCOMPLETE:		 msgText = "Объект ввода-вывода не в сигнальном состоянии\n"; break;
	case WSA_IO_PENDING:		 msgText = "Операция завершится позже\n";					  break;
	case WSA_NOT_ENOUGH_MEMORY:	 msgText = "Не достаточно памяти\n";						  break;
	case WSA_OPERATION_ABORTED:	 msgText = "Операция отвергнута\n";							  break;
	case WSAEINVALIDPROCTABLE:	 msgText = "Ошибочный сервис\n";							  break;
	case WSAEINVALIDPROVIDER:	 msgText = "Ошибка в версии сервиса\n";						  break;
	case WSAEPROVIDERFAILEDINIT: msgText = "Невозможно инициализировать сервис\n";			  break;
	case WSASYSCALLFAILURE:		 msgText = "Аварийное завершение системного вызова\n";		  break;
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
	char resource[20];	// имя ресурса
	enum STATUS
	{
		NOINIT = 0,	
		INIT,
		ENTER,
		WAIT
	} status;
};

SOCKET cS;			// сокет для обмена данными с клиентом
SOCKADDR_IN clnt;	// параметры сокета клиента

SOCKET sS;			// дескриптор сокета
SOCKADDR_IN serv;	// параметры сокета сервера

CA InitCA(const char ipaddr[15], const char resource[20])	
{
	CA ca;	
	memcpy(ca.ipaddr, ipaddr, sizeof(ipaddr));	// для копирования блока памяти из исходного местоположения в местоположение назначения
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
	memset(&ca, 0, sizeof(ca));			// для установки блоку памяти заданного значения
	cout << "Section uninitialized" << endl;
	return true;
}


int _tmain(int argc, _TCHAR* argv[])
{	
	setlocale(LC_ALL, "rus");
	WSADATA wsaData;
	serv.sin_family = AF_INET;			// используется IP-адресация
	serv.sin_port = htons(2000);		// порт 2000
	serv.sin_addr.s_addr = INADDR_ANY;	// любой собственный IP-адрес

	memset(&clnt, 0, sizeof(clnt));		// обнулить память
	int lclnt = sizeof(clnt);			// размер SOCKADDR_IN

	char ibuf[50];
	char ibuf1[2];						// буфер ввода
	int libuf = 0;						// количество принятых байт
	int lobuf = 0;						// количество отправленных байт
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
				
				if ((ibuf[0] == 'e') && (!close))		// если пришёл запрос на вход в секцию
				{
					if (EnterCA(ca))
					{
						ibuf1[0] = 'o';
						if ((lobuf = sendto(sS, ibuf1, 1, NULL, (sockaddr*)&clnt, sizeof(clnt))) == SOCKET_ERROR)
							throw SetErrorMsgText("sendto: ", WSAGetLastError());
						close = true;
					}
				}
				else if (ibuf[0] == 'l')				// если пришёл запрос на выход из секции
				{
					if(LeaveCA(ca))
					{
						ibuf1[0] = 'l';
						if ((lobuf = sendto(sS, ibuf1, 1, NULL, (sockaddr*)&clnt, sizeof(clnt))) == SOCKET_ERROR)
							throw SetErrorMsgText("sendto: ", WSAGetLastError());
						close = false;
					}
				}
				else if ((ibuf[0] == 'e') && (close))	// если пришёл запрос на вход в секцию, но она уже занята другим клиентом
				{
					ibuf1[0] = 'n';
					if ((lobuf = sendto(sS, ibuf1, 1, NULL, (sockaddr*)&clnt, sizeof(clnt))) == SOCKET_ERROR)
						throw SetErrorMsgText("sendto: ", WSAGetLastError());
					cout << "Denai: " << inet_ntoa(clnt.sin_addr) << ":" << clnt.sin_port << endl;
				}

				if (strcmp(ibuf, "") == 0)				// если клиент отключился
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