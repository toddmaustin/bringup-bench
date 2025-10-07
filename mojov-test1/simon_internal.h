#ifndef SIMON_INTERNAL_H
#define SIMON_INTERNAL_H


// cipher_constants.h

enum cipher_mode_t { ECB, CTR, CBC, CFB, OFB };

static const uint8_t block_sizes[] = {32, 48, 48, 64, 64, 96, 96, 128, 128, 128};

static const uint16_t key_sizes[] = {64, 72, 96, 96, 128, 96, 144, 128, 192, 256};

enum cipher_config_t {
    cfg_64_32,
    cfg_72_48,
    cfg_96_48,
    cfg_96_64,
    cfg_128_64,
    cfg_96_96,
    cfg_144_96,
    cfg_128_128,
    cfg_192_128,
    cfg_256_128
} ;

typedef struct {
    enum cipher_config_t cipher_cfg;
    void (*encryptPtr)(const uint8_t, const uint8_t *, const uint8_t *, uint8_t *);
    void (*decryptPtr)(const uint8_t, const uint8_t *, const uint8_t *, uint8_t *);
    uint16_t key_size;
    uint8_t block_size;
    uint8_t round_limit;
    uint8_t init_vector[16];
    uint8_t counter[16];
    uint8_t key_schedule[576];
    uint8_t alpha;
    uint8_t beta;
    uint8_t z_seq;
} SimSpk_Cipher;

typedef struct _bword_24{
  uint32_t data: 24;
} bword_24;

typedef struct _bword_48{
  uint64_t data: 48;
} bword_48;

uint8_t Simon_Init(SimSpk_Cipher *cipher_object, enum cipher_config_t cipher_cfg, enum cipher_mode_t c_mode, void *key, uint8_t *iv, uint8_t *counter);

uint8_t Simon_Encrypt(SimSpk_Cipher cipher_object, const void *plaintext, void *ciphertext);

void Simon_Encrypt_32(uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *plaintext,
                      uint8_t *ciphertext);
void Simon_Encrypt_48(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *plaintext,
                      uint8_t *ciphertext);
void Simon_Encrypt_64(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *plaintext,
                      uint8_t *ciphertext);
void Simon_Encrypt_96(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *plaintext,
                      uint8_t *ciphertext);
void Simon_Encrypt_128(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *plaintext,
                       uint8_t *ciphertext);

uint8_t Simon_Decrypt(SimSpk_Cipher cipher_object, const void *ciphertext, void *plaintext);
void Simon_Decrypt_32(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *ciphertext,
                      uint8_t *plaintext);
void Simon_Decrypt_48(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *ciphertext,
                      uint8_t *plaintext);
void Simon_Decrypt_64(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *ciphertext,
                      uint8_t *plaintext);
void Simon_Decrypt_96(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *ciphertext,
                      uint8_t *plaintext);
void Simon_Decrypt_128(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *ciphertext,
                       uint8_t *plaintext);

#endif /* SIMON_INTERNAL_H */

