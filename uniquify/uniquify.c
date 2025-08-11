#include "libmin.h"

/* Comparison function for strings.
 * qsort passes pointers to elements (here pointers to char pointers),
 * so we cast accordingly.
 */
int string_compare(const void *a, const void *b) {
    const char * const *str1 = (const char * const *) a;
    const char * const *str2 = (const char * const *) b;
    return libmin_strcmp(*str1, *str2);
}

int main(void) {
    /* 
     * Define a statically allocated list of strings.
     * Most strings are unique, with a few duplicates.
     */
    char *strings[] = {
        "banana",
        "apple",
        "orange",
        "kiwi",
        "grape",
        "mango",
        "strawberry",
        "pear",
        "pineapple",
        "watermelon",
        "blueberry",
        "raspberry",
        "apple",   /* duplicate */
        "blackberry",
        "cherry",
        "banana",  /* duplicate */
        "peach",
        "apricot",
        "plum",
        "nectarine",
        "cantaloupe",
        "honeydew",
        "lime", /* duplicate */
        "papaya",
        "passionfruit",
        "guava",
        "lychee",
        "pomegranate",
        "grapefruit",
        "lemon",
        "lime",
        "tangerine",
        "cranberry",
        "fig",
        "plum" /* duplicate */
    };
    size_t n = sizeof(strings) / sizeof(strings[0]);

    libmin_printf("Unsorted Strings:\n");
    for (size_t i = 0; i < n; i++) {
        libmin_printf("%s\n", strings[i]);
    }
    libmin_printf("\n");

    libtarg_start_perf();
    /* Sort the list using qsort from qsort.h/qsort.c */
    libmin_qsort(strings, n, sizeof(char *), string_compare);

    /* Count unique strings */
    size_t unique_count = 0;
    if (n > 0) {
        unique_count = 1;
        for (size_t i = 1; i < n; i++) {
            if (libmin_strcmp(strings[i], strings[i - 1]) != 0) {
                unique_count++;
            }
        }
    }
    libtarg_stop_perf();

    libmin_printf("Sorted Strings:\n");
    for (size_t i = 0; i < n; i++) {
        libmin_printf("%s\n", strings[i]);
    }
    libmin_printf("\n");

    /* Remove duplicates from the sorted list.
     * Since duplicates become adjacent after sorting, we simply print the
     * first occurrence and then any string that differs from its predecessor.
     */
    libmin_printf("Unique Strings:\n");
    if (n > 0) {
        libmin_printf("%s\n", strings[0]);
        for (size_t i = 1; i < n; i++) {
            if (libmin_strcmp(strings[i], strings[i - 1]) != 0) {
                libmin_printf("%s\n", strings[i]);
            }
        }
        libmin_printf("\nTotal Unique Strings: %u\n", unique_count);
    } else {
        libmin_printf("No strings.\n");
    }

    libmin_success();
    return 0;
}

