# Windows10 で FTP サーバー構築

今回は簡単な FTP クライアントプログラムの動作確認を行うための,  
前準備として FTP サーバーの構築を行った。  
実験用なのでセキュリティはガバガバです。

- 1.コントロールパネルを開く

- 2.プログラムをクリック

![1](https://user-images.githubusercontent.com/49807271/184576295-c8324862-dbc4-4181-80e8-0d19a12e3098.jpg)

- 3.Windows の機能の有効化または無効化をクリック

![2](https://user-images.githubusercontent.com/49807271/184576507-57827ba1-5c17-49c3-803e-c8f58e8c57de.jpg)

- 4.FTP Service と ISS コンソールサービス をクリックして OK 押下

![3](https://user-images.githubusercontent.com/49807271/184579208-5dec7a51-911b-4c1b-b3c0-efcf1b37db9f.jpg)

- 5.インストール後、ウィンドウを閉じます

![4](https://user-images.githubusercontent.com/49807271/184576601-453a8f3c-473c-4f3c-8160-3312059eb6c6.jpg)

- 6.コマンドプロンプトにて ftp サーバー接続用のアカウントを作成します

```
net user ftp-user ftp-password /add
```

- 7.IIS マネージャーを起動します

![5](https://user-images.githubusercontent.com/49807271/184580424-79f2a604-da31-4923-98d3-5b1a81ae7c5e.jpg)

- 8.ftp サイトの追加 を選択します

![6](https://user-images.githubusercontent.com/49807271/184582068-83bd3db9-0115-43b9-ac62-34079ff88a38.jpg)

- 7.  FTP サイト名とパスを設定します

![7](https://user-images.githubusercontent.com/49807271/184582479-e2cd2343-f445-4c2f-9a4f-80e5054c6e32.jpg)

- 8. 今回は SSL は使用しないので SSL 無しを選択

![8](https://user-images.githubusercontent.com/49807271/184591931-5aac99f0-59b5-4d6e-ba1f-428c1c15b6fb.jpg)

- 9.　全てのユーザーで読み書き可能な設定とします

![9](https://user-images.githubusercontent.com/49807271/184592910-24f33aa1-0887-4bd4-832c-45fe7af85690.jpg)

- 10. ftp サーバーを起動します

![12](https://user-images.githubusercontent.com/49807271/184597395-6052716f-ce63-4fc5-bff8-4fa6ac95eafd.jpg)

- 11. コマンドプロンプトから ftp サーバに接続します

```
C:\Users\user>ftp
ftp> open localhost
DESKTOP-L18OTEK に接続しました。
220 Microsoft FTP Service
200 OPTS UTF8 command successful - UTF8 encoding now ON.
ユーザー (DESKTOP-L18OTEK:(none)): ftp-user
331 Password required
パスワード:
230 User logged in.
ftp> quit
221 Goodbye.
```
