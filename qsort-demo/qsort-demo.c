#include "libmin.h"

/* qsort int comparison function */
int
int_cmp(const void *a, const void *b)
{
    const int *ia = (const int *)a; // casting pointer types
    const int *ib = (const int *)b;
    return *ia  - *ib;
    /* integer comparison: returns negative if a < b
    and positive if a > b */
}

/* integer array printing function */
void
print_int_array(const int *array, size_t len)
{
    size_t i;

    for(i=0; i<len; i++)
        libmin_printf("%d | ", array[i]);

    libmin_putc('\n');
}

/* sorting integers using qsort() example */
void
sort_integers_example(void)
{
    int numbers[] = { 7, 3, 4, 1, -1, 23, 12, 43, 2, -4, 5 };
    size_t numbers_len = sizeof(numbers)/sizeof(int);

    libmin_puts("*** Integer sorting...");

    /* print original integer array */
    print_int_array(numbers, numbers_len);

    /* sort array using qsort functions */
    libmin_qsort(numbers, numbers_len, sizeof(int), int_cmp);

    /* print sorted integer array */
    print_int_array(numbers, numbers_len);
}

/* qsort C-string comparison function */
int
cstring_cmp(const void *a, const void *b)
{
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return libmin_strcmp(*ia, *ib);
    /* strcmp functions works exactly as expected from
    comparison function */
}

/* C-string array printing function */
void
print_cstring_array(char **array, size_t len)
{
    size_t i;

    for(i=0; i<len; i++)
        libmin_printf("%s | ", array[i]);

    libmin_putc('\n');
}

/* sorting C-strings array using qsort() example */
void
sort_cstrings_example(void)
{
    char *strings[] = { "Zorro", "Alex", "Celine", "Bill", "Forest", "Dexter" };
    size_t strings_len = sizeof(strings) / sizeof(char *);

    /* STRING */
    libmin_puts("*** String sorting...");

    /* print original string array */
    print_cstring_array(strings, strings_len);

    /* sort array using qsort functions */
    libmin_qsort(strings, strings_len, sizeof(char *), cstring_cmp);

    /* print sorted string array */
    print_cstring_array(strings, strings_len);
}

/* an example of struct */
struct st_ex {
    char product[16];
    float price;
};

/* qsort struct comparision function (price float field) */
int
struct_cmp_by_price(const void *a, const void *b)
{
    struct st_ex *ia = (struct st_ex *)a;
    struct st_ex *ib = (struct st_ex *)b;
    return (int)(100.f*ia->price - 100.f*ib->price);
    /* float comparison: returns negative if b > a
    and positive if a > b. We multiplied result by 100.0
    to preserve decimal fraction */
}

/* qsort struct comparision function (product C-string field) */
int
struct_cmp_by_product(const void *a, const void *b)
{
    struct st_ex *ia = (struct st_ex *)a;
    struct st_ex *ib = (struct st_ex *)b;
    return libmin_strcmp(ia->product, ib->product);
    /* strcmp functions works exactly as expected from
    comparison function */
}

/* Example struct array printing function */
void
print_struct_array(struct st_ex *array, size_t len)
{
    size_t i;

    for(i=0; i<len; i++)
        libmin_printf("[ product: %s \t price: $%.2f ]\n", array[i].product, array[i].price);

    libmin_puts("--");
}

/* sorting structs using qsort() example */
void
sort_structs_example(void)
{
    struct st_ex structs[] = {{"mp3 player", 299.0f}, {"plasma tv", 2200.0f},
                              {"notebook", 1300.0f}, {"smartphone", 499.99f},
                              {"dvd player", 150.0f}, {"matches", 0.2f }};

    size_t structs_len = sizeof(structs) / sizeof(struct st_ex);

    libmin_puts("*** Struct sorting (price)...");

    /* print original struct array */
    print_struct_array(structs, structs_len);

    /* sort array using qsort functions */
    libmin_qsort(structs, structs_len, sizeof(struct st_ex), struct_cmp_by_price);

    /* print sorted struct array */
    print_struct_array(structs, structs_len);

    libmin_puts("*** Struct sorting (product)...");

    /* resort using other comparision function */
    libmin_qsort(structs, structs_len, sizeof(struct st_ex), struct_cmp_by_product);

    /* print sorted struct array */
    print_struct_array(structs, structs_len);
}

/* MAIN program (calls all other examples) */
int
main(void)
{
    /* run all example functions */
    sort_integers_example();
    sort_cstrings_example();
    sort_structs_example();

    libmin_success();
    return 0;
}
