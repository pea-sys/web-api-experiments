## Windows10 で telnet を試す

今はセキュリティの問題で ssh が使われることが多いですが、  
telnet でリモート PC へのログインを試す。

## ■Server 編

Windows10 では telnet server は建てられないので、WSL2 から起動します。

- 1. コマンドプロンプトから wsl を起動します。

```
C:\Users\user>wsl
```

- 2. wsl に telnet をインストールします。

```
sudo apt update
sudo apt install telnetd
sudo apt install inetutils-inetd
```

- 3. telnet server 起動

```
sudo /etc/init.d/inetutils-inetd start
```

- 4. wsl へポートフォワーディングします

```
netsh interface portproxy add v4tov4 listenport=23 connectaddress=localhost
```

- 実験後に telnet server を修了します

```
sudo /etc/init.d/inetutils-inetd stop
```

## ■Client 編

- 1. Windows10 はデフォルトで telnet クライアント機能が無効なので有効化します。

![1](https://user-images.githubusercontent.com/49807271/183861639-5ff9206a-8864-4d54-9b40-fc9730446a71.jpg)

- 2. OK 押下後、インストールが開始します。インストール完了時に次の画面を表示します。

![2](https://user-images.githubusercontent.com/49807271/183861676-6e7d042f-e92b-4568-ae28-5a371e455bf4.jpg)

- 3. telnet server に接続します。

```
telnet localhost

Ubuntu 20.04.4 LTS
DESKTOP-L18OTEK login: ******
Password:
Welcome to Ubuntu 20.04.4 LTS (GNU/Linux 5.4.72-microsoft-standard-WSL2 x86_64)

 * Documentation:  https://help.ubuntu.com
 * Management:     https://landscape.canonical.com
 * Support:        https://ubuntu.com/advantage

  System information as of Wed Aug 10 19:20:10 JST 2022

  System load:  0.0                Processes:             11
  Usage of /:   0.6% of 250.98GB   Users logged in:       0
  Memory usage: 1%                 IPv4 address for eth0: 172.23.75.242
  Swap usage:   0%


140 updates can be applied immediately.
87 of these updates are standard security updates.
To see these additional updates run: apt list --upgradable


Last login: Wed Aug 10 19:13:22 JST 2022 from localhost on pts/1
```

4. 試しに ping 実行

```
ping -c 5 www.google.com
-------------------------------------------------------
PING www.google.com (142.250.207.4) 56(84) bytes of data.
64 bytes from nrt13s54-in-f4.1e100.net (142.250.207.4): icmp_seq=1 ttl=110 time=84.5 ms
64 bytes from nrt13s54-in-f4.1e100.net (142.250.207.4): icmp_seq=2 ttl=110 time=78.7 ms
64 bytes from nrt13s54-in-f4.1e100.net (142.250.207.4): icmp_seq=3 ttl=110 time=108 ms
64 bytes from nrt13s54-in-f4.1e100.net (142.250.207.4): icmp_seq=4 ttl=110 time=105 ms
64 bytes from nrt13s54-in-f4.1e100.net (142.250.207.4): icmp_seq=5 ttl=110 time=82.1 ms

--- www.google.com ping statistics ---
5 packets transmitted, 5 received, 0% packet loss, time 4007ms
rtt min/avg/max/mdev = 78.707/91.752/108.436/12.431 ms
```
