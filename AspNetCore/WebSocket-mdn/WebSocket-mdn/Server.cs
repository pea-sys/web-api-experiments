using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace WebSocket_mdn
{
    internal class Server
    {
        public static void Main()
        {
            string ip = "127.0.0.1";
            int port = 80;
            var server = new TcpListener(IPAddress.Parse(ip), port);

            server.Start();
            Console.WriteLine("Server has started on {0}:{1}, Waiting for a connection...", ip, port);

            TcpClient client = server.AcceptTcpClient();
            Console.WriteLine("A client connected.");

            NetworkStream stream = client.GetStream();

            // enter to an infinite cycle to be able to handle every change in stream
            while (true)
            {
                while (!stream.DataAvailable) ;
                while (client.Available < 3) ; // match against "get"

                byte[] bytes = new byte[client.Available];
                stream.Read(bytes, 0, client.Available);
                string s = Encoding.UTF8.GetString(bytes);
                // ハンドシェイク
                /*
                 * GET / HTTP/1.1
                   Host: 127.0.0.1
                   Connection: Upgrade
                   Pragma: no-cache
                   Cache-Control: no-cache
                   User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.0.0 Safari/537.36
                   Upgrade: websocket
                   Origin: null
                   Sec-WebSocket-Version: 13
                   Accept-Encoding: gzip, deflate, br
                   Accept-Language: en-US,en;q=0.9,ja;q=0.8
                   Sec-WebSocket-Key: U+urlh98pFs15eWeWNQf6A==
                   Sec-WebSocket-Extensions: permessage-deflate; client_max_window_bits
                 */
                if (Regex.IsMatch(s, "^GET", RegexOptions.IgnoreCase))
                {
                    Console.WriteLine("=====Handshaking from client=====\n{0}", s);
                    // ハンドシェイク手順
                    // 1. Obtain the value of the "Sec-WebSocket-Key" request header without any leading or trailing whitespace
                    //    Sec-WebSocket-Keyリクエストヘッダーの値を取得します
                    // 2. Concatenate it with "258EAFA5-E914-47DA-95CA-C5AB0DC85B11" (a special GUID specified by RFC 6455)
                    //    取得したヘッダー値にGUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11" を付加
                    // 3. Compute SHA-1 and Base64 hash of the new value
                    //    手順2の値をSHA-1ハッシュ関数を通してBase64変換します
                    // 4. Write the hash back as the value of "Sec-WebSocket-Accept" response header in an HTTP response
                    //    HTTP レスポンスの "Sec-WebSocket-Accept" レスポンスヘッダーの値としてハッシュを書き戻します
                    string swk = Regex.Match(s, "Sec-WebSocket-Key: (.*)").Groups[1].Value.Trim();
                    string swka = swk + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
                    byte[] swkaSha1 = System.Security.Cryptography.SHA1.Create().ComputeHash(Encoding.UTF8.GetBytes(swka));
                    string swkaSha1Base64 = Convert.ToBase64String(swkaSha1);

                    // HTTP/1.1 defines the sequence CR LF as the end-of-line marker
                    byte[] response = Encoding.UTF8.GetBytes(
                        "HTTP/1.1 101 Switching Protocols\r\n" +
                        "Connection: Upgrade\r\n" +
                        "Upgrade: websocket\r\n" +
                        "Sec-WebSocket-Accept: " + swkaSha1Base64 + "\r\n\r\n");

                    stream.Write(response, 0, response.Length);
                }
                else
                {
                    //finビット：完全なメッセージがクライアントから送信されたかどうかを示します
                    bool fin = (bytes[0] & 0b10000000) != 0,
                        mask = (bytes[1] & 0b10000000) != 0; // must be true, "All messages from the client to the server have this bit set"

                    //ペイロードの長さ: この値が 0〜125 の場合、メッセージの長さになります。
                    //126 の場合、次の 2 バイト(16 ビットの符号なし整数) が長さになります。
                    //127 の場合、次の 8 バイト(64ビットの符号なし整数) が長さになります。
                    //126以上にする場合、RFC6455によると、ハンドシェイク時にSec-WebSocket-Extensionsを使用して、拡張方法を取り決めしておく必要がある
                    int opcode = bytes[0] & 0b00001111, // expecting 1 - text message
                        msglen = bytes[1] - 128, // & 0111 1111
                        offset = 2;

                    if (msglen == 126)
                    {
                        // was ToUInt16(bytes, offset) but the result is incorrect
                        msglen = BitConverter.ToUInt16(new byte[] { bytes[3], bytes[2] }, 0);
                        offset = 4;
                    }
                    else if (msglen == 127)
                    {
                        Console.WriteLine("TODO: msglen == 127, needs qword to store msglen");
                        // i don't really know the byte order, please edit this
                        // msglen = BitConverter.ToUInt64(new byte[] { bytes[5], bytes[4], bytes[3], bytes[2], bytes[9], bytes[8], bytes[7], bytes[6] }, 0);
                        // offset = 10;
                    }

                    if (msglen == 0)
                        Console.WriteLine("msglen == 0");
                    else if (mask)
                    {
                        byte[] decoded = new byte[msglen];
                        byte[] masks = new byte[4] { bytes[offset], bytes[offset + 1], bytes[offset + 2], bytes[offset + 3] };
                        offset += 4;

                        for (int i = 0; i < msglen; ++i)
                            decoded[i] = (byte)(bytes[offset + i] ^ masks[i % 4]);

                        string text = Encoding.UTF8.GetString(decoded);
                        Console.WriteLine("{0}", text);
                    }
                    else
                        Console.WriteLine("mask bit not set");

                    Console.WriteLine();
                }
            }
        }
    }
}
