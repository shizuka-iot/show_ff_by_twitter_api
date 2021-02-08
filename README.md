# Raspberry Piで作るTwitterのフォロー・フォロワーを表示するIoTデバイスを作ろう！！

## 概要
twitterAPIを使って取得したフォロー数・フォロワー数や最新のツイートについたいいね数やリツイート数を7セグLEDに表示させます。

事前に各自TwitterAPIの__コンシューマーキー__・__コンシューマーシークレット__・__アクセストークン__・__アクセストークンシークレット__を発行しておく。

上記のキーを用いてTwitterOAuthでPHPからtwitterにログインし、情報を取得。

取得した情報をff.txtに書き込み。

書き込んだテキストファイルをc言語で記述したスクリプトファイルから呼び出して7セグに表示する。

一定時間ごとにPHPスクリプトを実行するシェルスクリプトで値を更新します。


## 用意するもの

* ラズベリーパイ本体

	※今回はRaspberryPi3 model Bを使用していますが、zeroや2,4等でも問題ありません

* twitterAPIの各キー

* 7セグメントLED 3桁2個

	※アノードコモン・カソードコモンどちらでも可。

	※私は３桁を使用していますが、４桁や３桁を２個以上組み合わせて表示したい桁数を選んでください。私はFFが少ないので３桁にしました。

* 74HC595 シフトレジスタ 1個

	※本来なら7セグ1個につき1個必要ですが、実装が面倒なので1個のみ使用しています。ちゃんとしたものを作りたいなら2個必要です。

* 7セグ用電流制限抵抗 カーボン抵抗7個

	※抵抗値は7セグの仕様によるので各自計算してください。

* プルダウン抵抗 カーボン抵抗1本 1Kオーム程度

	※プルダウンできれば良いので10Kでも100Kでも問題ないと思います。

* 単極単投モーメンタリースイッチ

	※FF表示/最新のツイートのリツイート数・いいね数を切り替えるスイッチです。

	※信号を送るだけなので何でもオッケーです。

* ブレッドボード・ジャンプワイヤー等

## 使用するソフト等
* php
* apache2
* twitterAPI
* twitteroauth

## インストール
* sudo apt install php
* sudo apt install apache2

## 回路図に沿って配線してください
![diagram](diagram.png)


