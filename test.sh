
# -lm links the math.h library definitions
# -g includes debug info into the binary so that we can use debugger on it
gcc -o test_cli.so tests/test_cli.c cli.c cache.c access_metadata.c -lm -g && 
	./test_cli.so

echo "test_cli.so: $?"

# Example debugger usage
#
# gdb ./test_cli.so
# > run
# > bt
#
# Then it tells you what happened.

./build.sh
echo "Project Build: $?"
