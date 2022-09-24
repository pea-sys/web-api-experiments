# ftp url Scheme

FTP 経由でのファイルの読み込みと新しいファイルの作成を許可します。  
サーバーがパッシブモードの FTP をサポートしていない場合、接続は失敗します。

ちなみに、ftp プロトコルは、Chronium ベースのブラウザでは受け付けないようになっているので、ftps を使用する必要があります。
Edge の IE モードであれば、ftp も使用可能になっています。
ftp サーバが匿名認証を許可していない場合は、URL に id とパスワードも埋め込む必要があります。  
ブラウザのアドレスバーに次の URL を入力した場合、

```url
ftp://localhost/file.txt
```

GET メソッドでファイルが取得できます。

![ss](https://user-images.githubusercontent.com/49807271/191910167-e8f0b066-ec0d-4e65-8b5f-6e619986a160.png)
