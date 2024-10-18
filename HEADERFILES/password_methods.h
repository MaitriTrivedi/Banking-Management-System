#ifndef PASSWORD_METHODS_H
#define PASSWORD_METHODS_H

#include<openssl/sha.h>
#include<openssl/evp.h>
#include<string.h>

// void hashPassword(const char* password, unsigned char* hashed_password) {
//     SHA256_CTX sha256;
//     SHA256_Init(&sha256);
//     SHA256_Update(&sha256, password, strlen(password));
//     SHA256_Final(hashed_password, &sha256);
// }

void hashPassword(const char* password, unsigned char* hashed_password) {
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(ctx, password, strlen(password));
    unsigned int len = SHA256_DIGEST_LENGTH;
    EVP_DigestFinal_ex(ctx, hashed_password, &len);
    EVP_MD_CTX_free(ctx);
}

void hashedPasswordToHex(const unsigned char* hashed_password, char* hex_output, size_t length) {
    for (size_t i = 0; i < length; i++) {
        sprintf(hex_output + (i * 2), "%02x", hashed_password[i]);
    }
    hex_output[length * 2] = '\0';  // Null-terminate the string
}
#endif