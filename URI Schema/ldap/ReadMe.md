# LDAP URI Scheme

Lightweight Directory Access Protocol(LDAP)とは、ネットワーク内のディレクトリサービスと通信できるように設計されたプロトコルです。  
通常 port は 389、TSL の場合は 636 を使用します。

LDAP を利用することで、認証の一元化やリソースのアクセス制御が出来ます。

LDAP サーバとは、LDAP を実装したデータベースです。

## [実験]

LDAP サーバーを立ち上げて、通信し検索成功までを試しました。  
職務上触る機会はなさそうなので途中で打ち切り。  
結果は LDAP サーバと通信したところまでで実験失敗となりました。  
※失敗することもある...

- 1.次の URL から OpenLDAP をダウンロードします
  https://www.maxcrc.de/en/download-en/

- 2.ダウンロードした圧縮ファイルを解凍してインストールします。

![1](https://user-images.githubusercontent.com/49807271/192074848-5a5de12b-4a84-4c7f-99d2-44eb09864ed6.png)

DB の選択以外は特に迷う所はないと思います。  
 実験後にすぐ使わなくなるので後片付けが面倒かなと思い、私はオンメモリの BDB にしました。
![2](https://user-images.githubusercontent.com/49807271/192074855-8dfb3d94-c1ec-451c-b05a-7b509864518c.png)

- 3. インストール完了後、インストール先にある slapd.conf を編集する。

```
suffix		"dc=自分のコンピュータ名,dc=com"
rootdn		"cn=Manager,dc=自分のコンピュータ名,dc=com"
```

- 4. LDAP サービスを停止します。

```
net stop OpenLDAP-slapd
```

- 5. LDAP サーバーを起動します。

```
cd C:\OpenLDAP
slapd -d 1 -f C:\OpenLDAP\slapd.conf
```

出力の最後が slapd starting になっていれば、起動成功

```
Expect poor performance for suffix "dc=127.0.0.1,dc=com".
632e71dd bdb_db_open: database "dc=127.0.0.1,dc=com": dbenv_open(./data).
632e71dd bdb_monitor_db_open: monitoring disabled; configure monitor database to enable
632e71dd slapd starting
```

※ちなみにカレントディレクトリをインストール先フォルダにせずに  
コマンド実行すると schema パスが通らなくて失敗しました。  
設定ファイル内から参照しているファイルパスが相対パスのためだと考えられます。

```
C:\OpenLDAP\slapd -d 1 -f C:\OpenLDAP\slapd.conf
```

- 6.LDAP サービスを停止します。

- 7.インストールディレクトリ先に次のファイル「add_users.ldif」を格納します。

```
dn: cn=Suzie Smith,ou=Server Technology,o=Acme, c=US
changetype: add
cn: Suzie Smith
cn: Suzie
sn: Smith
mail: ssmith@us.Acme.com
telephoneNumber: 69332
photo: \$ORACLE_INSTANCE/empdir/photog/ssmith.jpg
objectClass: organizationalPerson
objectClass: person
objectClass: top
```

- 8. LDAP サーバーを起動します。

```
cd C:\OpenLDAP
slapd -d 1 -f C:\OpenLDAP\slapd.conf
```

- 9.ブラウザの URL に入力
  IE 以外は Sldap じゃないとダメかもしれません。

```
ldap://自分のコンピュータ名/
```

- 10.アクセス許可の確認画面が表示されます

![3](https://user-images.githubusercontent.com/49807271/192083772-165f7389-e331-4ee4-b610-4a5059ff6c01.png)

- 11.検索条件を入力します。

![4](https://user-images.githubusercontent.com/49807271/192083771-024a6436-182b-4d1a-8e8c-b605b0f96ab5.png)

- 12.検索開始でサーバと通信を開始します。
  かなり片手落ちなのですが、サーバと通信した後にサーバが落ちてしまいました。多分、ドメインの設定がきちんと出来ていないのが原因な気はします。

```
632ea577 slap_listener_activate(2):
632ea577 >>> slap_listener(ldap:///)
632ea577 connection_get(4): got connid=1000
632ea577 connection_read(4): checking for input on id=1000
ber_get_next
ber_get_next: tag 0x30 len 16 contents:
632ea577 op tag 0x60, time 1664001399
ber_get_next
632ea577 conn=1000 op=0 do_bind
ber_scanf fmt ({imt) ber:
ber_scanf fmt (m}) ber:
632ea577 >>> dnPrettyNormal: <>
632ea577 <<< dnPrettyNormal: <>, <>
632ea577 do_bind: version=3 dn="" method=128
```

本気で解決する場合は、ログ調査等必要になりそうです。
今回はサーバーと通信できたので、これで終わりにしちゃいます。
関係ないですが、OpenLDAP のマスコットキャラクターのような幼虫がリアリティあって気持ち悪いなと思いました。

[参考]  
[オープン LDAP トラブル対処の第一歩](http://www.ldap.jp/_media/doc/osc2009_tokyo_spring.pdf)
