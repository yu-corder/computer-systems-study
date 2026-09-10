//無限ループ(入力待ち)
(LOOP)

//キーボード入力があれば、スクリーンを黒に(16384 - 24575)
@16384
D=A

@point
M=D

@i
M=0

@24575
D=A

@16384
D=D-A
@fi
M=D

//キーボード入力
@24576
D=M
//キーボードが入力されていなければENDへ
@WHITE
D;JEQ

(LOOP2)
@i
D=M
@point
A=D+M
M=-1
D=D+1
@i
M=D
@fi
D=M
D=D-1
M=D
@LOOP2
D;JGT

@LOOP
0;JMP

(WHITE)
(LOOP3)
@i
D=M
@point
A=D+M
M=0
D=D+1
@i
M=D
@fi
D=M
D=D-1
M=D
@LOOP3
D;JGT

@LOOP
0;JMP