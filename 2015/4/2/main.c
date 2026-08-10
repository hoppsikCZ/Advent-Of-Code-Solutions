#include <openssl/evp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// https://docs.openssl.org/master/man3/EVP_DigestInit/#examples
unsigned int calc_md5_hex(const char *key, unsigned char *md_value) {
    EVP_MD_CTX *mdctx;
    unsigned int md_len, i;

    mdctx = EVP_MD_CTX_new();
    if (mdctx == NULL) {
        printf("Message digest create failed.\n");
        exit(1);
    }
    if (!EVP_DigestInit_ex2(mdctx, EVP_md5(), NULL)) {
        printf("Message digest initialization failed.\n");
        EVP_MD_CTX_free(mdctx);
        exit(1);
    }
    if (!EVP_DigestUpdate(mdctx, key, strlen(key))) {
        printf("Message digest update failed.\n");
        EVP_MD_CTX_free(mdctx);
        exit(1);
    }
    if (!EVP_DigestFinal_ex(mdctx, md_value, &md_len)) {
        printf("Message digest finalization failed.\n");
        EVP_MD_CTX_free(mdctx);
        exit(1);
    }
    EVP_MD_CTX_free(mdctx);

    printf("Digest is: ");
    for (i = 0; i < md_len; i++) {
        printf("%02x", md_value[i]);
    }
    printf("\n");

    return md_len;
}

int main(int argc, char *argv[]) {
    unsigned char md5[EVP_MAX_MD_SIZE];
    unsigned int md5_len;
    const unsigned int to_match[] = {0x00, 0x00, 0x00};
    char num[30];
    const char *key = "bgvyzdsv";
    char key_num[40];

    for (size_t i = 0; i < SIZE_MAX; i++) {
        sprintf(num, "%lu", i);
        strcpy(key_num, key);
        strcat(key_num, num);
        md5_len = calc_md5_hex(key_num, md5);

        if (memcmp(to_match, md5, 3) == 0) {
            printf("%lu\n", i);
            break;
        }
    }
    // printf("Digest is: ");
    // for (int i = 0; i < md5_len; i++) {
    //     printf("%02x", md5[i]);
    // }
    // printf("\n");
    return EXIT_SUCCESS;
}
