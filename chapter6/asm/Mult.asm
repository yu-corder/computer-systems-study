//RAM[2]を0に初期化
@R2
M=0

//カウンタiにR1の値をセット
@R1
D=M
@i
M=D

//最初からiが0なら計算せずに即終了する
@i
D=M
@END
D;JEQ

(LOOP)
//現代のRAM[2]の値とR0の値を足し合わせる
@R2
D=M

//R0の値を足す
@R0
D=D+M

//計算結果をRAM[2]に書き戻す
@R2
M=D

//カウンタiを1減らす
@i
M=M-1

//カウンタiが0でなければLOOPに戻る
@i
D=M
@LOOP
D;JNE

(END)
@END
0;JMP