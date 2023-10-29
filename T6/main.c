//
// Created by Des Caldnd on 10/27/2023.
//
#include "../functions.h"
#include <stdarg.h>
#include <math.h>
#include <ctype.h>

enum flag_type
{ Ro, Zr, Cv, CV, St_f, UNDEFINED };

enum flag_type_s
{ INT, DOUBLE, LDOUBLE, LLONG, POINTER, CHAR, STRING, UNDEF };

enum flag_type get_flag_type(char* str);
enum flag_type_s get_flag_type_s(char* str);
int overfscanf(FILE* stream, char* format, ...);
int oversscanf(char* stream, char* format, ...);


bool int_from_roman(char* str, int* out);
bool uint_from_zr(char* str, unsigned int* out);
bool int_from_cv(char* str, int* out, int radix, int (*char_to_int)(char), bool (*is_char_correct)(char, int));

bool build_zr(char *str, int index, unsigned int* out, unsigned int prev, unsigned int cur);

int char_to_int_up(char sym);
int char_to_int_down(char sym);
bool is_char_correct_up(char sym, int radix);
bool is_char_correct_down(char sym, int radix);
int get_offset(char* str, int count);


int main()
{
    int d, k, g, num, base = 10;
	char str[50];
    overfscanf(stdin, "%Ro %Cv %s", &num, &k, 20, str);

    printf("%d %d %s\n", num, k, str);
}


int overfscanf(FILE* stream, char* format, ...)
{
    enum flag_type_s flags[256];
    int f_count = 0;
    struct string buf;
    buf.buf = NULL;
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
        if (format[counter] == '%' && (f_type = get_flag_type(format + counter + 1)) != St_f)
        {
            buf_push_back(&buf, '\0');
            if (buf.buf == NULL)
                return result;
            result += vfscanf(stream, buf.buf, args);
            for (int i = 0; i < f_count; ++i)
            {
                switch (flags[i])
                {
                    case INT:
                    {
                        va_arg(args, int);
                        break;
                    }
                    case DOUBLE:
                    {
                        va_arg(args, double );
                        break;
                    }
                    case LDOUBLE:
                    {
                        va_arg(args, long double);
                        break;
                    }
                    case LLONG:
                    {
                        va_arg(args, long long);
                        break;
                    }
                    case POINTER:
                    {
                        va_arg(args, char*);
                        break;
                    }
                    case CHAR:
                    {
                        va_arg(args, char);
                        break;
                    }
                    case STRING:
                    {
                        va_arg(args, char*);
                        break;
                    }
                }
            }
            f_count = 0;
            buf_reinit(&buf);
            if (buf.buf == NULL)
                return result;

            switch (f_type)
            {
                case Ro:
                {
                    char str[128];
                    int *arg = va_arg(args, int*);
                    if (fscanf(stream, "%s", str) != 0)
                    {
                        if (int_from_roman(str, arg))
                            ++result;
                    }
                    break;
                }
                case Zr:
                {
                    char str[128];
                    unsigned int *arg = va_arg(args, unsigned int*);
                    if (fscanf(stream, "%s", str) != 0)
                    {
                        if (uint_from_zr(str, arg))
                            ++result;
                    }
                    break;
                }
                case Cv:
                {
                    char str[128];
                    int *arg = va_arg(args, int*);
                    int radix = va_arg(args, int);
                    if (scanf("%s", str) != 0)
                    {
                        if (int_from_cv(str, arg, radix, &char_to_int_down, &is_char_correct_down))
                            ++result;
                    }
                    break;
                }
                case CV:
                {
                    char str[128];
                    int *arg = va_arg(args, int*);
                    int radix = va_arg(args, int);
                    if (fscanf(stream, "%s", str) != 0)
                    {
                        if (int_from_cv(str, arg, radix, &char_to_int_up, &is_char_correct_up))
                            ++result;
                    }
                    break;
                }
            }
            counter += 2;
        } else
        {
            if (f_type == St_f)
            {
                flags[f_count] = get_flag_type_s(format + counter + 1);
                ++f_count;
            }
            buf_push_back(&buf, format[counter]);
            if (buf.buf == NULL)
                return result;
        }

        f_type = UNDEFINED;
        ++counter;
    }

    buf_push_back(&buf, '\0');
    if (buf.buf == NULL)
        return result;

    result += vfscanf(stream, buf.buf, args);

    buf_delete(&buf);
    va_end(args);

    return result;
}

