#include "pch.h"
#include "Winsock2.h"
#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include "Errors.h"
#pragma comment(lib, "WS2_32.lib")
using namespace std;

//#pragma once
//struct GetSync
//{
//	char cmd[4];
//	int curValue;
//};
//
//struct SetSync
//{
//	char cmd[4];
//	int correction;
//};
//
//
//int main() {
//	WSADATA wsaData;
//	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
//		printf("Startup error: %d", WSAGetLastError());
//
//	SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//	if (clientSocket == INVALID_SOCKET) {
//		printf("Socket creating failed! Error: %d\n", WSAGetLastError());
//		return 1;
//	}
//
//	SOCKADDR_IN serverInfo;
//	serverInfo.sin_family = AF_INET;
//	serverInfo.sin_port = htons(2000);
//	serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
//	sockaddr* info = (sockaddr*)&serverInfo;
//
//
//	int infoLength = sizeof(SOCKADDR_IN);
//	char* buffer = new char[8];
//	memcpy(buffer, "connect", 8);
//	if (sendto(clientSocket, buffer, strlen(buffer), NULL, info, infoLength) == SOCKET_ERROR) {
//		printf("Sending 'connect' failed! Error: %d\n", WSAGetLastError());
//		return 1;
//	}
//
//	ZeroMemory(buffer, 8);
//
//	int bytesCount = recvfrom(clientSocket, buffer, 8, 0, info, &infoLength);
//	if (bytesCount == SOCKET_ERROR) {
//		printf("Can't recieve data! Error: %d", WSAGetLastError());
//		return 1;
//	}
//
//	buffer[bytesCount] = '\0';
//	printf("Got from server: %s, server port: %d\n", buffer, htons(serverInfo.sin_port));
//
//	GetSync getSync;
//	memcpy(getSync.cmd, "SYNC", 4);
//	getSync.curValue = 0;
//
//	SetSync setSync;
//
//	int Tc = 1000;
//	printf("Tc = %d\n", Tc);
//
//	for (int i = 0; i < 10; i++)
//	{
//		ZeroMemory(buffer, 8);
//
//		Sleep(Tc);
//		if (i != 0)
//			getSync.curValue++;
//		memcpy(buffer, &getSync, 8);
//		if (sendto(clientSocket, buffer, 8, 0, info, infoLength) == SOCKET_ERROR) {
//			printf("Sending data failed on %d iteration. Error: %d", i, WSAGetLastError());
//			continue;
//		}
//
//		ZeroMemory(buffer, 8);
//
//		if (recvfrom(clientSocket, buffer, 8, 0, info, &infoLength) == SOCKET_ERROR) {
//			printf("Data recieving failed on %d iteration. Error: %d", i, WSAGetLastError());
//			continue;
//		}
//
//		memcpy(&setSync, buffer, 8);
//		getSync.curValue += setSync.correction;
//	}
//
//	getchar();
//
//	closesocket(clientSocket);
//	WSACleanup();
//
//	return 0;
//}
struct GETSINCHRO
{
	DWORD64 mTime;
};

struct SETSINCHRO
{
	__int64 corrTime;
};

DWORD64 GetLocalUnixTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int main()
{
	GetLocalUnixTime();

	string IP;
	cout << "Enter IP of local NTP-server: ";
	cin >> IP;

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		throw SetErrorMsgText("Startup: ", WSAGetLastError());

	while (true) {
		int TC = 1000;
		int maxcorr = 0, mincorr = INT_MAX;
		__int64 mcorr = 0;
		__int64 mCcOs = 0;

		DWORD64 ClientTime = 0;
		SOCKET clientSocket;
		cout << "Enter TC: ";
		cin >> TC;

		try
		{
			if ((clientSocket = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
				throw SetErrorMsgText("Socket: ", WSAGetLastError());

			SOCKADDR_IN serverParameters;
			serverParameters.sin_family = AF_INET;
			serverParameters.sin_port = htons(2000);
			serverParameters.sin_addr.s_addr = inet_addr(IP.c_str());

			GETSINCHRO gs;
			SETSINCHRO ss;
			ss.corrTime = 0;
			gs.mTime = 0;

			sendto(clientSocket, (char*)&gs, sizeof(gs), 0, (sockaddr*)&serverParameters, sizeof(serverParameters));
			int lc = sizeof(serverParameters);
			if ((recvfrom(clientSocket, (char*)&ss, sizeof(ss), NULL, (sockaddr*)&serverParameters, &lc)) == SOCKET_ERROR)
				throw SetErrorMsgText("Get time: ", WSAGetLastError());
			ClientTime = ss.corrTime;
			cout << " * Init sync:" << ss.corrTime << endl << endl;

			for (int i = 0; i < 10; i++)
			{
				Sleep(TC);
				//посылаем серверу запрос со своим временем
				ClientTime = gs.mTime = (DWORD64)ClientTime + TC;
				sendto(clientSocket, (char*)&gs, sizeof(gs), 0, (sockaddr*)&serverParameters, sizeof(serverParameters));

				//получаем с сервера корректировку
				int lc = sizeof(serverParameters);
				if ((recvfrom(clientSocket, (char*)&ss, sizeof(ss), NULL, (sockaddr*)&serverParameters, &lc)) == SOCKET_ERROR)
					throw SetErrorMsgText("RecvFrom: ", WSAGetLastError());

				DWORD64 localUnixTime = GetLocalUnixTime();
				ClientTime = (ClientTime + ss.corrTime);
				mcorr = (mcorr + ss.corrTime);
				mCcOs = (mCcOs + (ClientTime - localUnixTime));
				maxcorr = (maxcorr < ss.corrTime) ? ss.corrTime : maxcorr;
				mincorr = (mincorr > ss.corrTime) ? ss.corrTime : mincorr;

				cout << "Client time: " << ClientTime << " Local time: " << localUnixTime << " Correction: " << ss.corrTime << endl;
				if (ClientTime > localUnixTime)
				{
					cout << "The local time is " << ClientTime - localUnixTime << " behind of the global" << endl;
				}
				else
				{
					cout << "The local time is " << localUnixTime - ClientTime << " ms ahead of the the global" << endl;
				}
				cout << endl;
			}
			cout << endl << "delay on the client: " << TC << endl;
			cout << "avg correction: " << mcorr / 10.0 << endl;
			cout << "avg CC - OStime: " << mCcOs / 10.0 << endl;
			cout << "max correction: " << maxcorr << endl;
			cout << "min correction: " << mincorr << endl << endl;

			if (closesocket(clientSocket) == SOCKET_ERROR)
				throw SetErrorMsgText("Closesocket: ", WSAGetLastError());
		}
		catch (string errorMsgText)
		{
			cout << endl << errorMsgText << endl;
		}

		system("pause");
	}
	if (WSACleanup() == SOCKET_ERROR)
		throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
	return 0;
}