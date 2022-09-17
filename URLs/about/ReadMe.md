# About スキーム

内部状態と組み込み関数を明らかにするためにさまざまな Web ブラウザーに実装されている内部 URI スキーム。  
様々な URL が提供されています。

### ■about:about

URL に about:about を入力。
about ページの一覧が表示されます。
ブラウザによって提供されるページは異なりますが、同じ役割のモノは URL も同じことが多いです。

- Chrome と Edge の場合
  ![1](https://user-images.githubusercontent.com/49807271/190853744-030685c8-6664-4b0e-b710-580d62af535a.png)

※「chrome://」は「about」に置き換えられます。  
　つまり、「chrome://about」は「about:about」と同じ。  
 加えて「chrome://chrome-urls」と「about:about」も同じ。

- FireFox の場合
  ![2](https://user-images.githubusercontent.com/49807271/190853804-15405cc2-27ce-450c-9815-d3d87f6e197d.png)

### ■ その他、使用する機会がありそうな About URL メモ

- about:crashes ・・・ブラウザのクラッシュレポートの確認ページ
  ![3](https://user-images.githubusercontent.com/49807271/190856879-f72bda24-4234-41c2-80f9-9e2efbe30a8b.png)

* about://device-log・・・デバイスログの確認ページ
  ※デバイスの着脱ログ等見れます。クエリパラメータにより自動更新可能。
  ![4](https://user-images.githubusercontent.com/49807271/190857095-4d4d2cca-b9f8-40e0-9ba2-0c7358d1cc07.png)

* chrome://flags/・・・各種実験機能  
  ※Chromium 専用。技術検証とかしたい場合に利用。

* chrome://gpu/・・・GPU 関連設定  
  ※Chromium 専用。GPU 搭載機で処理を高速化したい場合に利用。

* chrome://indexeddb-internals/・・・IndexedDB のログ  
  ※Chromium 専用。

* chrome://inspect・・・開発者向けツールを起動。Android の Chrome をデバッグする時に利用。

* chrome://interstitials・・・ページアクセス時に表示される通知や警告、エラー画面の一覧を表示します

* chrome://net-export・・・ネットワークログのキャプチャ機能。  
  ※[参考](https://www.chromium.org/for-testers/providing-network-details/)

* chrome://net-internals・・・Proxy や DNS のキャッシュクリア。ドメインに HSTS を適用したり、ソケット切断が可能。
