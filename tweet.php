<?php

require 'common.php';
require 'vendor/autoload.php';
use Abraham\TwitterOAuth\TwitterOAuth;

// twitterにログイン
$twitter = new TwitterOAuth(
	CONSUMER_KEY, 
	CONSUMER_SECRET, 
	ACCESS_TOKEN, 
	ACCESS_TOKEN_SECRET
	);


# 認証ユーザーの情報を取得(stdClassが返ってくる)
$content = $twitter->get("account/verify_credentials");
var_dump($content);

if (isset($content->errors))
{
	echo $content->errors->message . PHP_EOL;
	exit;
}
else
{
	# フォローフォロワー数を記述するファイル
	$file = 'ff.txt';

	# 認証ユーザーのフォロワー数を取得
	$followers = $content->followers_count;

	# 認証ユーザーのフォロー数を取得
	$followings = $content->friends_count;

	#最新の投稿のリツイート数
	$retweet_count = $content->status->retweet_count;

	#最新の投稿のいいね数
	$favorite_count = $content->status->favorite_count;

	// ファイルに書き込み
	file_put_contents($file, $followers . "\n");
	// 追記モード | 排他ロック
	file_put_contents($file, $followings . "\n", FILE_APPEND | LOCK_EX);
	file_put_contents($file, $retweet_count . "\n", FILE_APPEND | LOCK_EX);
	file_put_contents($file, $favorite_count . "\n", FILE_APPEND | LOCK_EX);

	echo "followers = " .$followers. PHP_EOL;
	echo "followings = " .$followings. PHP_EOL;
	echo "retweet = " . $retweet_count. PHP_EOL;
	echo "favorite = " .$favorite_count. PHP_EOL;
}
