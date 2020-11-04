// C++のクライアントアプリ
// サーバークライアント間の約束事はIPおよびポート番号を一致させること、閉じるときの合図を揃えておくことです

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <conio.h>

int main()
{
	char buffer[1024];
	int dstSocket;
	struct sockaddr_in dstAddr;
	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);

	// sockaddr_in 構造体のセット
	memset(&dstAddr, 0, sizeof(dstAddr));
	dstAddr.sin_port = htons(3000);
	dstAddr.sin_family = AF_INET;
	dstAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// ソケットの生成、ストリームを流す
	dstSocket = socket(AF_INET, SOCK_STREAM, 0);

	//接続
	if (connect(dstSocket, (struct sockaddr*)&dstAddr, sizeof(dstAddr))) return(-1);
	printf("connect with 127.0.0.1\n");

	int count = 0;
	bool loop = true;
	while (loop) {
		if (_kbhit())
		{
			if (_getch() == 13)
			{
				const char* sendmsg = "finish\n";
				send(dstSocket, sendmsg, 1024, 0);
				printf("client -> %s\n", sendmsg);
				loop = false;
			}
		}
		//送信
		count++;
		const char* sendmsg = (char*)count;
		send(dstSocket, sendmsg, 1024, 0);
		printf("client -> %s\n", sendmsg);
		//受信
		recv(dstSocket, buffer, 1024, 0);
		printf("server -> %s\n", buffer);
		Sleep(500);
	}

	closesocket(dstSocket);
	WSACleanup();
	return(0);
}

