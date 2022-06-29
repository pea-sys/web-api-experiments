//https://docs.microsoft.com/ja-jp/aspnet/core/fundamentals/websockets?view=aspnetcore-6.0
//https://github.com/dotnet/AspNetCore.Docs/tree/main/aspnetcore/fundamentals/websockets/samples/6.x/WebSocketsSample
var builder = WebApplication.CreateBuilder(args);

builder.Services.AddControllers();

var app = builder.Build();

var webSocketOptions = new WebSocketOptions
{
    KeepAliveInterval = TimeSpan.FromMinutes(2) //プロキシの接続の維持を保証する、クライアントに "ping" フレームを送信する頻度。 既定値は 2 分です。
};
app.UseWebSockets(webSocketOptions); //WebSocket ミドルウェア

app.UseDefaultFiles();
app.UseStaticFiles();

app.MapControllers();

app.Run();
