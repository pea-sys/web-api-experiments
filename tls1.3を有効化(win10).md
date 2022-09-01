## Windows10 で TLS1.3 を有効化

- 1.コマンドプロンプトで次のコマンドを実行

```
inetcpl.cpl
```

- 2.「詳細設定」タブをクリック

![1](https://user-images.githubusercontent.com/49807271/187685804-f9e3400d-b5b7-4d4e-8078-2404584d6c7c.png)

- 3.「TLS1.3 を使用する」のチェックボックスをチェックして適用ボタンクリック

![2](https://user-images.githubusercontent.com/49807271/187685893-befaeb77-d9b3-4fb8-b34a-3e4fb09bd2d5.png)

## 各種ブラウザの TLS1.3 対応状況

- GoogleChrome 104.0.5112.102 は　デフォルトで TLS1.3 有効
- Edge は TLS1.2 まで有効
- FireFox は未確認

## おまけ

WireShark で TLS1.3 が使われていることを確認
![3](https://user-images.githubusercontent.com/49807271/187694090-e0881b51-a6bb-4ccb-97a7-9d734157f02c.png)
