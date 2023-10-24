//
// Created by Des Caldnd on 10/24/2023.
//
#include "../functions.h"
#include <stdlib.h>

enum flag_type
{ Ro, Zr, Cv, CV, to, TO, mi, mu, md, mf, UNDEF };

struct string
{
    char* buf;
    int capacity, length;
};

int overfprintf(FILE* stream, char* format, ...);
enum flag_type get_flag_type(char* str);
void buf_realloc(struct string* str);
void buf_push_back(struct string* str, char val);
void convertToRoman (unsigned int val, char *res);




int overfprintf(FILE* stream, char* format, ...)
{
    struct string buf;
    buf.buf = malloc(65 * sizeof(char));
    buf.capacity = 64;
    buf.length = 0;

    if (buf.buf == NULL)
        return 0;

    int counter = 0, offset = 0;

    while(format[counter] != '\0')
    {
        if (format[counter] == '%')
        {

        } else

    }
    buf_push_back(&buf, '\0');
    if (buf.buf == NULL)
        return 0;
    int result;
}

void convertToRoman (unsigned int val, char *res) {
    char *huns[] = {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};
    char *tens[] = {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
    char *ones[] = {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};
    int   size[] = { 0,   1,    2,     3,    2,   1,    2,     3,      4,    2};

    //  Add 'M' until we drop below 1000.

    while (val >= 1000) {
        *res++ = 'M';
        val -= 1000;
    }

    // Add each of the correct elements, adjusting as we go.

    strcpy (res, huns[val/100]); res += size[val/100]; val = val % 100;
    strcpy (res, tens[val/10]);  res += size[val/10];  val = val % 10;
    strcpy (res, ones[val]);     res += size[val];

    // Finish string off.

    *res = '\0';
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
        return UNDEF;
}