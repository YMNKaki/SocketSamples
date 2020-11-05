// C++のクライアントアプリ
// サーバークライアント間の約束事はIPおよびポート番号を一致させること、閉じるときの合図を揃えておくことです

#include <conio.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment( lib, "ws2_32.lib" )

int main()
{
	WSADATA wsaData;
	struct sockaddr_in server;
	SOCKET sock;
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	// 接続先指定用構造体の準備
	server.sin_family = AF_INET;
	server.sin_port = htons(3000);
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);

	// サーバに接続
	printf("Press Enter\n");
	getchar();
	sock = socket(AF_INET, SOCK_STREAM, 0);
	connect(sock, (struct sockaddr*)&server, sizeof(server));
	printf("connect\n");

	int count = 0;
	while (true)
	{

		//Enterが押されたら'finish'を送りエスケープ
		if (_kbhit() && _getch() == 13) {
			//送信
			const char* sendmsg = "finish\n";
			send(sock, sendmsg, strlen(sendmsg), 0);
			printf("client -> %s\n", sendmsg);
			//受信
			char buf[1024];
			memset(buf, 0, sizeof(buf));
			recv(sock, buf, sizeof(buf), 0);
			printf("server -> %s\n", buf);
			break;
		}

		//送信
		count++;
		std::string s = std::to_string(count);
		s.push_back('\n');
		const char* sendmsg = s.data();
		send(sock, sendmsg, strlen(sendmsg), 0);
		printf("client -> %s\n", sendmsg);
		//受信
		char buf[1024];
		memset(buf, 0, sizeof(buf));
		recv(sock, buf, sizeof(buf), 0);
		printf("server -> %s\n", buf);
		Sleep(500);
	}

	closesocket(sock);
	WSACleanup();
	printf("Press Enter\n");
	getchar();

	return 0;
}

