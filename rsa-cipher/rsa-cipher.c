#include "libmin.h"

// Define 128-bit integer type (GCC/Clang-specific)
typedef __int128 int128;

// Fast modular exponentiation: computes (base^exp) mod mod
int128 mod_pow(int128 base, int128 exp, int128 mod) {
    int128 result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// Extended Euclidean Algorithm: returns d such that (a*d) % mod == 1
int128 mod_inverse(int128 a, int128 mod) {
    int128 m0 = mod, t, q;
    int128 x0 = 0, x1 = 1;
    
    if (mod == 1)
        return 0;
    
    while (a > 1) {
        q = a / mod;
        t = mod;
        mod = a % mod;
        a = t;
        
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    
    if (x1 < 0)
        x1 += m0;
    return x1;
}

// Helper function to print a 128-bit number in hexadecimal.
// It converts the number to a 32-digit hex string, then trims leading zeros.
void print_hex_int128(int128 n) {
    if (n == 0) {
        libmin_printf("0x0");
        return;
    }
    if (n < 0) {
        libmin_printf("-");
        n = -n;
    }
    
    // 128 bits = 32 hex digits; extra one for the null terminator
    char hex_digits[33];
    hex_digits[32] = '\0';
    
    // Fill the array from the least-significant nibble upward.
    for (int i = 31; i >= 0; i--) {
        hex_digits[i] = "0123456789abcdef"[n & 0xF];
        n >>= 4;
    }
    
    // Skip over any leading zeros for a cleaner output.
    int j = 0;
    while (hex_digits[j] == '0' && hex_digits[j+1] != '\0') {
        j++;
    }
    libmin_printf("0x%s", &hex_digits[j]);
}

int main() {
    // Fixed primes for demonstration (not cryptographically secure)
    unsigned long long p_ll = 1000003ULL;  // About 1e6
    unsigned long long q_ll = 1000033ULL;  // About 1e6
    int128 p = p_ll;
    int128 q = q_ll;
    
    // Compute modulus: n = p * q (using 128-bit arithmetic)
    int128 n = p * q;
    
    // Compute Euler's totient: phi = (p - 1) * (q - 1)
    int128 phi = (p - 1) * (q - 1);
    
    // Choose a common public exponent e = 65537
    int128 e = 65537;
    
    // Compute private exponent d as the modular inverse of e modulo phi
    int128 d = mod_inverse(e, phi);
    
    // Choose a sample message to encrypt (must be less than n)
    int128 message = 0xdeadbeef;
    
    // Encrypt: ciphertext = message^e mod n
    int128 encrypted = mod_pow(message, e, n);
    
    // Decrypt: decrypted = ciphertext^d mod n
    int128 decrypted = mod_pow(encrypted, d, n);
    
    // Print all outputs in hexadecimal
    libmin_printf("RSA Key Pair Example (Hexadecimal Output):\n\n");
    
    libmin_printf("p: ");
    print_hex_int128(p);
    libmin_printf("\nq: ");
    print_hex_int128(q);
    
    libmin_printf("\nn (modulus): ");
    print_hex_int128(n);
    
    libmin_printf("\nphi(n): ");
    print_hex_int128(phi);
    
    libmin_printf("\n\nPublic exponent (e): ");
    print_hex_int128(e);
    
    libmin_printf("\nPrivate exponent (d): ");
    print_hex_int128(d);
    
    libmin_printf("\n\nOriginal message: ");
    print_hex_int128(message);
    
    libmin_printf("\nEncrypted message: ");
    print_hex_int128(encrypted);
    
    libmin_printf("\nDecrypted message: ");
    print_hex_int128(decrypted);
    
    libmin_printf("\n");

    libmin_success();
    return 0;
}
