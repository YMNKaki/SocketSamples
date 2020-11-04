//node.js初めてさわってみた
//ソケット通信でクライアントとして振る舞います

let count = 0;
const client = new require('net').Socket();
client.setEncoding('utf8');

//終わるためのキー入力待ち
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

//接続を試みる部分。接続したら1秒ごとにメッセージを送る
client.connect('3000', '127.0.0.1', function () {
    console.log('connected to server');
    setInterval(() => {
        const sendmsg = `${++count}\n`;
        client.write(sendmsg);
        console.log(`client -> ${sendmsg}`);
    }, 500);
});

//サーバーから返答が来た時の処理
client.on('data', data => {
    console.log(`server -> ${data}\n`);
    if (data == 'accept finish\n') {
        client.close();
    }
});

client.on('close', () => {
    console.log('client -> connection is closed');
});
