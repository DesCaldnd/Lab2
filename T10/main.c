//
// Created by Des Caldnd on 10/22/2023.
//

#include "../functions.h"
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>

enum error_type recalc_coef(double eps, double a, double** result, int degree, ...);
void recalc_coef_for_n_degree(double eps, double a, double src, double* result, int n);
double rand_d();
double calc_expression(int degree, double x, double * coeffs);

int main()
{
    srand(time(NULL));

    double* coeffs = malloc(6 * sizeof(double));

    if (coeffs == NULL)
        return 0;

    for (int i = 3; i >= 0; --i)
    {
        coeffs[i] = rand_d();
        printf("%lf ", coeffs[i]);
    }
    printf("\n");

    printf("%lf %lf %lf\n", calc_expression(3, -1, coeffs), calc_expression(3, 0, coeffs), calc_expression(3, 1, coeffs));

    double* new_coeffs;

    enum error_type ret = recalc_coef(0, 5, &new_coeffs, 3, coeffs[0], coeffs[1], coeffs[2], coeffs[3]);

    if (ret == ERROR)
        return 0;

    for (int i = 3; i >= 0; --i)
    {
        printf("%lf ", new_coeffs[i]);
    }
    printf("\n");

    printf("%lf %lf %lf\n", calc_expression(3, 4, new_coeffs), calc_expression(3, 5, new_coeffs), calc_expression(3, 6, new_coeffs));
}

enum error_type recalc_coef(double eps, double a, double** result, int degree, ...)
{
    va_list in_coef;
    va_start(in_coef, degree);

    *result = malloc((degree + 1) * sizeof(double));

    if (*result == NULL)
        return ERROR;

    double* src = malloc((degree + 1) * sizeof(double));

    if (src == NULL)
    {
        free(*result);
        return ERROR;
    }

    for (int i = 0; i <= degree; ++i)
    {
        src[i] = va_arg(in_coef, double);
        (*result)[i] = src[i];
    }
    va_end(in_coef);

    for(int i = degree; i >= 0; --i)
    {
        recalc_coef_for_n_degree(eps, a, src[i], *result, i);
    }
    return CORRECT;
}

void recalc_coef_for_n_degree(double eps, double a, double src, double* result, int n)
{
    ll c_n = n * -1;
    int k = 1;
    double multiplier = a;

    for (int i = n - 1; i >= 0; --i)
    {
        result[i] += multiplier * c_n * src;

        multiplier *= a;
        ++k;

        c_n = (c_n * -1 * (n - k + 1)) / k;
    }

}

double calc_expression(int degree, double x, double * coeffs)
{
    double accum = 0, x_n = 1;
    for (int i = 0; i <= degree; ++i)
    {
        accum += x_n * coeffs[i];
        x_n *= x;
    }

    return accum;
}

double rand_d()
{
    return ((double)(rand() % 20000 - 10000))/100;
}