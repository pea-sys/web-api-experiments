# FireFox で IndexedDB のディスクフラッシュを有効化する

IndexedDB は Commit 成功時のディスクフラッシュを保証していない。  
FireFox だけ例外的にディスクフラッシュを同期して行うための
実験的機能が提供されている。

この機能を使用するには、FireFox の設定を変える必要がある。

- 1.FireFox を起動する
- 2.URL に「about:config」と入力する
- 3.設定検索欄に「indexeddb」を入力する
- 4.次の検索候補が出るので「dom.indexdDB.experimental」の項目の
  赤枠部をクリックして、true に設定する

![1](https://user-images.githubusercontent.com/49807271/189515699-bdff1e99-f354-4c11-bbe9-8e18463448f4.png)

- 5.IndexedDB のトランザクション開始時に "readwriteflush" を指定すれば OK
