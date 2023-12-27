#include <stdio.h>
#define WriteLine() printf("\n");
#define WriteLong(x) printf(" %lld", (long)x);
#define ReadLong(a) if (fscanf(stdin, "%lld", &a) != 1) a = 0;
#define long long long

void main()
{
    long a, b, c, d, e, f;
    a = 123;
    b = a;
    c = b;
    d = c;
    e = d;
    f = e;
    WriteLong(f);
    WriteLine();
}