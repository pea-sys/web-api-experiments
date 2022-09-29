# Stun URI Scheme と Turn URI Sheme

共に WebRTC で使用されることがある。

[P2P コネクションを生成作成する場合の定義例]

```js
var myPeerConnection = new RTCPeerConnection({
  iceServers: [
    {
      urls: "stun:openrelay.metered.ca:80",
    },
    {
      urls: "turn:openrelay.metered.ca:80",
      username: "openrelayproject",
      credential: "openrelayproject",
    },
  ],
});
```

「stun:」は STUN URI Scheme  
「turn:」は TURN URI Scheme  
を意味します。

[参考]  
[Open Relay](https://www.metered.ca/tools/openrelay/)  
[WebRTC](https://webrtcforthecurious.com/ja/docs/03-connecting/)