int oversscanf(char* stream, char* format, ...)
{
    struct string buf;
    buf.buf = NULL;
    int result = 0;
    int counter = 0;
    int offset = 0;
    enum flag_type f_type;
    va_list args;
    va_start(args, format);

    buf_reinit(&buf);

    if (buf.buf == NULL)
        return 0;

    while(format[counter] != '\0')
    {
        if (format[counter] == '%' && (f_type = get_flag_type(format + counter + 1)) != St_f)
        {
            result += vsscanf(stream + offset, buf.buf, args);
            offset = get_offset(stream, result);
            buf_reinit(&buf);
            if (buf.buf == NULL)
                return result;

            switch (f_type)
            {
                case Ro:
                {
                    char str[128];
                    int *arg = va_arg(args, int*);
                    if (sscanf(stream + offset, "%s", str) != 0)
                    {
                        if (int_from_roman(str, arg))
                            ++result;
                    }
                    break;
                }
                case Zr:
                {
                    char str[128];
                    unsigned int *arg = va_arg(args, unsigned int*);
                    if (sscanf(stream + offset, "%s", str) != 0)
                    {
                        if (uint_from_zr(str, arg))
                            ++result;
                    }
                    break;
                }
                case Cv:
                {
                    char str[128];
                    int *arg = va_arg(args, int*);
                    int radix = va_arg(args, int);
                    if (sscanf(stream + offset, "%s", str) != 0)
                    {
                        if (int_from_cv(str, arg, radix, &char_to_int_down, &is_char_correct_down))
                            ++result;
                    }
                    break;
                }
                case CV:
                {
                    char str[128];
                    int *arg = va_arg(args, int*);
                    int radix = va_arg(args, int);
                    if (sscanf(stream + offset, "%s", str) != 0)
                    {
                        if (int_from_cv(str, arg, radix, &char_to_int_up, &is_char_correct_up))
                            ++result;
                    }
                    break;
                }
            }
            offset += get_offset(stream, result);
            counter += 2;
        } else
        {
            buf_push_back(&buf, format[counter]);
            if (buf.buf == NULL)
                return result;
        }

        f_type = UNDEFINED;
        ++counter;
    }

    buf_push_back(&buf, '\0');
    if (buf.buf == NULL)
        return result;

    result += vsscanf(stream + offset, buf.buf, args);

    buf_delete(&buf);
    va_end(args);

    return result;
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
    else
        return St_f;
}

bool int_from_roman(char* str, int* out)
{
    char *huns[] = {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};
    char *tens[] = {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
    char *ones[] = {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};
    int   size[] = { 0,   1,    2,     3,    2,   1,    2,     3,      4,    2};

    int res = 0;
    int counter = 0;
    bool is_negative = false;

    if (str[0] == '-')
    {
        is_negative = true;
        ++counter;
    }

    while(str[counter] == 'M')
    {
        res += 1000;
        ++counter;
    }

    for (int i = 9; i >= 1 ; --i)
    {
        bool has = true;
        for (int j = 0; j < size[i]; ++j)
        {
            has = has && (str[counter + j] == huns[i][j]);
        }
        if (has)
        {
            counter += size[i];
            res += i * 100;
            break;
        }
    }
    for (int i = 9; i >= 1 ; --i)
    {
        bool has = true;
        for (int j = 0; j < size[i]; ++j)
        {
            has = has && (str[counter + j] == tens[i][j]);
        }
        if (has)
        {
            counter += size[i];
            res += i * 10;
            break;
        }
    }
    for (int i = 9; i >= 1 ; --i)
    {
        bool has = true;
        for (int j = 0; j < size[i]; ++j)
        {
            has = has && (str[counter + j] == ones[i][j]);
        }
        if (has)
        {
            counter += size[i];
            res += i;
            break;
        }
    }
    *out = res;
    return str[counter] == '\0';
}

bool uint_from_zr(char* str, unsigned int* out)
{
    return build_zr(str, 0, out, 0, 1);
}

bool int_from_cv(char* str, int* out, int radix, int (*char_to_int)(char), bool (*is_char_correct)(char, int))
{
    if (radix < 2 || radix > 36)
        radix = 10;
    enum error_type state;
    int res = integer_from_n_radix(str, radix, &state, char_to_int, is_char_correct);
    *out = res;
    return state == CORRECT;
}

bool build_zr(char *str, int index, unsigned int* out, unsigned int prev, unsigned int cur)
{
    if (str[index] != '0' && str[index] != '1')
        return false;
    if (str[index + 2] == '\0')
    {
        if (str[index + 1] != '1')
            return false;

        *out = cur + prev;
        return true;
    } else
    {
        bool status = build_zr(str, index + 1, out, cur, prev + cur);
        if (!status)
            return false;

        if (str[index] == '1')
            *out += cur + prev;

        return true;
    }
}

int char_to_int_up(char sym)
{
    if(sym >= '0' && sym <= '9')
        return sym - '0';
    else if(sym >= 'A' && sym <= 'Z')
        return sym - 'A' + 10;
    else
        return -1;
}

int char_to_int_down(char sym)
{
    if(sym >= '0' && sym <= '9')
        return sym - '0';
    else if(sym >= 'a' && sym <= 'z')
        return sym - 'a' + 10;
    else
        return -1;
}

bool is_char_correct_up(char sym, int radix)
{
    int a = char_to_int_up(sym);
    return a >= 0 && a < radix;
}

bool is_char_correct_down(char sym, int radix)
{
    int a = char_to_int_down(sym);
    return a >= 0 && a < radix;
}

int get_offset(char* str, int count)
{
    int res = 0;
    for (int i = 0; i < count; ++i)
    {
        while (!isspace(str[res]))
            ++res;
        while (isspace(str[res]))
            ++res;
    }

    return res;
}

enum flag_type_s get_flag_type_s(char* str)
{
    if (str[0] == 'i' || str[0] == 'd' || str[0] == 'x' || str[0] == 'X' || str[0] == 'o')
        return INT;
    else if((str[0] == 'l' && str[1] == 'f') || str[0] == 'f')
        return DOUBLE;
    else if(str[0] == 'L' && str[1] == 'F')
        return LDOUBLE;
    else if(str[0] == 'p' || str[0] == 'n')
        return POINTER;
    else if(str[0] == 'c')
        return CHAR;
    else if(str[0] == 's')
        return STRING;
    else
        return UNDEF;
    
    //INT, DOUBLE, LDOUBLE, LLONG, POINTER, CHAR, STRING, UNDEF
}
