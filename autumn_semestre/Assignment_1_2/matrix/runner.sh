FILENAME=$1
gcc -Wall "$FILENAME.c" $(python3-config --includes --ldflags) -shared -fPIC -fsanitize=address -o "$FILENAME.so"