# Pythonのサーバープログラム
# サーバークライアント間の約束事はIPおよびポート番号を一致させること、閉じるときの合図を揃えておくことです

import socket

serversock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serversock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
serversock.bind(('127.0.0.1', 3000))
serversock.listen(5)

print('Waiting for connections...')
clientsock, client_address = serversock.accept()

with clientsock, serversock:
    count = 0
    while True:
        count += 1
        rcvmsg = clientsock.recv(1024).decode('utf-8')
        print('client -> {}'.format(rcvmsg))
        if rcvmsg == '': break
        # クライアントが'finish'を要求してきたら'accept'を送ってループを抜ける
        if rcvmsg == 'finish\n':
            lastmsg = 'accept finish\n'
            clientsock.sendall(lastmsg.encode('utf-8'))
            print('server -> {}'.format(lastmsg))
            break
        sendmsg = '{}\n'.format(count)
        print('server -> {}'.format(sendmsg))
        clientsock.sendall(sendmsg.encode('utf-8'))
