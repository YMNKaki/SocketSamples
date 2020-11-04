// C++のサーバーアプリ
// サーバークライアント間の約束事はIPおよびポート番号を一致させること、閉じるときの合図を揃えておくことです

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

int main()
{
	// ポート番号，ソケット
	int srcSocket;  // 自分
	int dstSocket;  // 相手

	// sockaddr_in 構造体
	struct sockaddr_in srcAddr;
	struct sockaddr_in dstAddr;
	int dstAddrSize = sizeof(dstAddr);
	int status;
	int numrcv;
	char buffer[1024];

	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);

	// sockaddr_in 構造体のセット
	memset(&srcAddr, 0, sizeof(srcAddr));
	srcAddr.sin_port = htons(3000);
	srcAddr.sin_family = AF_INET;
	srcAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// ソケットの生成（ストリーム型）
	srcSocket = socket(AF_INET, SOCK_STREAM, 0);
	// ソケットのバインド
	bind(srcSocket, (struct sockaddr*)&srcAddr, sizeof(srcAddr));
	// 接続の許可
	listen(srcSocket, 1);

	dstSocket = accept(srcSocket, (struct sockaddr*)&dstAddr, &dstAddrSize);
	printf("connect %s \n", inet_ntoa(dstAddr.sin_addr));

	int count = 0;
	bool loop = true;
	while (loop) {
		//受信
		numrcv = recv(dstSocket, buffer, sizeof(char) * 1024, 0);
		if (numrcv == 0 || numrcv == -1) {
			loop = false;
			status = closesocket(dstSocket);
			break;
		}
		printf("client -> %s", buffer);
		for (int i = 0; i < numrcv; i++) { // bufの中の小文字を大文字に変換
			buffer[i] = toupper(buffer[i]);
		}
		//送信
		send(dstSocket, buffer, sizeof(char) * 1024, 0);
		printf("server -> %s \n", buffer);
	}

	WSACleanup();
	return(0);
}

