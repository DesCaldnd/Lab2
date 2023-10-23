//
// Created by Des Caldnd on 10/23/2023.
//
#include "../functions.h"

ld dihotomy_equation(ld a, ld b, ld eps, ld (*func)(ld));
ld f(ld x)
{
    return 3 * x - 7;
}

int main()
{
    printf("%LF", dihotomy_equation(2, 3, 0.00001, &f));
}

ld dihotomy_equation(ld a, ld b, ld eps, ld (*func)(ld))
{
    ld mid = (a + b) / 2, y;

    while (fabsl(y = func(mid)) > eps)
    {
        if (y > 0)
            b = mid;
        else
            a = mid;
        mid = (a + b) / 2;
    }
    return mid;
}