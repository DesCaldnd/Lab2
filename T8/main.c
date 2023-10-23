//
// Created by Des Caldnd on 10/23/2023.
//
#include "../functions.h"
#include <stdlib.h>
#include <stdarg.h>

char* sum_n_radix(unsigned int radix, size_t count, ...);
void sum_two_strings(char** lhs, char* rhs, int radix);
char* realloc_str(char* str, size_t cur_len, size_t add);
short add_digits_n_radix(size_t radix, char* lhs, char rhs, short a);

int main()
{
    char* str = sum_n_radix(8, 3, "5", "175", "3");

    printf("%s", str);

    free(str);
}

char* sum_n_radix(unsigned int radix, size_t count, ...)
{
    va_list strings;
    va_start(strings, count);
    char *current_string = va_arg(strings, char*);
    size_t length = strlen(current_string);

    char* accum = malloc((length + count + 1) * sizeof(char));

    if (accum == NULL)
        return accum;

    for (int i = 0; i < length + count; ++i)
    {
        accum[i] = '0';
    }

    accum[length + count] = '\0';

    for (int i = 0; i < count; ++i)
    {
        sum_two_strings(&accum, current_string, radix);
        if (accum == NULL)
            return accum;
        current_string = va_arg(strings, char*);
    }

    va_end(strings);

    size_t offset = 0;
    length = strlen(accum);

    while (offset < length)
    {
        if (accum[offset] == '0')
            ++offset;
        else
            break;
    }



    if (offset != 0)
    {
        char* tmp = malloc((length - offset + 1) * sizeof(char));

        if (tmp == NULL)
            return accum;

        for (int i = offset; i < length; ++i)
        {
            tmp[i - offset] = accum[i];
        }

        tmp [length - offset] = '\0';
        free(accum);
        accum = tmp;
    }


    return accum;
}

void sum_two_strings(char** lhs, char* rhs, int radix)
{
    int first_l, second_l, first_v = 0, second_v = 0;
    first_l = strlen(*lhs);
    second_l = strlen(rhs);

    for (int i = 0; i < first_l; ++i)
    {
        if ((*lhs)[i] != '0')
            break;
        else
            ++first_v;
    }
    for (int i = 0; i < second_l; ++i)
    {
        if (rhs[i] != '0')
            break;
        else
            ++second_v;
    }

    first_v = first_l - first_v;
    second_v = second_l - second_v;

    if (first_v == second_v && first_l <= first_v)
    {
        *lhs = realloc_str(*lhs, first_l, 2);
        if (*lhs == NULL)
            return;
        first_l += 2;
    } else if (first_l < second_v)
    {
        *lhs = realloc_str(*lhs, first_l, second_v - first_l);
        if (*lhs == NULL)
            return;
        first_l += second_v - first_l;
    }

    int counter = first_l - 1;
    int offset = second_l - first_l;
    short add = 0;

    while (counter >= first_l - second_v)
    {
        add = add_digits_n_radix(radix, *lhs + counter, rhs[counter + offset], add);
        --counter;
        add /= radix;
    }

    while (add != 0)
    {
        add = add_digits_n_radix(radix, *lhs + counter, rhs[counter + offset], add);
        --counter;
        add /= radix;
    }
}

char* realloc_str(char* str, size_t cur_len, size_t add)
{
    char * tmp = malloc((cur_len + add + 1) * sizeof(char));
    if (tmp == NULL)
        return NULL;

    tmp[cur_len + add] = '\0';

    for (int i = 0; i < add; ++i)
    {
        tmp[add] = '0';
    }

    for (int i = 0; i < cur_len; ++i)
    {
        tmp[add + i] = str[i];
    }
    free(str);
    return tmp;
}

short add_digits_n_radix(size_t radix, char* lhs, char rhs, short a)
{
    short res, l, r;

    if (*lhs >= '0' && *lhs <= '9')
        l = *lhs - '0';
    else
        l = *lhs - 'A';

    if (rhs >= '0' && rhs <= '9')
        r = rhs - '0';
    else
        r = rhs - 'A';

    short summ = l + r + a;
    short cur = summ % radix;
    res = summ - cur;
    if (cur >= 0 && cur <= 9)
        *lhs = cur + '0';
    else
        *lhs = cur + 'A';

    return res;
}