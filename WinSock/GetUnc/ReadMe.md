# ネットワーク共有フォルダを作成する

- 1.ユーザー追加

```
net user share-id share-password /add
```

- 2.フォルダ作成

```
md E:\share
```

- 3.フォルダを共有化する

```
net share share=E:\share
```

- 4.ドライブへのマッピングを作成する

```
net use Z: \\127.0.0.1\share share-password /user:share-id
```

- 5.ドライブのマッピング一覧を確認する

```
net use
新しい接続は記憶されます。


ステータス  ローカル名 リモート名                ネットワーク名

-------------------------------------------------------------------------------
OK           Z:        \\127.0.0.1\share         Microsoft Windows Network
コマンドは正常に終了しました。
```

- 6.確認が終わったらマッピングを削除する

```
net use Z: /delete
net share E:\share /delete
```
