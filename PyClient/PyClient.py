# Pythonのクライアントプログラム
# サーバークライアント間の約束事はIPおよびポート番号を一致させること、閉じるときの合図を揃えておくことです

import socket
import msvcrt
import time

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client:
    print('Enterを押すと進みます')
    while True:
        key = msvcrt.getch()
        if ord(key) == 13 : break
    client.connect(('127.0.0.1', 3000))
    print('接続成功');
    count = 0
    while True:
        # Enterキー入力があったら'finish'を送信。サーバーの受諾を待ってからループを抜ける
        if msvcrt.kbhit():
            key = msvcrt.getch()
            if ord(key) == 13 : 
                sendmsg = 'finish\n'
                client.send(sendmsg.encode('utf-8'))
                print('client -> {}'.format(sendmsg))
                data = client.recv(1024)
                res = data.decode()
                print('server -> {}'.format(res))
                break
        # 通常のループ処理
        count += 1
        sendmsg = '{}\n'.format(count)              # 送るデータ。ここでは単純な文字列
        client.send(sendmsg.encode('utf-8'))
        print('client -> {}'.format(sendmsg))
        data = client.recv(1024)                    # 受け取るデータ
        res = data.decode()
        print('server -> {}'.format(res))
        time.sleep(0.5)