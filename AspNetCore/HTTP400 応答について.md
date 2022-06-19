## ■ 自動的な HTTP 400 応答

[ApiController] 属性により、モデル検証エラーが発生すると  
HTTP 400 応答が自動的にトリガーされます。

例えば、次のような API を備えているコントローラーに対し、

```
        // GET: api/TodoItems/5
        [HttpGet("{id}")]
        public async Task<ActionResult<TodoItemDTO>> GetTodoItem(long id)
        {
            var todoItem = await _context.TodoItems.FindAsync(id);

            if (todoItem == null)
            {
                return NotFound();
            }

            return ItemToDTO(todoItem);
        }
```

エラーが出るように HTTP リクエストを送信します。

```
リクエスト URL: https://localhost:7221/api/todoitems/a
リクエスト メソッド: GET
ステータス コード: 400
リモート アドレス: [::1]:7221
参照ポリシー: strict-origin-when-cross-origin
content-type: application/problem+json; charset=utf-8
date: Sat, 18 Jun 2022 23:41:45 GMT
server: Kestrel
:authority: localhost:7221
:method: GET
:path: /api/todoitems/a
:scheme: https
accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
accept-encoding: gzip, deflate, br
accept-language: en-US,en;q=0.9,ja;q=0.8
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

## ■ モデル検証エラーによるレスポンス作成をフックする

[ApiController] 属性により、モデル検証エラーが発生すると  
HTTP 400 応答が自動的にトリガーされますが、
次のコードを Progarm.cs に追加することで
ロギングを追加したり、レスポンスを書き換えることが可能です。

```
builder.Services.AddControllers()
    .ConfigureApiBehaviorOptions(options =>
    {
        // To preserve the default behavior, capture the original delegate to call later.
        var builtInFactory = options.InvalidModelStateResponseFactory;

        options.InvalidModelStateResponseFactory = context =>
        {
            var logger = context.HttpContext.RequestServices
                                .GetRequiredService<ILogger<Program>>();

            // Perform logging here.
            // ...
            logger.LogWarning("InvalidModelState", EventArgs.Empty);
            // Invoke the default behavior, which produces a ValidationProblemDetails
            // response.

            // To produce a custom response, return a different implementation of
            // IActionResult instead.

            var problemDetails = new ValidationProblemDetails(context.ModelState)
            {
                Title = "InvalidModelState",
                Status = StatusCodes.Status422UnprocessableEntity,
                Instance = context.HttpContext.Request.Path
            };

            problemDetails.Extensions.Add("traceId", context.HttpContext.TraceIdentifier);

            return new UnprocessableEntityObjectResult(problemDetails)
            {
                ContentTypes = { "application/problem+json" }
            };
            //return builtInFactory(context);
        };
    });
```

```

リクエスト URL: https://localhost:7221/api/todoitems/a
リクエスト メソッド: GET
ステータス コード: 422
リモート アドレス: [::1]:7221
参照ポリシー: strict-origin-when-cross-origin
content-type: application/problem+json; charset=utf-8
date: Sun, 19 Jun 2022 00:30:27 GMT
server: Kestrel
:authority: localhost:7221
:method: GET
:path: /api/todoitems/a
:scheme: https
accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
accept-encoding: gzip, deflate, br
accept-language: en-US,en;q=0.9,ja;q=0.8
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

## ■ 自動的な 400 応答を無効にする

ApiController による自動的な 400 応答を無効にするには、
Program.cs に以下のコードを追加します。

```
builder.Services.AddControllers()
    .ConfigureApiBehaviorOptions(options =>
    {
        options.SuppressModelStateInvalidFilter = true; //自動的な400応答を廃止する
    });
```

これにより、Controller に自分で実装した結果コード(NotFound)を応答するようになります。

```
リクエスト URL: https://localhost:7221/api/todoitems/a
リクエスト メソッド: GET
ステータス コード: 404
リモート アドレス: [::1]:7221
参照ポリシー: strict-origin-when-cross-origin
content-type: application/problem+json; charset=utf-8
date: Sat, 18 Jun 2022 23:45:21 GMT
server: Kestrel
:authority: localhost:7221
:method: GET
:path: /api/todoitems/a
:scheme: https
accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
accept-encoding: gzip, deflate, br
accept-language: en-US,en;q=0.9,ja;q=0.8
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
