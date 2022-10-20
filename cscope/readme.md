# exec_scope_files.sh 的解释

    #!/bin/bash
    find $1 -path $1/scripts* -prune -o -name *.[chxsS] -print > ./cscope.files
    cscope -Rbkq

exec_scope_files.sh 用来在下载完成的内核代码中建立cscope的数据库，用来帮助在vim中进行跳转

第一个find命令用来生成一个文件集合，cscope会在这个文件集合范围内搜索符号（这样能够缩小搜索范围）

find -path xxx -prune 表示排除xxx目录(-prune True;  if  the  file  is  a  directory, do not descend into it.)

-o 表示 逻辑或，也就是说在前一个动作（prune）是false的之后会执行之后的动作

即：将后缀为.c .h .x .s .S的文件名称输出

最终写入cscope.files

第二个cscope命令的参数
-R 递归搜索
-b 只生成索引文件，不进入cscope的界面
-k 内核模式，不去搜索/usr/include
-q 生成cscope.in.out和cscope.po.out文件，加快cscope的索引速度
