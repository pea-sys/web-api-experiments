[grpc]  
gRPC では、クライアントアプリケーションは、ローカルオブジェクトであるかのように、別のマシン上のサーバーアプリケーションのメソッドを直接呼び出すことができるため、分散アプリケーションやサービスを簡単に作成できます。  
gRPC は、インターフェイス定義言語（IDL）としても、基盤となるメッセージ交換形式としても、プロトコルバッファを使用できます。
デフォルトでは、gRPC はプロトコルバッファを使用します。
proto3 が C＃コードジェネレーターでサポートされています。
.proto メッセージタイプを定義するために使用するファイルは次のとおりです。

```
syntax = "proto3";

option csharp_namespace = "GrpcGreeter";

package greet;

// The greeting service definition.
service Greeter {
  // Sends a greeting
  rpc SayHello (HelloRequest) returns (HelloReply);
}

// The request message containing the user's name.
message HelloRequest {
  string name = 1;
}

// The response message containing the greetings.
message HelloReply {
  string message = 1;
}
```

- ファイルの最初の行は、proto3 構文を使用していることを示しています。これを行わないと、プロトコルバッファコンパイラは proto2 を使用していると見なします。これは、ファイルの最初の空でない非コメント行である必要があります。
- メッセージ定義は、この HelloRequest タイプのメッセージに含めるデータごとに 1 つずつ、1 つのフィールド（名前/値のペア）を指定します。各フィールドには名前とタイプがあります。
- メッセージ定義の各フィールドには一意の番号が付いています。これらのフィールド番号は、メッセージバイナリ形式でフィールドを識別するために使用されます。
- 1〜15 の範囲のフィールド番号は、フィールド番号とフィールドのタイプを含めて、エンコードに 1 バイトかかることに注意してください。16 から 2047 の範囲のフィールド番号は 2 バイトを取ります。したがって、非常に頻繁に発生するメッセージ要素用に 1 から 15 までの数字を予約する必要があります。
- .net でプロトコルバッファを扱うには、Google.Protobuf(ランタイム)を Nuget でインストールする
- .net で proto ファイルをコンパイルし、コード生成するためには Grpc.Tools を Nuget でインストールする
- .net で grpc クライアントとして rpc するためには Grpc.Net.Client を Nuget でインストールする

[参考 URL]

- [Protocol Buffer](https://developers.google.com/protocol-buffers/docs/proto3)
- [gRPC](https://www.grpc.io/docs/what-is-grpc/introduction/)
- [gPPC 入門](https://knowledge.sakura.ad.jp/24059/)
