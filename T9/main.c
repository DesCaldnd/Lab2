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
bool check_frac(size_t radix, ld frac, enum error_type* status);
ll greatest_common_divider(ll a, ll b);
struct ll_arr simple_dividers(ll n, enum error_type* status);
bool is_subset(struct ll_arr lhs, struct ll_arr rhs);

int main()
{
    check_n_fractions(8, 2, (ld)0.1, (ld)0.0625);
}

int check_n_fractions(size_t radix, size_t count, ...)
{
    va_list fracs;
    va_start(fracs, count);
    for (int i = 0; i < count; ++i)
    {
        ld f = va_arg(fracs, ld);
        enum error_type status;
        bool res = check_frac(radix, f, &status);
        if (status == CORRECT)
            printf("Fraction %LF has%s finite representation in number system with radix = %zu\n", f, res ? "" : "n`t", radix);
        else
            printf("Bad alloc\n");
    }
    va_end(fracs);
}

bool check_frac(size_t radix, ld frac, enum error_type* status)
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

    denominator /= divider;

    enum error_type s_1, s_2;

    struct ll_arr den_div = simple_dividers(denominator, &s_1);

    if (s_1 == ERROR)
    {
        *status = ERROR;
        return false;
    }

    struct ll_arr rad_div = simple_dividers(radix, &s_2);

    if (s_2 == ERROR)
    {
        *status = ERROR;
        free(den_div.arr);
        return false;
    }

    bool res = is_subset(den_div, rad_div);

    *status = CORRECT;
    free(den_div.arr);
    free(rad_div.arr);
    return res;
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

bool is_subset(struct ll_arr lhs, struct ll_arr rhs)
{
    if (lhs.len > rhs.len)
        return false;

    int r_index = 0;

    for (int i = 0; i < lhs.len; ++i)
    {
        while (rhs.arr[r_index] != lhs.arr[i] && r_index < rhs.len)
            ++r_index;
        if (r_index == rhs.len)
            return false;
    }
    return true;
}