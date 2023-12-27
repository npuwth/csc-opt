basename=$1
../build/csc-opt --backend=c ./tac/$1.tac -o $basename.tac.c
echo $basename
gcc $basename.tac.c -o $basename.tac.bin 
./bin/$basename.bin > $basename.bin.txt
./$basename.tac.bin > $basename.tac.bin.txt
md5sum $basename.bin.txt $basename.tac.bin.txt
rm $basename.tac.c
rm $basename.tac.bin
rm $basename.tac.bin.txt
rm $basename.bin.txt

