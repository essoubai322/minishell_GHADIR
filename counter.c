#include <stdio.h>

int main()
{
    int i = 0;
    while (i < 20)
    {
        if (i % 2 == 0)
            printf("%d\n", i);
        i++;
    }
}