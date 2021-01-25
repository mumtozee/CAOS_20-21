#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    char command[] = "print(eval(input()))";
    execlp("python3", "python3", "-c", command, NULL);
    return 0;
}
