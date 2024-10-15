/**
 * @file
 * @author [serturx](https://github.com/serturx/)
 * @brief Encode a null terminated string using [Run-length encoding](https://en.wikipedia.org/wiki/Run-length_encoding)
 * @details
 * Run-length encoding is a lossless compression algorithm.
 * It works by counting the consecutive occurences symbols
 * and encodes that series of consecutive symbols into the
 * counted symbol and a number denoting the number of
 * consecutive occorences.
 * 
 * For example the string "AAAABBCCD" gets encoded into "4A2B2C1D"
 * 
 */

#include "libmin.h"

/**
 * @brief Encodes a null-terminated string using run-length encoding
 * @param str String to encode
 * @return char* Encoded string
 */

char* run_length_encode(char* str) {
    int str_length = libmin_strlen(str);
    int encoded_index = 0;

    //allocate space for worst-case scenario
    char* encoded = libmin_malloc(2 * libmin_strlen(str) + 1);
    
    //temp space for int to str conversion
    char int_str[20];

    for(int i = 0; i < str_length; ++i) {
        int count = 0;
        char current = str[i];

        //count occurences
        while(current == str[i + count]) count++;

        i += count - 1;

        //convert occurrence amount to string and write to encoded string
        libmin_snprintf(int_str, 20, "%d", count);
        libmin_strncpy(&encoded[encoded_index], int_str, libmin_strlen(int_str));

        //write current char to encoded string
        encoded_index += libmin_strlen(int_str);
        encoded[encoded_index] = current;
        ++encoded_index;
    }

    //null terminate string and move encoded string to compacted memory space 
    encoded[encoded_index] = '\0';
    char* compacted_string = libmin_malloc(libmin_strlen(encoded) + 1);
    libmin_strcpy(compacted_string, encoded);
    
    libmin_free(encoded);

    return compacted_string;
}

/**
 * @brief Self-test implementations
 * @returns void
 */
static void test() {
    char *in, *out;
    in = "aaaaaaabbbaaccccdefaadr";
    out = run_length_encode(in);
    libmin_assert(!libmin_strcmp(out, "7a3b2a4c1d1e1f2a1d1r"));
    libmin_printf("in: %s -> out: %s\n", in, out);
    libmin_free(out);
 
    in = "lidjhvipdurevbeirbgipeahapoeuhwaipefupwieofb";
    out = run_length_encode(in);
    libmin_assert(!libmin_strcmp(out, "1l1i1d1j1h1v1i1p1d1u1r1e1v1b1e1i1r1b1g1i1p1e1a1h1a1p1o1e1u1h1w1a1i1p1e1f1u1p1w1i1e1o1f1b"));
    libmin_printf("in: %s -> out: %s\n", in, out);
    libmin_free(out);

    in = "htuuuurwuquququuuaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaahghghrw";
    out = run_length_encode(in);
    libmin_assert(!libmin_strcmp(out, "1h1t4u1r1w1u1q1u1q1u1q3u76a1h1g1h1g1h1r1w"));
    libmin_printf("in: %s -> out: %s\n", in, out);
    libmin_free(out);
}

/**
 * @brief Main function
 * @returns 0 on exit
 */
int main() {
    test();  // run self-test implementations

    libmin_printf("All tests have passed!\n");
    libmin_success();
    return 0;
}

