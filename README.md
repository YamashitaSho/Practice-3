# Practice-3
##概要
オリジナル文字列から検索文字列を探し出し、該当する文字列があれば "<"b">" タグで囲って結果を表示する。複数個存在すれば、すべて囲む。検索する文字列が見つからなければ該当なしメッセージを出す。それ以外はエラーメッセージを出す。
##コンパイル方法
gcc -Wall -o tcmkb tcmkb.c
##実行方法
tcmkb -f "abcdefg" -o "bcd"

tcmkb -f "string1" -o "string2"
に限り
tcmkb "string1" "string2"
と書くことが出来る。
