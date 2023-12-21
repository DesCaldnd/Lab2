//
// Created by Des Caldnd on 10/24/2023.
//
#include <stdlib.h>
#include <stdarg.h>
#include "../functions.h"


struct substr
{
    char* filename;
    ll string_index, symbol_index;
};

struct substr_array
{
    struct substr* arr;
    size_t len, capacity;
};

void arr_push_back(struct substr_array *arr, struct substr value);
struct substr_array arr_init();
void arr_realloc(struct substr_array *arr);
void arr_free(struct substr_array *arr);
struct substr arr_get(struct substr_array *arr, size_t index);

struct substr_array get_all_ascensions(char* substr, size_t file_count, ...);
void process_one_file(FILE* in, char* substr, size_t str_len, struct substr_array* arr, char* filepath);


int main()
{
    struct substr_array arr = get_all_ascensions("  ", 1, "../T3/1.txt");

    if (arr.arr != NULL)
    {
        for (int i = 0; i < arr.len; ++i)
        {
            printf("%s %lld %lld\n", arr_get(&arr, i).filename, arr_get(&arr, i).string_index,
                   arr_get(&arr, i).symbol_index);
        }
        arr_free(&arr);
    } else
        printf("An error occurred. May be there is no memory or substring is empty");
}



struct substr_array get_all_ascensions(char* substr, size_t file_count, ...)
{
    va_list filepaths;
    va_start(filepaths, file_count);
    size_t str_len = strlen(substr);
    struct substr_array res;
    res.arr = NULL;
    if (str_len == 0)
        return res;
    res = arr_init();


    if (res.arr == NULL)
        return res;


    for (int i = 0; i < file_count; ++i)
    {
        char * filepath = va_arg(filepaths, char*);
        FILE* in = fopen(filepath, "rb+");
        if (in != NULL)
        {
            process_one_file(in, substr, str_len, &res, filepath);
            fclose(in);

            if (res.arr == NULL)
                return res;
        }
    }

    va_end(filepaths);
    return res;
}

void process_one_file(FILE* in, char* substr, size_t str_len, struct substr_array* arr, char* filepath)
{
    ll str_n = 1, sym_n = 1, index = 0;
    char sym;

    while((fread(&sym, sizeof(char), 1, in)) != 0)
    {
        if (sym == substr[index])
        {
            ++index;
            if (index == str_len)
            {
                struct substr res;
                res.filename = filepath;
                res.string_index = str_n;
                res.symbol_index = sym_n;

                arr_push_back(arr, res);
                if (arr->arr == NULL)
                    return;

                if (substr[0] == '\n')
                {
                    ++str_n;
                    sym_n = 1;
                } else
                {
                    ++sym_n;
                }

                fseek(in, -1 * (index - 1), SEEK_CUR);
                index = 0;
            }
        } else
        {
            if (index != 0)
            {
                if (substr[0] == '\n')
                {
                    ++str_n;
                    sym_n = 1;
                } else
                {
                    ++sym_n;
                }

                fseek(in, -1 * index, SEEK_CUR);
                index = 0;
            } else
            {
                if (sym == '\n')
                {
                    ++str_n;
                    sym_n = 1;
                } else
                {
                    ++sym_n;
                }
            }
        }
    }

}

void arr_push_back(struct substr_array *arr, struct substr value)
{
    if (arr->len >= arr->capacity)
    {
        arr_realloc(arr);
        if (arr->arr == NULL)
            return;
    }

    arr->arr[arr->len] = value;
    ++arr->len;
}

struct substr_array arr_init()
{
    struct substr_array result;

    result.arr = malloc(10 * sizeof(struct substr));
    result.len = 0;
    result.capacity = 10;

    return result;
}

void arr_realloc(struct substr_array *arr)
{
    arr->arr = realloc(arr->arr, arr->capacity * 2);
    arr->capacity *= 2;
}

void arr_free(struct substr_array *arr)
{
    free(arr->arr);
}

struct substr arr_get(struct substr_array *arr, size_t index)
{
    return arr->arr[index];
}