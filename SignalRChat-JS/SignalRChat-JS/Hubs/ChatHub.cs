using Microsoft.AspNetCore.SignalR;

namespace SignalRChat_JS.Hubs
{
    /// <summary>
    /// ハブはクライアント サーバー通信を処理するハイレベル パイプラインとして機能するクラス
    /// </summary>
    public class ChatHub : Hub
    {
        /// <summary>
        /// SendMessage メソッドは、メッセージをすべてのクライアントに送信する
        /// </summary>
        /// <param name="user"></param>
        /// <param name="message"></param>
        /// <returns></returns>
        public async Task SendMessage(string user, string message)
        {
            await Clients.All.SendAsync("ReceiveMessage", user, message);
        }
    }
}
