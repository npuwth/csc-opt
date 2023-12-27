basename=$1
testnum=10000
../build/csc-opt --backend=c ./tac/$1.tac -n $testnum -o $basename.base.c
../build/csc-opt --backend=c ./tac/$1.tac -n $testnum --opt=scp,dse -o $basename.opt.c
echo $basename
gcc $basename.base.c -o $basename.base.bin
gcc $basename.opt.c -o $basename.opt.bin
# time for ((i=0;i<100;i++)); do
#     ./$basename.base.bin > /dev/null
# done
# time for ((i=0;i<100;i++)); do
#     ./$basename.opt.bin > /dev/null
# done
./$basename.base.bin 1> $basename.base.txt
./$basename.opt.bin 1> $basename.opt.txt
md5sum $basename.base.txt $basename.opt.txt
rm $basename.base.c
rm $basename.opt.c
rm $basename.base.bin
rm $basename.opt.bin
rm $basename.base.txt
rm $basename.opt.txt

