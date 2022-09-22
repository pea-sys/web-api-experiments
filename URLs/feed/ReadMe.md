# Feed URL Scheme

フィード文書であることを表します(RFC 4287、RFC 5005)。  
フィード文書は RSS,Atom などがあります。  
Atom は RSS の仕様に不満を持った方々は作成したらしいです。  
現状、フィード文書を作る場合 Atom で問題ないかと思います。  
Atom 仕様は RSS と異なり標準化団体が策定しています。

## 実験

試しにフィード文書を作って、フィードリーダーで取得できるか試します

- 1.フィード文書を作ります

```xml
<?xml version="1.0" encoding="UTF-8"?>
<feed xmlns="http://www.w3.org/2005/Atom" xml:lang="ja">
  <title>PageTitle</title>
  <id>FeedId</id>
  <author>
    <name>Author's Name</name>
  </author>
  <link rel="alternate" type="text/html" href="index.html"/>
  <link rel="self" type="application/atom+xml" href="feeds.xml"/>
  <updated>2021-11-02T12:13:14Z</updated>

  <entry>
    <title>TopicTitle</title>
    <id>FeedId-3</id>
    <link rel="alternate" type="text/html" href="2.html"/>
    <updated>2020-10-02T12:13:14Z</updated>
    <summary>Summary-1</summary>
  </entry>

  <entry>
    <title>TopicTitle</title>
    <id>FeedId-1</id>
    <link rel="alternate" type="text/html" href="1.html"/>
    <updated>2020-11-02T12:13:14Z</updated>
    <summary>Summary-1</summary>
  </entry>
</feed>
```

- 2.フィードを埋め込みたいページのヘッダーにフィードリンクを埋め込みます。  
   type は埋め込むフィード文書のフォーマットを指定する必要があります。

```xml
<!DOCTYPE html>
<html lang="en-us">
  <head>
    <link rel="alternate" type="application/atom+xml" title="My Atom Feeds" href="feeds/atom.xml" />
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width" />
    <title>Feed in HTML</title>
  </head>
  <body>
    <h1>Feed URL in HTML</h1>
    <div></div>
  </body>
</html>
```

- 4.Chrome 拡張の Web Server For Chrome を起動します。

  ![3](https://user-images.githubusercontent.com/49807271/191444083-5362469e-447c-411e-9dba-3bd7d08ba0d9.png)

- 5.Web Server For Chrome の CHOOSE FOLDER で上記 HTML を格納しているフォルダを指定します。

- 6.フィードリーダーである Chrome 拡張の Feedbro をインストールし、OpenFeedReader を選択

  ![4](https://user-images.githubusercontent.com/49807271/191445398-3c17bcab-a3a9-4db1-8ac6-a8172d53558e.png)

* 7.次のページが開くので、Add a New Feed を選択。

  ![5](https://user-images.githubusercontent.com/49807271/191445679-7413ac13-7866-4647-9bdd-81ece6782445.png)

* 6.URL の項目に Web Server For Chrome で表示されている URL を貼り付け、
  Load ボタンを押した後、Preview ボタンを押します。

![5](https://user-images.githubusercontent.com/49807271/191449809-9a0fb89a-e3ea-4abd-b7d3-72efe2253e79.png)

- 7.Cancel ボタンを押し、ダイアログを閉じるとフィード文書の内容が表示されていることが確認できます。

![6](https://user-images.githubusercontent.com/49807271/191449818-01005692-d481-42ce-9bc8-9ee6283986c2.png)

[参考]  
 [フィードデータのバリデータ](https://validator.w3.org/feed/check.cgi)
