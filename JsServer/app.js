//node.js���߂Ă�����Ă݂�
//�\�P�b�g�ʐM�ŃT�[�o�[�Ƃ��ĐU�镑���܂�

let count = 0;
require('net').createServer(socket => {

    socket.on('data', data => {
        console.log(`client -> ${data}`);
        if (data == 'finish\n') {
            const sendmsg = `accept finish\n`;
            socket.write(sendmsg);
            console.log(`server -> ${sendmsg}`);
        }
        else {
            const sendmsg = `${++count}\n`;
            socket.write(sendmsg);
            console.log(`server -> ${sendmsg}`);
        }
    });

    socket.on('close', () => {
        console.log('client closed connection');
    });

}).listen(3000);