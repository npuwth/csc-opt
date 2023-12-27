#include <stdio.h>
#define WriteLine() printf("\n");
#define WriteLong(x) printf(" %lld", (long)x);
#define ReadLong(a) if (fscanf(stdin, "%lld", &a) != 1) a = 0;
#define long long long

void main()
{
    long a, b, c, d, e, f;
    a = 123;
    ReadLong(b);
    c = a+b;
    d = c+1;
    e = d * 2;
    f = e - d;
}