#include "../functions.h"
#include <stdlib.h>

enum flag_type
{ L_F, R_F, U_F, N_F, C_F, UNDEFINED };

enum flag_type get_flag_type(char* str);
size_t strlen_m(char* str);
unsigned int rand_uint_in_range(unsigned int a, unsigned int b);
char* reverse_string(char* str);
char* u_flag(char* str);
char* n_flag(char* str);
char* c_flag(char** strings, size_t size);
char to_upper_m(char sym);
void docs();

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        incorrect_argc(&docs);
        return 0;
    }

    switch (get_flag_type(argv[1]))
    {
        case L_F:
        {
            printf("String length is: %zu", strlen_m(argv[2]));
            break;
        }
        case R_F:
        {
            char* res = reverse_string(argv[2]);

            if(res == NULL)
            {
                printf("Bad alloc\n");
            } else
            {
                printf("%s\n", res);
                free(res);
            }
            break;
        }
        case U_F:
        {
            char* res = u_flag(argv[2]);

            if(res == NULL)
            {
                printf("Bad alloc\n");
            } else
            {
                printf("%s\n", res);
                free(res);
            }
            break;
        }
        case N_F:
        {
            char* res = n_flag(argv[2]);

            if(res == NULL)
            {
                printf("Bad alloc\n");
            } else
            {
                printf("%s\n", res);
                free(res);
            }
            break;
        }
        case C_F:
        {
            if (argc < 4)
            {
                incorrect_argc(&docs);
                return 0;
            }

            enum error_type ret;

            unsigned int seed = uint_from_str(argv[3], &ret);

            if (ret == ERROR)
            {
                printf("'%s' is not unsigned number\n", argv[3]);
                return 0;
            }

            srand(seed);

            char** strings= malloc((argc - 3) * sizeof(char*));

            if (strings == NULL)
            {
                printf("Bad alloc\n");
                return 0;
            }

            strings[0] = argv[2];

            for (int i = 1; i < argc - 3; ++i)
            {
                strings[i] = argv[i + 3];
            }

            char* res = c_flag(strings, argc - 3);

            if(res == NULL)
            {
                printf("Bad alloc\n");
            } else
            {
                printf("%s\n", res);
                free(res);
            }
            break;
        }
        case UNDEFINED:
        {
            printf("\"%s\" is not a flag\n", argv[1]);
            break;
        }
    }
}

enum flag_type get_flag_type(char* str)
{
    if (strlen_m(str) != 2 || str[0] != '-')
        return UNDEFINED;

    switch (str[1])
    {
        case 'l':
        {
            return L_F;
            break;
        }
        case 'r':
        {
            return R_F;
            break;
        }
        case 'u':
        {
            return U_F;
            break;
        }
        case 'n':
        {
            return N_F;
            break;
        }
        case 'c':
        {
            return C_F;
            break;
        }
        default:
        {
            return UNDEFINED;
            break;
        }
    }
}

size_t strlen_m(char* str)
{
    size_t len = 0;

    while (str[len] != '\0')
    {
        ++len;
    }

    return len;
}

unsigned int rand_uint_in_range(unsigned int a, unsigned int b)
{
    return (rand() % (b - a)) - a;
}

char* reverse_string(char* str)
{
    size_t size = strlen_m(str);
    char* result = malloc((size + 1) * sizeof(char));

    if (result == NULL)
        return NULL;

    for (int i = 0; i < size; ++i)
    {
        result[i] = str[size - i - 1];
    }
    result[size] = '\0';

    return result;
}

char* u_flag(char* str)
{
    size_t size = strlen_m(str);
    char* result = malloc((size + 1) * sizeof(char));

    if (result == NULL)
        return NULL;

    for (int i = 0; i < size; ++i)
    {
        if (i % 2 == 0)
        {
            result[i] = to_upper_m(str[i]);
        } else
        {
            result[i] = str[i];
        }
    }

    result[size] = '\0';

    return result;
}

char* c_flag(char** strings, size_t size)
{
    size_t length = 0;

    for (int i = 0; i < size; ++i)
    {
        length += strlen_m(strings[i]);
    }

    char* result = malloc((length + 1) * sizeof(char));

    if (result == NULL)
        return result;

    for (int i = 1; i < size; ++i)
    {
        unsigned int ind = rand_uint_in_range(0, i);
        char* tmp = strings[i];
        strings[i] = strings[ind];
        strings[ind] = tmp;
    }

    size_t counter = 0;

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; strings[i][j] != '\0'; ++j)
        {
            result[counter] = strings[i][j];
            ++counter;
        }
    }

    result[length] = '\0';
    return result;
}

char* n_flag(char* str)
{
    size_t size = strlen_m(str), counter = 0;
    char* result = malloc((size + 1) * sizeof(char));

    if (result == NULL)
        return NULL;

    for (int i = 0; i < size; ++i)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            result[counter] = str[i];
            ++counter;
        }
    }
    for (int i = 0; i < size; ++i)
    {
        if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z'))
        {
            result[counter] = str[i];
            ++counter;
        }
    }
    for (int i = 0; i < size; ++i)
    {
        if (!((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')))
        {
            result[counter] = str[i];
            ++counter;
        }
    }

    result[size] = '\0';
    return result;
}

char to_upper_m(char sym)
{
    if (sym >= 'a' && sym <= 'z')
        return sym - 'a' + 'A';
    else
        return sym;
}

void docs()
{
    printf("You must enter one of following flags and string:\n"
           "\t-l - string length\n"
           "\t-r - reverse string\n"
           "\t-u - to upper odd numbers\n"
           "\t-n - string in order: digits, alphas, other symbols\n"
           "\t-c - (uint, strings...) concatenate strings in random order\n\n");
}