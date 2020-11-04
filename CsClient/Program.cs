using System;
using System.Text;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;

namespace SocketSamples
{
    /// <summary>
    /// C#のクライアントアプリ
    /// サーバークライアント間の約束事はIPおよびポート番号を一致させること、閉じるときの合図を揃えておくことです
    /// </summary>
    class Program
    {
        static void Main(string[] args)
        {

            Console.WriteLine("Enterを押すとサーバーに接続します");
            while(true)
            {
                if (Console.ReadKey().Key == ConsoleKey.Enter) break;
            }

            //サーバーを探して接続
            //当たり前ですが、先にサーバーを立ち上げてないとここでエラー吐きます
            var client = new TcpClient("127.0.0.1", 3000);
            Console.WriteLine($"start ({((IPEndPoint)client.Client.LocalEndPoint).Address}, {((IPEndPoint)client.Client.LocalEndPoint).Port})");
            Console.WriteLine($"connect ({((IPEndPoint)client.Client.RemoteEndPoint).Address}:{(((IPEndPoint)client.Client.RemoteEndPoint).Port)})");

            Console.WriteLine("Enterを押すとストリームを開始します");
            while (true)
            {
                var key = Console.ReadKey().Key;
                if (key == ConsoleKey.Enter) break;
            }

            //ストリームが流れ始める
            var ns = client.GetStream();

            //処理そのものは別スレッドを立てて行う
            //メインスレッドは、下のキー入力待ちの方へ進みます
            Task.Run(() =>
            {
                int count = 0;
                var loop = true;
                while (loop)
                {
                    //送信部分
                    var sendMsg = $"{++count}\n";
                    var sendBytes = Encoding.UTF8.GetBytes(sendMsg);
                    ns.Write(sendBytes, 0, sendBytes.Length);
                    Console.WriteLine($"client -> {sendMsg}");

                    //受信部分
                    var ms = new MemoryStream();
                    var resBytes = new byte[256];
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
                    var res = Encoding.UTF8.GetString(ms.ToArray());
                    Console.WriteLine($"server -> {res}");
                    Thread.Sleep(500);
                }
            });

            //終了のためのキー入力待ち
            while (true)
            {
                if (Console.ReadKey().Key == ConsoleKey.Enter)
                {
                    var sendMsg = $"finish\n";
                    var sendBytes = Encoding.UTF8.GetBytes(sendMsg);
                    ns.Write(sendBytes, 0, sendBytes.Length);
                    Console.WriteLine($"client -> {sendMsg}");
                    var ms = new MemoryStream();
                    var resBytes = new byte[256];
                    int resSize = 0;
                    do
                    {
                        resSize = ns.Read(resBytes, 0, resBytes.Length);
                        if (resSize == 0)
                        {
                            Console.WriteLine("disconnect");
                            break;
                        }
                        ms.Write(resBytes, 0, resSize);
                    } while (ns.DataAvailable || resBytes[resSize - 1] != '\n');
                    var res = Encoding.UTF8.GetString(ms.ToArray());
                    Console.WriteLine($"server -> {res}");
                    ns.Close();
                    client.Close();
                    break;
                }
            }
        }
    }
}
