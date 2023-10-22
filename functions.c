//
// Created by Des Caldnd on 10/3/2023.
//
#include "functions.h"

int int_from_str(char str[], enum error_type* check_state)
{
    int result = 0;
    bool has_sign = false, has_digit = false, is_negative = false;
    for (int i = 0; str[i] != '\0'; ++i) {
        char sym = str[i];
        if (sym >= '0' && sym <= '9')
        {
            has_sign = true;
            has_digit = true;
            result = result * 10 + (sym - '0');
        } else if ((sym == '-' || sym == '+') && !has_sign && !has_digit)
        {
            is_negative = sym == '-';
        } else
        {
            *check_state = ERROR;
            return 0;
        }
    }
    *check_state = CORRECT;
    return is_negative ? result * -1 : result;
}

double double_from_str(char str[], enum error_type* check_state)
{
    int result_up = 0;
    double result_down = 0, prec = 10;
    bool has_sign = false, has_digit = false, is_negative = false, dot_passed = false;
    for (int i = 0; str[i] != '\0'; ++i) {
        char sym = str[i];
        if (sym >= '0' && sym <= '9')
        {
            has_sign = true;
            has_digit = true;
            if (!dot_passed)
                result_up = result_up * 10 + (sym - '0');
            else
            {
                result_down = result_down + (double)(sym - '0') / prec;
                prec *= 10;
            }
        } else if ((sym == '-' || sym == '+') && !has_sign && !has_digit)
        {
            is_negative = sym == '-';
        } else if (sym == '.' && !dot_passed)
        {
            dot_passed = true;
        }else
        {
            *check_state = ERROR;
            return 0;
        }
    }
    check_state = CORRECT;
    return is_negative ? (-1.0 * (result_up + result_down)): result_up + result_down;
}

void incorrect_argc(void (*documentation_fun_ptr)())
{
    printf("You have entered incorrect number of arguments. Here is documentation below:\n");
    documentation_fun_ptr();
}

void incorrect_value(char val[])
{
    printf("\"%s\" is not a number. You must enter an integer number\n\n", val);
}

bool cmp_d(double first, double second, double precision)
{
    return fabs(first - second) < precision;
}

void swap(double *a, int i, int j)
{
    double s = a[i];
    a[i] = a[j];
    a[j] = s;
}

void swap_int(int *l, int *r)
{
    int tmp = *l;
    *l = *r;
    *r = tmp;
}

bool next_perm(double *a, int n)
{
    int j = n - 2;
    while (j != -1 && a[j] >= a[j + 1]) j--;
    if (j == -1)
        return false;
    int k = n - 1;
    while (a[j] >= a[k]) k--;
    swap(a, j, k);
    int l = j + 1, r = n - 1;
    while (l<r)
        swap(a, l++, r--);
    return true;
}

void sort_double_bubble(double *array, int size)
{
    for (int step = 0; step < size - 1; ++step) {

        for (int i = 0; i < size - step - 1; ++i) {

            if (array[i] > array[i + 1]) {

                int temp = array[i];
                array[i] = array[i + 1];
                array[i + 1] = temp;
            }
        }
    }
}

ll integer_from_n_radix(char *string, int radix, enum error_type* error_return, int (*char_to_int)(char), bool (*is_char_correct)(char, int))
{
    ll result = 0, multiplier = 1;
    bool has_sign_entered = false, is_negative = false;
    int length = strnlen_s(string, 64);

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

ld power_ld_int(ld base, unsigned int pow)
{
    ld result = 1, last_multiplier = base;

    for (int i = 0; i < sizeof(unsigned int) * 8; ++i)
    {
        if (pow & 1u << i)
        {

            result *= last_multiplier;
        }
        last_multiplier *= last_multiplier;
    }
    return result;
}

ll round_ld(ld number)
{
    if (((ll)(number * 10)) % 10 >= 5)
        return number + 1;
    else
        return number;
}

ld clamp_low(ld number, ld border)
{
    return number < border ? border : number;
}

void integer_to_n_radix(ll number, int radix, char* string)
{
	int length = logl(number) / logl(radix);
	int counter = 1;
	++length;

	if (number < 0)
	{
		string[0] = '-';
		++length;
	}
	string[length] = '\0';
	number = labs(number);

	while(number)
	{
		int tmp = number % radix;
		number /= radix;
		string[length - counter] = int_to_char(tmp);
		counter++;
	}
}

char int_to_char(int num)
{
	if (num <= 9)
		return num + '0';
	else
		return num - 10 + 'A';
}

double ld_from_str(char str[], enum error_type* check_state)
{
    int result_up = 0;
    ld result_down = 0, prec = 10;
    bool has_sign = false, has_digit = false, is_negative = false, dot_passed = false;
    for (int i = 0; str[i] != '\0'; ++i) {
        char sym = str[i];
        if (sym >= '0' && sym <= '9')
        {
            has_sign = true;
            has_digit = true;
            if (!dot_passed)
                result_up = result_up * 10 + (sym - '0');
            else
            {
                result_down = result_down + (double)(sym - '0') / prec;
                prec *= 10;
            }
        } else if ((sym == '-' || sym == '+') && !has_sign && !has_digit)
        {
            is_negative = sym == '-';
        } else if (sym == '.' && !dot_passed)
        {
            dot_passed = true;
        }else
        {
            *check_state = ERROR;
            return 0;
        }
    }
    check_state = CORRECT;
    return is_negative ? (-1.0 * (result_up + result_down)): result_up + result_down;
}

ld clamp(ld number, ld low_border, ld up_border)
{
    return number < low_border ? low_border : number > up_border ? up_border : number;
}