// C++のクライアントアプリ
// サーバークライアント間の約束事はIPおよびポート番号を一致させること、閉じるときの合図を揃えておくことです

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
	char buf[1024];
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	// 接続先指定用構造体の準備
	server.sin_family = AF_INET;
	server.sin_port = htons(3000);
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);

	// サーバに接続
	sock = socket(AF_INET, SOCK_STREAM, 0);
	connect(sock, (struct sockaddr*)&server, sizeof(server));

	int count = 0;
	bool loop = true;
	while (loop)
	{
		//送信
		count++;
		std::string s = std::to_string(count);
		s.push_back('\n');
		const char* sendmsg = s.data();
		send(sock, sendmsg, strlen(sendmsg), 0);
		printf("client -> %s", sendmsg);
		//受信
		memset(buf, 0, sizeof(buf));
		int n = recv(sock, buf, sizeof(buf), 0);
		printf("server -> %s\n", buf);
		Sleep(500);
	}
	// winsock2の終了処理
	WSACleanup();

	return 0;
}

