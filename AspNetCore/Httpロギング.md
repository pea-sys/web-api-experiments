## HTTP ロギング

[参考](https://docs.microsoft.com/ja-jp/aspnet/core/fundamentals/http-logging/?view=aspnetcore-6.0)

■ 環境

- ASP.NET Core 6.0

■ 手順
Program.cs で HTTP ロギングミドルウェアの使用を宣言する。

```
app.UseHttpLogging(); // HTTPログ用の組み込みミドルウェア
```

launchSettings.json に設定追加。  
公式 Github に上がっているサンプルは古かったので
ドキュメントとやや記述が異なる。

```
"Logging": {
    "LogLevel": {
      "Default": "Information",
      "Microsoft.AspNetCore": "Warning",
      "Microsoft.AspNetCore.HttpLogging.HttpLoggingMiddleware": "Information" ←追加
    }
  }
```

プログラムを起動すると、サーバーを立ち上げているコンソールにログ出力される。

```
info: Microsoft.AspNetCore.HttpLogging.HttpLoggingMiddleware[1]
      Request:
      Protocol: HTTP/1.1
      Method: GET
      Scheme: http
      PathBase:
      Path: /weatherforecast
      Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
      Connection: keep-alive
      Host: localhost:5092
      User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/102.0.0.0 Safari/537.36
      Accept-Encoding: gzip, deflate, br
      Accept-Language: en-US,en;q=0.9,ja;q=0.8
      Upgrade-Insecure-Requests: [Redacted]
      sec-ch-ua: [Redacted]
      sec-ch-ua-mobile: [Redacted]
      sec-ch-ua-platform: [Redacted]
      Sec-Fetch-Site: [Redacted]
      Sec-Fetch-Mode: [Redacted]
      Sec-Fetch-User: [Redacted]
      Sec-Fetch-Dest: [Redacted]
info: Microsoft.AspNetCore.HttpLogging.HttpLoggingMiddleware[2]
      Response:
      StatusCode: 200
      Content-Type: application/json; charset=utf-8
      Date: Tue, 21 Jun 2022 13:50:16 GMT
      Server: Kestrel
      Transfer-Encoding: chunked
```

ログの構成はカスタムできます。  
Program.cs で構成します。
詳しくは公式ドキュメントを見てください。

```
builder.Services.AddHttpLogging(logging =>
{
    logging.LoggingFields = HttpLoggingFields.RequestHeaders | HttpLoggingFields.RequestBody;
    logging.MediaTypeOptions.AddText("application/json", System.Text.Encoding.UTF8);
});
```

次のログが出力されます。

```
info: Microsoft.AspNetCore.HttpLogging.HttpLoggingMiddleware[1]
      Request:
      Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
      Connection: keep-alive
      Host: localhost:5092
      User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/102.0.0.0 Safari/537.36
      Accept-Encoding: gzip, deflate, br
      Accept-Language: en-US,en;q=0.9,ja;q=0.8
      Upgrade-Insecure-Requests: [Redacted]
      sec-ch-ua: [Redacted]
      sec-ch-ua-mobile: [Redacted]
      sec-ch-ua-platform: [Redacted]
      Sec-Fetch-Site: [Redacted]
      Sec-Fetch-Mode: [Redacted]
      Sec-Fetch-User: [Redacted]
      Sec-Fetch-Dest: [Redacted]
info: Microsoft.AspNetCore.HttpLogging.HttpLoggingMiddleware[2]
      Response:
```

ログの量が増えると、当然パフォーマンスも落ちるので注意が必要です。
