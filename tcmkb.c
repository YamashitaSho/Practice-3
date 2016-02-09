#include <stdio.h>

/*
C言語学習用課題
コマンドラインを利用し、文字列oから指定文字列fを検索する
該当するものがあれば<b>タグで囲む。

コマンドライン
-o string: 検索対象テキスト
-f string: 検索文字列

tcmkb -f string1 -o string2 を
tcmkb string1 string2
と書くことができる。

 エラーメッセージ
・引数が不正な場合
・その他

*/
int argchk(char* arg);				//コマンドライン引数の識別
void errmsg(int p);					//エラーメッセージ表示
int srch(char* org, char* match);	//検索
char BoldInsert(char* org, char* boldspot, char* result, int srchcount, int orglength);

int Strlength(const char* s);		//文字列の長さを調べる

int i; int j; int k;

int main(int argc, char *argv[]){
	
	int chk = 0;						//エラー検出用変数 0でなければエラー
	int s_org = 0;					//検索対象テキスト指定のオプション
	int s_find = 0;					//検索文字列指定のオプション
	
//	ループ毎に1つずつコマンドラインオプションを取得する：argchk(argv[i])	
	for(i = 1; i < argc; i++){	//1つ目のパラメータは実行ファイルなので弾く
		if (argchk(argv[i]) == 15){	//すでに検出されたものでないかチェック
			if (s_org == 0){
				s_org = i;			//-oがあった配列番号
			}else{
				chk = 1;
			}
		}
		if (argchk(argv[i]) == 6){
			if (s_find == 0){
				s_find = i;			//-fがあった配列番号
			}else{
				chk = 1;
			}
		}
	}
//-f -oオプションの指定漏れがないかチェック
//s_org,s_findが全て0かつargc==3 → 省略と判断
//省略に当てはまらず、0のものがある → 引数エラー
	if (!(s_org && s_find) && argc == 3){
		s_org = 1;
		s_find = 0;
	}else if(!(s_org && s_find)){
		chk = 1;
	}
//	printf("s_org:%d\ns_find:%d\n", s_org, s_find);
	if (chk == 0) {
		chk = srch(argv[s_org+1], argv[s_find+1]);		//検索
	}
	errmsg(chk);					//エラーメッセージを表示
	return 0;
}


//コマンドライン引数の識別
int argchk(char* arg){
	if (arg[0] == '-'){
		if (arg[1] == 'f'){
			return 6;
		} else if (arg[1] == 'o'){
			return 15;
		}
	}else{
		return 0;
	}
	return 0;
}


//検索・置換
int srch(char *org, char *match){
	//	方針:1文字目を探す→全一致かどうか確認する
	int orglength = Strlength(org);
	int matchlength = Strlength(match);
	int scm = orglength - matchlength + 1;	//サーチ回数
	if (scm <= 0) {return 2;}				//検索文字列が対象文字列より長い=>エラー
	
	int srchcount = 0;						//発見回数
	char boldspot[orglength+1];						//<b>挿入位置の配列
	for (i=0;i<=orglength+1;i++){boldspot[i]='0';}	//初期化
	boldspot[orglength+1] = '\0';			//<b>最後にnullを入れる
	
	for(i = 0; i < scm;){					//ハズレまたは発見が確定した時にi++
		
		if (org[i] == match[0]) {			//1文字目が一致
			for(j = 0; j < matchlength; j++){
				
				if (org[i+j] != match[j]) {
					i++;
					break;
				}							//2文字目以降でハズレ
				
				if ((org[i+j] == match[j]) & (j == matchlength - 1)) {
					srchcount++;			//発見
					boldspot[i] += 1;		//<b>マーキング
					for (k = 0; k < matchlength-1 ; k++){
						i++;				//検索文字数分インデックスを進める
					}
					boldspot[i+1] += 2;		//</b>マーキング
					i++;
					break;
				}
			}
		} else {							//1文字目もハズレ
			i++;
		}
	}

	if(srchcount >= 1){
		char result[orglength + srchcount*7];
		for (int i=0;i<(orglength + srchcount*7);i++){result[i]='0';}	//0でフォーマット
		result[orglength + srchcount*7] = '\0';
		
//		printf("orgl+srchcount*7=%d\n", orglength+srchcount*7);
//		printf("boldspot:%s\n", boldspot);
		
		BoldInsert(org, boldspot, result, srchcount, orglength);	//resultで<b>挿入後の文を受け取る
		
		printf("%s\n", result);
//		printf("文字列\"%s\"が%d個見つかりました。\n", match, srchcount);
	}else{
		printf("文字列\"%s\"は見つかりませんでした。\n", match);
	}

	return 0;
}
//<b>挿入関数
char BoldInsert(char *org, char *boldspot, char *result, int srchcount, int orglength){
//	printf("boldspot:%s\n", boldspot);
	int cursor = 0;
	for (int i=0;i<=orglength;i++){
		switch (boldspot[i]){
		  case '0':
			result[cursor] = org[i];
			cursor += 1;				//1バイト進む
			break;
		  case '1':
			result[cursor] = '<';
			result[cursor+1] = 'b';
			result[cursor+2] = '>';
			result[cursor+3] = org[i];
			cursor += 4;				//4バイト進む:<b>挿入
			break;
		  case '2':
			result[cursor] = '<';
			result[cursor+1] = '/';
			result[cursor+2] = 'b';
			result[cursor+3] = '>';
			result[cursor+4] = org[i];
			cursor += 5;				//5バイト進む:</b>挿入
			break;
		  case '3':
			result[cursor] = '<';
			result[cursor+1] = '/';
			result[cursor+2] = 'b';
			result[cursor+3] = '>';
			result[cursor+4] = '<';
			result[cursor+5] = 'b';
			result[cursor+6] = '>';
			result[cursor+7] = org[i];
			cursor += 8;				//8バイト進む:</b><b>挿入
			break;
		  default:
			printf("ERROR\n");
			break;
		}

//		printf("i:%d\nresult:%s\n",i,result);
	}
	return *result;
}


//文字列の長さを調べる \0は含まない
int Strlength(const char* s){
	int n=0;while (*s != '\0'){s++; n++;}
	return (n);
}


//エラーメッセージ
void errmsg(int chk){
	switch (chk){
	  case 0:
		break;
	  case 1:
		printf("引数が不正です。\n");
		break;
	  case 2:
		printf("検索文字列が探索文字列より長いです。\n");
		break;
	  default:
		printf("不正なエラーです。\n");
		break;
	}
}