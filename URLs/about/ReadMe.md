# About スキーム

内部状態と組み込み関数を明らかにするためにさまざまな Web ブラウザーに実装されている内部 URI スキーム。  
様々な URL が提供されています。
同じブラウザでも、PC とモバイルや OS によって機能差はあります。

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
  技術検証とかしたい場合に利用。

* chrome://gpu/・・・GPU 関連設定  
  GPU 搭載機で処理を高速化したい場合に利用。

* chrome://indexeddb-internals/・・・IndexedDB のログ

* chrome://inspect・・・開発者向けツールを起動。Android の Chrome をデバッグする時に利用。

* chrome://interstitials・・・ページアクセス時に表示される通知や警告、エラー画面の一覧を表示します

* chrome://net-export・・・ネットワークログのキャプチャ機能。  
  ※[参考](https://www.chromium.org/for-testers/providing-network-details/)

* chrome://net-internals・・・Proxy や DNS のキャッシュクリア。ドメインに HSTS を適用したり、ソケット切断が可能。

* chrome://network-errors/・・・様々なネットワークエラーの画面を表示できます

* chrome://optimization-guide-internals・・・ optimization guide debug flags を Enable に変更すると、最適化に役立つログを出力します。
  ![4](https://user-images.githubusercontent.com/49807271/190878368-d0f82394-8f66-427f-966e-0a92793f0ae9.png)

* chrome://safe-browsing・・・safe browsing 機能の設定確認やテストが出来ます。

* chrome://serviceworker-internals・・・ServiceWorker の情報を表示します。デバッグ用。

* chrome://system・・・システム診断データが参照できます。各種使用メモリ等確認可能。

* chrome://tracing・・・パフォーマンスの問題を診断する場合、Chrome が「内部」で何を行っているかを確認することが重要な場合があります。何が起こっているのかをより詳細に把握する 1 つの方法は、about:tracing ツールを使用することです。
  [参考](https://www.chromium.org/developers/how-tos/trace-event-profiling-tool/)

* chrome://usb-internals/・・・USB デバイスとデバイスリストをテストするオプションを提供します

* chrome://user-actions/・・・ユーザーアクションを表示。

* chrome://webrtc-internals・・・WebRTC ダンプの作成。

* chrome://webrtc-logs・・・WebRTC ログの表示。

* chrome://internals/session-service・・・chrome のセッションサービスのログを確認します。

* chrome://hang・・・ページをハングさせる
* chrome://kill・・・Chrome を強制終了
* chrome://quit/・・・Chrome を修了
* chrome://restart/・・・Chrome を再起動します。
