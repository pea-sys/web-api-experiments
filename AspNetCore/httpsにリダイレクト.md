# ASP.NET Core で HTTPS にリダイレクトする

[参考 URL](https://docs.microsoft.com/ja-jp/aspnet/core/security/enforcing-ssl?view=aspnetcore-6.0&tabs=visual-studio)

- Http アクセスをリダイレクトしなかった場合の動作確認  
  結果コード 200 OK のレスポンスと GET の結果が返ります。

```
リクエスト URL: http://localhost:5113/todoitems
リクエスト メソッド: GET
ステータス コード: 200 OK
リモート アドレス: [::1]:5113
参照ポリシー: strict-origin-when-cross-origin
HTTP/1.1 200 OK
Content-Type: application/json; charset=utf-8
Date: Thu, 16 Jun 2022 12:16:38 GMT
Server: Kestrel
Transfer-Encoding: chunked
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Accept-Encoding: gzip, deflate, br
Accept-Language: en-US,en;q=0.9,ja;q=0.8
Cache-Control: no-cache
Connection: keep-alive
Host: localhost:5113
Pragma: no-cache
sec-ch-ua: " Not A;Brand";v="99", "Chromium";v="102", "Google Chrome";v="102"
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: "Windows"
Sec-Fetch-Dest: document
Sec-Fetch-Mode: navigate
Sec-Fetch-Site: none
Sec-Fetch-User: ?1
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/102.0.0.0 Safari/537.36
```

- 次のコードを Program.cs に追加します。

```
//HTTP 要求を HTTPS にリダイレクトするためのミドルウェア
app.UseHttpsRedirection();
//HTTP Strict Transport Security プロトコル (HSTS) ヘッダーをクライアントに送信するための、HSTS ミドルウェア
app.UseHsts();
```

- Http アクセスをリダイレクトする場合の動作確認  
  結果コード 307 Temporary Redirect のレスポンスの結果が返ります。レスポンスヘッダーの Location にはリダイレクト先が格納されています。

```
リクエスト URL: http://localhost:5113/todoitems
リクエスト メソッド: GET
ステータス コード: 307 Temporary Redirect
リモート アドレス: [::1]:5113
参照ポリシー: strict-origin-when-cross-origin
Content-Length: 0
Date: Thu, 16 Jun 2022 12:22:07 GMT
Location: https://localhost:7113/todoitems
Server: Kestrel
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Accept-Encoding: gzip, deflate, br
Accept-Language: en-US,en;q=0.9,ja;q=0.8
Cache-Control: no-cache
Connection: keep-alive
Host: localhost:5113
Pragma: no-cache
sec-ch-ua: " Not A;Brand";v="99", "Chromium";v="102", "Google Chrome";v="102"
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: "Windows"
Sec-Fetch-Dest: document
Sec-Fetch-Mode: navigate
Sec-Fetch-Site: none
Sec-Fetch-User: ?1
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/102.0.0.0 Safari/537.36
```

- それを受けてクライアント(Chrome)がリダイレクトします。  
  　初回のリダイレクトは遅いです。

```
リクエスト URL: https://localhost:7113/todoitems
リクエスト メソッド: GET
ステータス コード: 200
リモート アドレス: [::1]:7113
参照ポリシー: strict-origin-when-cross-origin
content-type: application/json; charset=utf-8
date: Thu, 16 Jun 2022 12:22:08 GMT
server: Kestrel
:authority: localhost:7113
:method: GET
:path: /todoitems
:scheme: https
accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
accept-encoding: gzip, deflate, br
accept-language: en-US,en;q=0.9,ja;q=0.8
cache-control: no-cache
pragma: no-cache
sec-ch-ua: " Not A;Brand";v="99", "Chromium";v="102", "Google Chrome";v="102"
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: "Windows"
sec-fetch-dest: document
sec-fetch-mode: navigate
sec-fetch-site: none
sec-fetch-user: ?1
upgrade-insecure-requests: 1
user-agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/102.0.0.0 Safari/537.36
```

- ちなみに Chrome だと レスポンスコード 307 は主要なブラウザではキャッシュしないようです。

  ![2](https://user-images.githubusercontent.com/49807271/174084897-e982bc31-095e-464a-8631-01c01116f467.jpg)

# 永続的なリダイレクトを構成する

永続的なリダイレクトを構成します

launchSettings.json を次のように編集

```
"ASPNETCORE_ENVIRONMENT": "Production"
```

- 次のコードを Program.cs に追加します。

```
if (!builder.Environment.IsDevelopment())
{
    builder.Services.AddHttpsRedirection(options =>
    {
        //永続的なリダイレクトを構成する
        options.RedirectStatusCode = (int)HttpStatusCode.PermanentRedirect;
        //httpsサーバーのPort
        options.HttpsPort = 7113;
    });
}
```

- Http アクセスをすると、308 Permanent Redirect が返ります。

```
リクエスト URL: http://localhost:5113/todoitems
リクエスト メソッド: GET
ステータス コード: 308 Permanent Redirect
リモート アドレス: [::1]:5113
参照ポリシー: strict-origin-when-cross-origin
Content-Length: 0
Date: Fri, 17 Jun 2022 13:48:38 GMT
Location: https://localhost:7113/todoitems
Server: Kestrel
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Accept-Encoding: gzip, deflate, br
Accept-Language: en-US,en;q=0.9,ja;q=0.8
Cache-Control: no-cache
Connection: keep-alive
Host: localhost:5113
Pragma: no-cache
sec-ch-ua: " Not A;Brand";v="99", "Chromium";v="102", "Google Chrome";v="102"
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: "Windows"
Sec-Fetch-Dest: document
Sec-Fetch-Mode: navigate
Sec-Fetch-Site: none
Sec-Fetch-User: ?1
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/102.0.0.0 Safari/537.36
```

- それを受けてクライアント(Chrome)がリダイレクトします。  
  　初回のリダイレクトは遅いです。

```
リクエスト URL: https://localhost:7113/todoitems
リクエスト メソッド: GET
ステータス コード: 200
リモート アドレス: [::1]:7113
参照ポリシー: strict-origin-when-cross-origin
content-type: application/json; charset=utf-8
date: Fri, 17 Jun 2022 13:48:39 GMT
server: Kestrel
:authority: localhost:7113
:method: GET
:path: /todoitems
:scheme: https
accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
accept-encoding: gzip, deflate, br
accept-language: en-US,en;q=0.9,ja;q=0.8
cache-control: no-cache
pragma: no-cache
sec-ch-ua: " Not A;Brand";v="99", "Chromium";v="102", "Google Chrome";v="102"
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: "Windows"
sec-fetch-dest: document
sec-fetch-mode: navigate
sec-fetch-site: none
sec-fetch-user: ?1
upgrade-insecure-requests: 1
user-agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/102.0.0.0 Safari/537.36
```

ちなみに、永続的なリダイレクトに対応しているブラウザかどうかは
次のサイトで確認できます（今は主要ブラウザ全てで対応されているようです)  
http://webdbg.com/test/308/
