# Dict URI

クライアントが、自然言語の辞書データベースのセットから辞書定義にアクセスすることを可能にする TCP トランザクションベースのクエリ/応答プロトコル(RFC2229)。
サーバーとクライアントは TCP の Port2628 で通信します。

調べてみたが活用事例があまり見つからない。  
取り合えず、手っ取り早く Windows で動作確認したかったので、[SorceForge](https://sourceforge.net/projects/dictc/)から dict クライアントプログラムをダウンロードした。

- 1.ダウンロードした Dict クライアントを起動し、Setup ボタンクリック
  ![1](https://user-images.githubusercontent.com/49807271/190901201-dc7a84a1-4a89-4ea5-b28c-d0df0185f867.png)

- 2.問い合わせするサーバを選択する。恐らく一番メジャーな dict.org を選択し、「OK」をクリック。
  ![2](https://user-images.githubusercontent.com/49807271/190901365-efa1e5cb-0a34-4963-9d96-6cff9cf4c0d8.png)

* 3.Connect クリック
  接続すると次のログが出力されます。

```
< Connecting to dict.org ...
220 dict.dict.org dictd 1.12.1/rf on Linux 4.19.0-10-amd64 <auth.mime> <141437933.11524.1663502977@dict.dict.org>
```

- 4.索引したいワードを入力して、Send クリック
  ここでは Dictionary というワードで索引しています。  
  索引結果を取得できます。

```
< Dictionary
 5  entries found:

nguage dictionary, gazetteer, general dictionary,
   geological dictionary, gloss, glossary, gradus, jargon, language,
   lexicon, nomenclator, onomasticon, palaver, phrase book,
   polyglot dictionary, promptorium, rhyming dictionary,
   science dictionary, slang dictionary, specialized dictionary,
   synonym dictionary, synonymy, terminology, thesaurus,
   treasury of words, unabridged dictionary, vocabulary, word list,
   wordbook



.
151 "dictionary" foldoc "The Free On-line Dictionary of Computing (30 December 2018)"
dictionary

   1. {data dictionary}.

   2. {associative array}.

   3. {Free On-line Dictionary of Computing}.

.
151 "dictionary" devil "The Devil's Dictionary (1881-1906)"
DICTIONARY, n.  A malevolent literary device for cramping the growth
of a language and making it hard and inelastic.  This dictionary,
however, is a most useful work.

.
250 ok [d/m/c = 5/0/141; 0.000r 0.000u 0.000s]
```

[参考]  
 [DICT](https://tradic.osdn.jp/dict)
