//
// Created by Des Caldnd on 10/23/2023.
//
#include <stdarg.h>
#include "../functions.h"

ld average_geom(size_t size, ...);
ld power_ld_int_rec(long double base, int pow);

int main()
{
    printf("%LF\n", average_geom(3, (ld)10, (ld)20, (ld)30));
    printf("%LF\n", power_ld_int_rec(2, 10));
}

ld average_geom(size_t size, ...)
{
    va_list numbers;
    va_start(numbers, size);

    ld accum = 1;

    for (int i = 0; i < size; ++i)
    {
        accum *= va_arg(numbers, ld);
    }
    va_end(numbers);

    return powl(accum, 1L/(ld)size);
}

ld power_ld_int_rec(long double base, int pow){
    if(pow < 0){
        return 1.0L / power_ld_int_rec(base, fabsl(pow));
    }
    else if(pow == 0){
        return 1.0L;
    }
    else if(pow % 2 == 1){
        return power_ld_int_rec(base, pow - 1) * base;
    }
    else{
        long double base_n = power_ld_int_rec(base, pow / 2);
        return base_n * base_n;
    }
}