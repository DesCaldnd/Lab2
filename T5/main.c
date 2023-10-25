//
// Created by Des Caldnd on 10/24/2023.
//
#include "../functions.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

enum flag_type
{ Ro, Zr, Cv, CV, to, TO, mi, mu, md, mf, ST_F, UNDEF };

struct string
{
    char* buf;
    int capacity, length;
};

int overfprintf(FILE* stream, char* format, ...);
enum flag_type get_flag_type(char* str);
void buf_reinit(struct string* str);
void buf_realloc(struct string* str);
void buf_push_back(struct string* str, char val);
void buf_delete(struct string* str);

void insert_roman(int val, struct string *buf);
void integer_to_n_radix_b(int number, int radix, struct string *buf, char (*int_to_char)(int));
int integer_from_n_radix_b(char *string, int radix, enum error_type* error_return, int (*char_to_int)(char), bool (*is_char_correct)(char, int));


char int_to_char_up(int val);
char int_to_char_down(int val);
int char_to_int_up(char);
bool is_char_correct_down(char, int);
int char_to_int_up(char);
bool is_char_correct_down(char, int);

int main()
{
    overfprintf(stdout, "My age: %Cv %CV years", 27, 16, 28, 16);
}


int overfprintf(FILE* stream, char* format, ...)
{
    struct string buf;
    int result = 0;
    int counter = 0;
    enum flag_type f_type;
    bool has_st_flag = false;
    va_list args;
    va_start(args, format);

    buf_reinit(&buf);

    if (buf.buf == NULL)
        return 0;


    while(format[counter] != '\0')
    {
        if (format[counter] == '%' && (f_type = get_flag_type(format + counter + 1)) != ST_F)
        {
            if (has_st_flag)
            {
                buf_push_back(&buf, '\0');
                if (buf.buf == NULL)
                    return result;

                result += vfprintf(stream, buf.buf, args);

                buf_reinit(&buf);

                if (buf.buf == NULL)
                    return result;

                has_st_flag = false;
            }

            switch (f_type)
            {
                case Ro:
                {
                    int arg = va_arg(args, int);
                    insert_roman(arg, &buf);
                    break;
                }
                case Zr:
                {

                    break;
                }
                case Cv:
                {
                    int arg = va_arg(args, int);
                    int radix = va_arg(args, int);
                    integer_to_n_radix_b(arg, radix, &buf, &int_to_char_down);
                    break;
                }
                case CV:
                {
                    int arg = va_arg(args, int);
                    int radix = va_arg(args, int);
                    integer_to_n_radix_b(arg, radix, &buf, &int_to_char_up);
                    break;
                }
                case to:
                {

                    break;
                }
                case TO:
                {

                    break;
                }
                case mi:
                {

                    break;
                }
                case mu:
                {

                    break;
                }
                case md:
                {

                    break;
                }
                case mf:
                {

                    break;
                }
            }

            if (buf.buf == NULL)
                return result;

            counter += 2;
        } else
        {
            if (f_type == ST_F)
                has_st_flag = true;
            buf_push_back(&buf, format[counter]);
            if (buf.buf == NULL)
                return result;
        }
        f_type = UNDEF;
        ++counter;
    }
    buf_push_back(&buf, '\0');
    if (buf.buf == NULL)
        return result;

    result += vfprintf(stream, buf.buf, args);

    buf_delete(&buf);
    va_end(args);

    return result;
}

