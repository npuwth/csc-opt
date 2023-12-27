basename=$1
../build/csc-opt --backend=c ./tac/$1.tac -o $basename.tac.c
echo $basename
gcc $basename.tac.c -o $basename.tac.bin 
./bin/$basename.bin 1> $basename.bin.txt 2> /dev/null
./$basename.tac.bin 1> $basename.tac.bin.txt 2> /dev/null
md5sum $basename.bin.txt $basename.tac.bin.txt
rm $basename.tac.c
rm $basename.tac.bin
rm $basename.tac.bin.txt
rm $basename.bin.txt

