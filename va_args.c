#include <stdio.h>
#include <stdarg.h>
#include <string.h>

int myprintf(char *fmt, ...)
{
    va_list args;
    int n;
    char sprint_buf[1024];
    va_start(args, fmt);
    n = vsprintf(sprint_buf, fmt, args);
    va_end(args);
    write(1, sprint_buf, n);
    return n;
}

int main()
{
    char a[10] = "Misoralv";
    char b[10] = "Klose";
    int a1 = 14;
    int n = myprintf("%s %s, his WorldCup goals reach %d.", a,b,a1);
    printf("n=%d\n",n);
    return 0;
}