void insert_roman(int val, struct string *buf) {

    char *huns[] = {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};
    char *tens[] = {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
    char *ones[] = {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};
    int   size[] = { 0,   1,    2,     3,    2,   1,    2,     3,      4,    2};

    if (val < 0)
    {
        buf_push_back(buf, '-');
        if (buf->buf == NULL)
            return;
        val = abs(val);
    }

    while (val >= 1000) {
        buf_push_back(buf, 'M');
        if (buf->buf == NULL)
            return;
        val -= 1000;
    }

    // Add each of the correct elements, adjusting as we go.

    for (int i = 0; i < size[val/100]; ++i)
    {
        buf_push_back(buf, huns[val/100][i]);
        if (buf->buf == NULL)
            return;
    }
    val = val % 100;
    for (int i = 0; i < size[val/10]; ++i)
    {
        buf_push_back(buf, tens[val/10][i]);
        if (buf->buf == NULL)
            return;
    }
    val = val % 10;
    for (int i = 0; i < size[val]; ++i)
    {
        buf_push_back(buf, ones[val][i]);
        if (buf->buf == NULL)
            return;
    }
}

void integer_to_n_radix_b(int number, int radix, struct string *buf, char (*int_to_char)(int))
{
    if (radix < 2 || radix > 36)
        radix = 10;
    int length = logl(number) / logl(radix) + 1;
    if (number < 0)
        ++length;
    int counter = 1;

    char* str = malloc(length * sizeof(char));
    if (str == NULL)
        return;

    if (number < 0)
    {
        str[0] = '-';
        number = abs(number);
    }

    while(number)
    {
        int tmp = number % radix;
        number /= radix;
        str[length - counter] = int_to_char(tmp);
        counter++;
    }
    for (int i = 0; i < length; ++i)
    {
        buf_push_back(buf, str[i]);
        if (buf->buf == NULL)
        {
            free(str);
            return;
        }
    }
    free(str);
}

int integer_from_n_radix_b(char *string, int radix, enum error_type* error_return, int (*char_to_int)(char), bool (*is_char_correct)(char, int))
{
    int result = 0, multiplier = 1;
    bool has_sign_entered = false, is_negative = false;
    int length = strnlen(string, 64);

    for(int i = length - 1; i >= 0; --i)
    {
        if (is_char_correct(string[i], radix) && !has_sign_entered)
        {
            result += char_to_int(string[i]) * multiplier;
            multiplier *= radix;
        } else if ((string[i] == '+' || string[i] == '-') && !has_sign_entered)
        {
            is_negative = string[i] == '-';
            has_sign_entered = true;
        } else
        {
            *error_return = ERROR;
            return 0;
        }
    }
    *error_return = CORRECT;
    return is_negative ? result * -1 : result;
}

void buf_reinit(struct string* str)
{
    if (str->buf != NULL)
        free(str->buf);
    str->buf = malloc(65 * sizeof(char));
    str->capacity = 64;
    str->length = 0;
}

void buf_push_back(struct string* str, char val)
{
    if (str->length >= str->capacity)
    {
        buf_realloc(str);
    }
    if (str->buf == NULL)
        return;

    str->buf[str->length] = val;
    ++str->length;
}

void buf_realloc(struct string* str)
{
    str->buf = realloc(str->buf, (str->capacity * 2 + 2) * sizeof(char));
    str->capacity = str->capacity * 2 + 1;
}

void buf_delete(struct string* str)
{
    free(str->buf);
}

char int_to_char_up(int val)
{
    if (val <= 9)
        return val + '0';
    else
        return val - 10 + 'A';
}

char int_to_char_down(int val)
{
    if (val <= 9)
        return val + '0';
    else
        return val - 10 + 'a';
}

int char_to_int_up(char)
{

}

bool is_char_correct_down(char, int)
{

}

int char_to_int_up(char)
{

}

bool is_char_correct_down(char, int)
{

}

enum flag_type get_flag_type(char* str)
{
    if (str[0] == 'R' && str[1] == 'o')
        return Ro;
    else if (str[0] == 'Z' && str[1] == 'r')
        return Zr;
    else if (str[0] == 'C' && str[1] == 'v')
        return Cv;
    else if (str[0] == 'C' && str[1] == 'V')
        return CV;
    else if (str[0] == 't' && str[1] == 'o')
        return to;
    else if (str[0] == 'T' && str[1] == 'O')
        return TO;
    else if (str[0] == 'm' && str[1] == 'i')
        return mi;
    else if (str[0] == 'm' && str[1] == 'u')
        return mu;
    else if (str[0] == 'm' && str[1] == 'd')
        return md;
    else if (str[0] == 'm' && str[1] == 'f')
        return mf;
    else
        return ST_F;
}