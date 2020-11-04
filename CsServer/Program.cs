using System;
using System.Text;
using System.IO;
using System.Net;
using System.Net.Sockets;

namespace CsServer
{
    /// <summary>
    /// C#のサーバーアプリ
    /// サーバークライアント間の約束事はIPおよびポート番号を一致させること、閉じるときの合図を揃えておくことです
    /// </summary>
    class Program
    {
        static void Main(string[] args)
        {

            //listenerを立ち上げてクライアントからの接続を待つ。接続したらストリームを流し始める
            var listener = new TcpListener(IPAddress.Parse("127.0.0.1"), 3000);
            listener.Start();
            Console.WriteLine($"start listen ({((IPEndPoint)listener.LocalEndpoint).Address}:{(((IPEndPoint)listener.LocalEndpoint).Port)})。");
            var client = listener.AcceptTcpClient();
            Console.WriteLine($"connect ({((IPEndPoint)client.Client.RemoteEndPoint).Address}:{(((IPEndPoint)client.Client.RemoteEndPoint).Port)})");
            var ns = client.GetStream();

            //ループ内の処理1回でメッセージが1往復する
            int count = 0;
            var loop = true;
            while (loop)
            {
                //以下、受信部分
                var ms = new MemoryStream();
                byte[] resBytes = new byte[256];
                int resSize = 0;
                do
                {
                    resSize = ns.Read(resBytes, 0, resBytes.Length);
                    if (resSize == 0)
                    {
                        loop = false;
                        Console.WriteLine("disconnect");
                        break;
                    }
                    ms.Write(resBytes, 0, resSize);
                } while (ns.DataAvailable || resBytes[resSize - 1] != '\n');
                var resMsg = Encoding.UTF8.GetString(ms.ToArray());
                ms.Close();
                resMsg = resMsg.TrimEnd('\n');
                Console.WriteLine($"client -> {resMsg}");

                //以下、送信部分
                //もし'finish'を要求されたら、受諾メッセージを返信してループ脱出
                if (resMsg == "finish")
                {
                    var sendMsg = $"accept finish\n";
                    var sendBytes = Encoding.UTF8.GetBytes(sendMsg);
                    ns.Write(sendBytes, 0, sendBytes.Length);
                    Console.WriteLine($"server -> {sendMsg}");
                    loop = false;
                }
                //ここが普通のループ処理
                else
                {
                    var sendMsg = $"{++count}\n";
                    var sendBytes = Encoding.UTF8.GetBytes(sendMsg);
                    ns.Write(sendBytes, 0, sendBytes.Length);
                    Console.WriteLine($"server -> {sendMsg}");
                }
            }

            //ループを抜けたらいろいろお片付け
            ns.Close();
            client.Close();
            listener.Stop();
            Console.WriteLine("Enterを押すと終了します");
            while (true)
            {
                if (Console.ReadKey().Key == ConsoleKey.Enter) break;
            }

        }
    }
}
