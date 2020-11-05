// C++のサーバーアプリ
// サーバークライアント間の約束事はIPおよびポート番号を一致させること、閉じるときの合図を揃えておくことです

#include <string.h>
#include <sstream>
#include <winsock2.h>
#pragma comment( lib, "ws2_32.lib" )

int main()
{
	WSADATA wsaData;
	SOCKET sock0;
	struct sockaddr_in addr;
	struct sockaddr_in client;
	int len;
	SOCKET sock;
	

	WSAStartup(MAKEWORD(2, 0), &wsaData);

	// ソケットの作成
	addr.sin_family = AF_INET;
	addr.sin_port = htons(3000);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	sock0 = socket(AF_INET, SOCK_STREAM, 0);
	bind(sock0, (struct sockaddr*)&addr, sizeof(addr));
	

	// TCPクライアントからの接続要求を受け付ける
	printf("start listen\n");
	listen(sock0, 5);
	len = sizeof(client);
	sock = accept(sock0, (struct sockaddr*)&client, &len);
	printf("connect\n");

	int count = 0;
	while (true)
	{
		//受信
		char buf[1024];
		memset(buf, 0, sizeof(buf));
		recv(sock, buf, sizeof(buf), 0);
		printf("client -> %s\n", buf);

		//'finish'が来たとき
		if (strncmp(buf, "finish\n", 8) == 0) {
			const char* sendmsg = "accept finish\n";
			send(sock, sendmsg, strlen(sendmsg), 0);
			printf("server -> %s\n", sendmsg);
			break;
		}
		//通常の送信
		else {
			count++;
			std::string s = std::to_string(count);
			s.push_back('\n');
			const char* sendmsg = s.data();
			send(sock, sendmsg, strlen(sendmsg), 0);
			printf("server -> %s\n", sendmsg);
		}
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}

