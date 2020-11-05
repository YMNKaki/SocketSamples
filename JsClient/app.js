//node.js���߂Ă�����Ă݂�
//�\�P�b�g�ʐM�ŃN���C�A���g�Ƃ��ĐU�镑���܂�

const { clearInterval } = require('timers');

let connection = true;
let count = 0;
const client = new require('net').Socket();
client.setEncoding('utf8');

//�I��邽�߂̃L�[���͑҂�
const readline = require('readline').createInterface({
    input: process.stdin,
    output: process.stdout
});

readline.question('Press Enter when you finish ', (answer) => {
    const sendmsg = `finish\n`;
    client.write(sendmsg);
    console.log(`client -> ${sendmsg}`);
    readline.close();
});

//�ڑ������݂镔���B�ڑ�������1�b���ƂɃ��b�Z�[�W�𑗂�
client.connect('3000', '127.0.0.1', function () {
    console.log('connected to server');
    setInterval(() => {
        if (connection == true) {
            const sendmsg = `${++count}\n`;
            client.write(sendmsg);
            console.log(`client -> ${sendmsg}`);
        }
        else {
            clearInterval();
        }
    }, 500);
});

//�T�[�o�[����ԓ����������̏���
client.on('data', data => {
    console.log(`server -> ${data}\n`);
    if (data == 'accept finish\n') {
        connection = false;
    }
});

client.on('close', () => {
    console.log('client -> connection is closed');
});
