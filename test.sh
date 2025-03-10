
# -lm links the math.h library definitions
# -g includes debug info into the binary so that we can use debugger on it
gcc -o test_cli.so tests/test_cli.c cli.c cache.c access_metadata.c -lm -g && 
	./test_cli.so

echo "test_cli.so: $?"

gcc -o part_1_test.so tests/part_1_test.c cache.c access_metadata.c -lm -g && 
	./part_1_test.so

echo "part_1_test.so: $?"

gcc -o part_2_test.so tests/part_2_test.c cache.c access_metadata.c -lm -g && 
	./part_2_test.so

echo "part_2_test.so: $?"

# Example debugger usage
#
# gdb ./test_cli.so
# > run
# > bt
#
# Then it tells you what happened.

./build.sh
echo "Project Build: $?"
