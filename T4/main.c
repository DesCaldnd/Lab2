//
// Created by Des Caldnd on 10/22/2023.
//
#include "../functions.h"
#include <stdarg.h>

struct point_2d
{
    ld x, y;
};

ld calculate_polynom(ld x, int degree, ...);
bool check_polygon(int point_count, ...);
bool check_two_sides(int* direction, struct point_2d sides[]);


int main()
{
    printf("%LF", calculate_polynom(5L, 4, (ld)3, (ld)-2, (ld)1, (ld)5.45, (ld)7));
}

bool check_polygon(int point_count, ...)
{
    va_list points;
    va_start(points, point_count);

    struct point_2d first = va_arg(points, struct point_2d), second = va_arg(points, struct point_2d);
    int direction = 0;

    struct point_2d sides[3];
    sides[0] = first;
    sides[1] = second;

    for (int i = 2; i < point_count; ++i)
    {
        sides[2] = va_arg(points, struct point_2d);

        if (!check_two_sides(&direction, sides))
            return false;

        sides[0] = sides[1];
        sides[1] = sides[2];
    }

    sides[2] = first;

    if (!check_two_sides(&direction, sides))
        return false;

    sides[0] = sides[1];
    sides[1] = first;
    sides[2] = second;

    if (!check_two_sides(&direction, sides))
        return false;

    return true;
}

bool check_two_sides(int* direction, struct point_2d sides[])
{
    struct point_2d ab = {sides[1].x - sides[0].x, sides[1].y - sides[0].y};
    struct point_2d bc = {sides[2].x - sides[1].x, sides[2].y - sides[1].y};

    ld prod = ab.x * bc.y - ab.y * bc.x;

    if (*direction == 0)
    {
        *direction = prod > 0 ? 1 : -1;
    } else if (*direction != (prod > 0 ? 1 : -1))
    {
        return false;
    }
    return true;
}

ld calculate_polynom(ld x, int degree, ...)
{
    va_list coeffs;
    va_start(coeffs, degree);


    ld accum = 0;

    for (int i = degree; i >= 0; --i)
    {
        accum += accum * x + va_arg(coeffs, ld);
    }
    return accum;
}//1684.25