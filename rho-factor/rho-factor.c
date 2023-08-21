#include "libmin.h"

typedef unsigned long long int positive_number;

static positive_number multiplication_modulo(positive_number a, positive_number b, const positive_number mod) {
    positive_number res = 0, tmp;
    for (b %= mod; a; a & 1 ? b >= mod - res ? res -= mod : 0, res += b : 0, a >>= 1, (tmp = b) >= mod - b ? tmp -= mod : 0, b += tmp);
    return res % mod;
}

static int is_prime(positive_number n, int k) {
    positive_number a = 0, b, c, d, e, f, g; int h, i;
    if ((n == 1) == (n & 1)) return n == 2;
    if (n < 4669921) // fast constexpr for small primes (this line is removable).
        return ((n & 1) & ((n < 6) * 42 + 0x208A2882) >> n % 30 && (n < 49 || (n % 7 && n % 11 && n % 13 && n % 17 && n % 19 && n % 23 && n % 29 && (n < 961 || (n % 31 && n % 37 && n % 41 && n % 43 && n % 47 && n % 53 && n % 59 && n % 61 && n % 67 && (n < 5041 || (n % 71 && n % 73 && n % 79 && n % 83 && n % 89 && n % 97 && n % 101 && n % 103 && n % 107 && (n < 11881 || (n % 109 && n % 113 && n % 127 && n % 131 && n % 137 && n % 139 && n % 149 && n % 151 && n % 157 && (n < 26569 || (n % 163 && n % 167 && n % 173 && n % 179 && n % 181 && n % 191 && n % 193 && n % 197 && n % 199 && (n < 44521 || (n % 211 && n % 223 && n % 227 && n % 229 && n % 233 && n % 239 && n % 241 && n % 251 && n % 257 && (n < 69169 || (n % 263 && n % 269 && n % 271 && n % 277 && n % 281 && n % 283 && n % 293 && n % 307 && n % 311 && (n < 97969 || (n % 313 && n % 317 && n % 331 && n % 337 && n % 347 && n % 349 && n % 353 && n % 359 && n % 367 && (n < 139129 || (n % 373 && n % 379 && n % 383 && n % 389 && n % 397 && n % 401 && n % 409 && n % 419 && n % 421 && (n < 185761 || (n % 431 && n % 433 && n % 439 && n % 443 && n % 449 && n % 457 && n % 461 && n % 463 && n % 467 && (n < 229441 || (n % 479 && n % 487 && n % 491 && n % 499 && n % 503 && n % 509 && n % 521 && n % 523 && n % 541 && (n < 299209 || (n % 547 && n % 557 && n % 563 && n % 569 && n % 571 && n % 577 && n % 587 && n % 593 && n % 599 && (n < 361201 || (n % 601 && n % 607 && n % 613 && n % 617 && n % 619 && n % 631 && n % 641 && n % 643 && n % 647 && (n < 426409 || (n % 653 && n % 659 && n % 661 && n % 673 && n % 677 && n % 683 && n % 691 && n % 701 && n % 709 && (n < 516961 || (n % 719 && n % 727 && n % 733 && n % 739 && n % 743 && n % 751 && n % 757 && n % 761 && n % 769 && (n < 597529 || (n % 773 && n % 787 && n % 797 && n % 809 && n % 811 && n % 821 && n % 823 && n % 827 && n % 829 && (n < 703921 || (n % 839 && n % 853 && n % 857 && n % 859 && n % 863 && n % 877 && n % 881 && n % 883 && n % 887 && (n < 822649 || (n % 907 && n % 911 && n % 919 && n % 929 && n % 937 && n % 941 && n % 947 && n % 953 && n % 967 && (n < 942841 || (n % 971 && n % 977 && n % 983 && n % 991 && n % 997 && n % 1009 && n % 1013 && n % 1019 && n % 1021 && (n < 1062961 || (n % 1031 && n % 1033 && n % 1039 && n % 1049 && n % 1051 && n % 1061 && n % 1063 && n % 1069 && n % 1087 && (n < 1190281 || (n % 1091 && n % 1093 && n % 1097 && n % 1103 && n % 1109 && n % 1117 && n % 1123 && n % 1129 && n % 1151 && (n < 1329409 || (n % 1153 && n % 1163 && n % 1171 && n % 1181 && n % 1187 && n % 1193 && n % 1201 && n % 1213 && n % 1217 && (n < 1495729 || (n % 1223 && n % 1229 && n % 1231 && n % 1237 && n % 1249 && n % 1259 && n % 1277 && n % 1279 && n % 1283 && (n < 1661521 || (n % 1289 && n % 1291 && n % 1297 && n % 1301 && n % 1303 && n % 1307 && n % 1319 && n % 1321 && n % 1327 && (n < 1852321 || (n % 1361 && n % 1367 && n % 1373 && n % 1381 && n % 1399 && n % 1409 && n % 1423 && n % 1427 && n % 1429 && (n < 2053489 || (n % 1433 && n % 1439 && n % 1447 && n % 1451 && n % 1453 && n % 1459 && n % 1471 && n % 1481 && n % 1483 && (n < 2211169 || (n % 1487 && n % 1489 && n % 1493 && n % 1499 && n % 1511 && n % 1523 && n % 1531 && n % 1543 && n % 1549 && (n < 2411809 || (n % 1553 && n % 1559 && n % 1567 && n % 1571 && n % 1579 && n % 1583 && n % 1597 && n % 1601 && n % 1607 && (n < 2588881 || (n % 1609 && n % 1613 && n % 1619 && n % 1621 && n % 1627 && n % 1637 && n % 1657 && n % 1663 && n % 1667 && (n < 2785561 || (n % 1669 && n % 1693 && n % 1697 && n % 1699 && n % 1709 && n % 1721 && n % 1723 && n % 1733 && n % 1741 && (n < 3052009 || (n % 1747 && n % 1753 && n % 1759 && n % 1777 && n % 1783 && n % 1787 && n % 1789 && n % 1801 && n % 1811 && (n < 3323329 || (n % 1823 && n % 1831 && n % 1847 && n % 1861 && n % 1867 && n % 1871 && n % 1873 && n % 1877 && n % 1879 && (n < 3568321 || (n % 1889 && n % 1901 && n % 1907 && n % 1913 && n % 1931 && n % 1933 && n % 1949 && n % 1951 && n % 1973 && (n < 3916441 || (n % 1979 && n % 1987 && n % 1993 && n % 1997 && n % 1999 && n % 2003 && n % 2011 && n % 2017 && n % 2027 && (n < 4116841 || (n % 2029 && n % 2039 && n % 2053 && n % 2063 && n % 2069 && n % 2081 && n % 2083 && n % 2087 && n % 2089 && (n < 4405801 || (n % 2099 && n % 2111 && n % 2113 && n % 2129 && n % 2131 && n % 2137 && n % 2141 && n % 2143 && n % 2153 )))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))));
    for (b = c = n - 1, h = 0; !(b & 1); b >>= 1, ++h);
    for (; k--;) {
        for (g = 0; g < sizeof(positive_number); ((char*)&a)[g++] = libmin_rand()); // random number.
        do for (d = e = 1 + a % c, f = n; (d %= f) && (f %= d););
        while (d > 1 && f > 1);
        for (d = f = 1; f <= b; f <<= 1);
        for (; f >>= 1; d = multiplication_modulo(d, d, n), f & b && (d = multiplication_modulo(e, d, n)));
        if (d == 1) continue;
        for (i = h; i-- && d != c; d = multiplication_modulo(d, d, n));
        if (d != c) return 0;
    }
    return 1;
}

positive_number factor_worker(const positive_number n) {
    size_t a = -1, b = 2 ;
    positive_number c, d = 1 + libmin_rand(), e, f;
    c = d %= n;
    do {
        if (++a == b){
            // handle your timeout here, when (a == 1 << 20) or another power of two.
            d = c, b <<= 1, a = 0;
        }
        c = multiplication_modulo(c, c, n);
        for (++c, c *= c != n, e = n, f = c > d ? c - d : d - c; (f %= e) && (e %= f););
    } while ((f |= e) == 1);
    return f;
}

// fill the given array with prime factors of n, result will be zero terminated.
positive_number * factor(positive_number n, positive_number *array) {
    positive_number a, b; size_t s ;
    do  if (n < 4)
            *array++ = n, n = 1;
        else if (n & 1) {
            for (b = n >> 1, a = (b + n / b) >> 1; a < b; b = a, a = (b + n / b) >> 1);
            if (b * b == n) {
                s = factor(b, array) - array ;
                libmin_memcpy(array + s, array, s * sizeof(positive_number)) ;
                array += s + s ;
                n = 1;
            } else {
                if (is_prime(n, 24)) // number of Miller-Rabin iterations.
                    *array++ = n, n = 1;
                else {
                    a = factor_worker(n); // factor_worker can't be called with a prime.
                    array = factor(a, array);
                    n /= a;
                }
            }
        } else
            for (; !(n & 1); *array++ = 2, n >>= 1);
    while (n > 1);
    *array = 0 ;
    return array ;
}

int main(void)
{
    // allocate memory for 64 factors.
    positive_number *factors = libmin_calloc(64, sizeof(positive_number)), n = 0, mask = -1;
    /* TMA: unsigned sr = (size_t)factors; sr = -sr / 561; libmin_printf("srand at %u :\n\n", sr); */ libmin_srand(/* TMA: sr */42);
    for (int wrapper = 4, count = 0, j; wrapper < 64; ++wrapper)
        for (int n_bits = wrapper; n_bits <= 64; ++n_bits) {
            // generate a random number of ~ n_bits bits.
            for (size_t k = 0; k < sizeof(positive_number); ((char *) &n)[k++] = libmin_rand());
            n &= mask >> (8 * sizeof(positive_number) - n_bits); n += !(n & 1);
            libmin_printf("%5d. (%2d bits) %22llu = ", ++count, n_bits, n);
            // fill the "factors" array with the prime factors.
            factor(n, factors);
            // iterate over the factors (zero terminated array).
            for (j = 0; factors[j + 1]; ++j) {
                libmin_printf("%llu * ", factors[j]);
                libmin_assert(n % factors[j] == 0);
                libmin_assert(is_prime(factors[j], 36));
            }
            libmin_printf("%llu\n", factors[j]);
        }

    // release memory.
    libmin_free(factors);

    // proper exit
    libmin_success();
}

