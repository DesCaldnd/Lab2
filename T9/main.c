//
// Created by Des Caldnd on 10/23/2023.
//
#include "../functions.h"
#include <stdarg.h>
#include <stdlib.h>

struct ll_arr
{
    ll* arr;
    size_t len, capacity;
};

int check_n_fractions(size_t radix, size_t count, ...);
bool check_frac(size_t radix, ld frac);
ll greatest_common_divider(ll a, ll b);
struct ll_arr simple_dividers(ll n, enum error_type* status);

int main()
{
    enum error_type status;
    struct ll_arr a = simple_dividers(145341, &status);

    if (status == CORRECT)
    {
        for (int i = 0; i < a.len; ++i)
        {
            printf("%lld ", a.arr[i]);
        }
        free(a.arr);
    } else
    {
        printf("Bad alloc\n");
    }

}

int check_n_fractions(size_t radix, size_t count, ...)
{
    va_list fracs;
    va_start(fracs, count);
    for (int i = 0; i < count; ++i)
    {
        ld f = va_arg(fracs, ld);
        printf("Fraction %LF has%s finite representation in number system with radix = %zu", f, check_frac(radix, f) ? "" : "n`t", radix);
    }
    va_end(fracs);
}

bool check_frac(size_t radix, ld frac)
{
    frac = fabsl(frac);
    ll denominator = 1;

    while (frac != (ld)(ll)frac)
    {
        frac *= 10;
        denominator *= 10;
    }
    ll numerator = frac;
    ll divider = greatest_common_divider(numerator, denominator);

    numerator /= divider;
    denominator /= divider;
}

ll greatest_common_divider(ll a, ll b) {
    if (a % b == 0)
        return b;
    if (b % a == 0)
        return a;

    if (a > b)
        return greatest_common_divider(a % b, b);
    return greatest_common_divider(a, b % a);
}

struct ll_arr simple_dividers(ll n, enum error_type* status)
{
    struct ll_arr res;
    res.capacity = log2(n);
    res.arr = malloc(res.capacity * sizeof(ll));
    if (res.arr == NULL)
    {
        *status = ERROR;
        return res;
    }

    *status = CORRECT;
    res.len = 0;

    ll i = 2;
    bool written = false;

    while (n != 1)
    {
        if (n % i == 0)
        {
            if (!written)
            {
                res.arr[res.len] = i;
                ++res.len;
                written = true;
            }
            n /= i;
        } else
        {
            ++i;
            written = false;
        }
    }
    return res;
}