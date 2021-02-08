#include <wiringPi.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define LINE_MAX 4

#define MSBFIRST 0 // most significant bit first
#define LSBFIRST 1 // least significant bit first

#define ONES_ANODE 5
#define TENS_ANODE 6
#define HUNDREDS_ANODE 13

#define ONES_ANODE2 17
#define TENS_ANODE2 27
#define HUNDREDS_ANODE2 22

#define SWITCH 26 // スイッチ入力

#define SI 16 //シリアルデータ
#define SCK 20//シリアルクロック
#define RCK 21 //ラッチクロック

const int output_pins[] = {
	ONES_ANODE,
	TENS_ANODE,
	HUNDREDS_ANODE,
	ONES_ANODE2,
	TENS_ANODE2,
	HUNDREDS_ANODE2,
	SI,
	SCK,
	RCK,
};

int ones_digit = 0;
int tens_digit = 0;
int hundreds_digit = 0;
int	thousands_digit = 0;
int	ten_thousands_digit = 0;
int	hundred_thousands_digit = 0;

int old[4];

//ビットデータと７セグのレイアウトを定義
//カソードコモンタイプのLEDを使用
const int bit_patterns[] = {
	0b11111100,//0
	0b01100000,//1
	0b11011010,//2
	0b11110010,//3
	0b01100110,//4
	0b10110110,//5
	0b10111110,//6
	0b11100100,//7
	0b11111110,//8
	0b11110110,//9
	0b00000000,//10(off)
	0b00000001,//11 decimal point
	0b10000110,//12 c
};

// 8ビット出力
void shiftOut (int si_pin, int sck_pin, int order, int bit_pattern)
{
	int i = 0;
	if (order == MSBFIRST)
	{
		for (i = 7; i >= 0; --i)
		{
			// アノードコモンだとビットパターンの反転(^ 0xff)が必要
			digitalWrite (si_pin, ((bit_pattern ^ 0xff) >>i) &1);
			digitalWrite (sck_pin, HIGH);
			digitalWrite (sck_pin, LOW);
		}
	}
	else
	{
		for (i = 0; i < 8; ++i)
		{
			digitalWrite (si_pin, ((bit_pattern ^ 0xff) >>i) &1);
			digitalWrite (sck_pin, HIGH);
			digitalWrite (sck_pin, LOW);
		}
	}
}


//数字の一桁だけを表示。
//もし1024を表示したい場合、
//print_a_digit(1),print_a_digit(0),numprnt(2),print_a_digit(4)のように
//分割しないといけない。
void print_a_digit(int index_number)
{
	digitalWrite(RCK, LOW);
	shiftOut(SI, SCK, LSBFIRST, bit_patterns[index_number]);
	digitalWrite(RCK, HIGH);
	digitalWrite(RCK, LOW);
}

// 各桁のカソードのハイロウを指定
void setAnodeBit(
		int ones_anode_bit, 
		int tens_anode_bit, 
		int hundreds_anode_bit,
		int thousands_anode_bit, 
		int ten_thousands_anode_bit, 
		int hundred_thousands_anode_bit
		)
{
	digitalWrite(ONES_ANODE, ones_anode_bit);
	digitalWrite(TENS_ANODE, tens_anode_bit);
	digitalWrite(HUNDREDS_ANODE, hundreds_anode_bit);
	digitalWrite(ONES_ANODE2, thousands_anode_bit);
	digitalWrite(TENS_ANODE2, ten_thousands_anode_bit);
	digitalWrite(HUNDREDS_ANODE2, hundred_thousands_anode_bit);
}

//ledを全消灯
//出力をHIGHにするこで同電位となり電流は流れない。
void turnOffAllLed()
{
	setAnodeBit(0, 0, 0, 0, 0, 0);
	print_a_digit(10);
}

// 1の位を表示
void printOnesDigit(int number)
{
	setAnodeBit(1, 0, 0, 0, 0, 0);
	print_a_digit(number);
	delay(2);
	turnOffAllLed();
}

