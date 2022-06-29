//https://docs.microsoft.com/ja-jp/aspnet/core/fundamentals/websockets?view=aspnetcore-6.0
//https://github.com/dotnet/AspNetCore.Docs/tree/main/aspnetcore/fundamentals/websockets/samples/6.x/WebSocketsSample
var builder = WebApplication.CreateBuilder(args);

builder.Services.AddControllers();

var app = builder.Build();

var webSocketOptions = new WebSocketOptions
{
    KeepAliveInterval = TimeSpan.FromMinutes(2) //�v���L�V�̐ڑ��̈ێ���ۏ؂���A�N���C�A���g�� "ping" �t���[���𑗐M����p�x�B ����l�� 2 ���ł��B
};
app.UseWebSockets(webSocketOptions); //WebSocket �~�h���E�F�A

app.UseDefaultFiles();
app.UseStaticFiles();

app.MapControllers();

app.Run();