// 10の位を表示
void printTensDigit(int number)
{
	setAnodeBit(0, 1, 0, 0, 0, 0);
	print_a_digit(number);
	delay(2);
	turnOffAllLed();
}

// 100の位を表示
void printHundredsDigit(int number)
{
	setAnodeBit(0, 0, 1, 0, 0, 0);
	print_a_digit(number);
	delay(2);
	turnOffAllLed();
}

// 1000の位を表示
void printThousandsDigit(int number)
{
	setAnodeBit(0, 0, 0, 1, 0, 0);
	print_a_digit(number);
	delay(2);
	turnOffAllLed();
}

// 10000の位を表示
void printTenThousandsDigit(int number)
{
	setAnodeBit(0, 0, 0, 0, 1, 0);
	print_a_digit(number);
	delay(2);
	turnOffAllLed();
}

// 100000の位を表示
void printHundredThousandsDigit(int number)
{
	setAnodeBit(0, 0, 0, 0, 0, 1);
	print_a_digit(number);
	delay(2);
	turnOffAllLed();
}


// 全ての桁を出力
int printAllDigit(int number, int number2)
{
	int sum = 0;
	sum = number + number2 * 1000;
	ones_digit = sum % 10;
	tens_digit = sum /10 % 10;
	hundreds_digit = sum /100 % 10;
	thousands_digit = sum / 1000 % 10;
	ten_thousands_digit = sum / 10000 % 10;
	hundred_thousands_digit = sum / 100000 % 10;

	printOnesDigit(ones_digit);
	if (number >= 10) printTensDigit(tens_digit);
	if (number >= 100) printHundredsDigit(hundreds_digit);
	printThousandsDigit(thousands_digit);
	if (number2 >= 10) printTenThousandsDigit(ten_thousands_digit);
	if (number2 >= 100) printHundredThousandsDigit(hundred_thousands_digit);
}



// メイン処理
int main (void)
{
	// wiringPiセットアップ
	if(wiringPiSetupGpio() == -1) return -1;

	// 各ピンを出力モードに指定
	int p = 0;
	for ( p=0; p < sizeof(output_pins); p++)
	{
		pinMode( output_pins[p], OUTPUT );
	}
	pinMode( SWITCH, INPUT );

	FILE * fp = NULL;
	char lines[LINE_MAX][256];
	unsigned int i;

	int sw = 0;
	int sw_flag = 0;
	// メインループ
	while(1)
	{
		// スイッチオン
		if (digitalRead(SWITCH) == HIGH)
		{
			delay(2);
			sw_flag = 1;
		}
		// スイッチオフ
		if ( sw_flag == 1 && digitalRead(SWITCH) == LOW)
		{
			sw = (sw+1) %2;
			sw_flag = 0;
		}


		// 読み込みモードでファイルオープン
		fp = fopen("ff.txt", "r");
		// ファイルオープンできなかったときの処理
		if (fp == NULL)
		{
			printf("ファイルオープンエラー\n");
			return -1;
		}

		// 行ごとに配列に格納
		// 条件式の中でやってる
		// fgets(
		// 文字列を格納するための配列, 
		// n行目の最大文字列, 
		// 検査データ)
		// lines[i]は2次元配列
		for (i=0; 
				i<LINE_MAX && 
				fgets(lines[i], sizeof(lines[i]), fp) != NULL; 
				i++)
		{
			// 音声を鳴らしたい場合アンコメント
			// 適当に音声ファイルを用意してください
			/*
			char *command = "omxplayer -o local /home/pi/Music/crrect_answer3.mp3 < /home/pi/Music/cmd | echo . > /home/pi/Music/cmd &";
			if ( atoi(lines[i]) > old[i] )
			{
				system(command);
			}
			old[i] = atoi(lines[i]);
			*/
		}

		// atoi()は数字を数値に変換 #include <stdlib.h>
		if( sw == 0 )
		{
			printAllDigit(atoi(lines[0]), atoi(lines[1]));
		}
		else
		{
			printAllDigit(atoi(lines[3]), atoi(lines[2]));
		}

		fclose(fp);
	}

	return 0;
}
