#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define CURRENCY_STR_LENGTH 4
#define MAX_TABLE_COUNT 81

typedef uint8_t u8;

u8 hash_str(char* str) {
    int p = MAX_TABLE_COUNT;
    int offset = 65;
    u8 r = 0;

    for (size_t i = 0; i < strlen(str); i++) {
        char c = str[i];
        int digit = (c - 'a' + 1) - offset;
        r = (r + digit * 33 * i) % p;
    }

    return r;
}

void upper(char* str) {
    while (*str) {
        *str = toupper((unsigned char) *str);
        str++;
    }
}

enum Currency { USD=0, EUR=1, YEN=2, QTZ=3, NONE=4 };

int main() {
    enum Currency TABLE[MAX_TABLE_COUNT];
    for (size_t i = 0; i < MAX_TABLE_COUNT; i++) {
        TABLE[i] = NONE;
    }

    TABLE[hash_str("USD")] = USD;
    TABLE[hash_str("EUR")] = EUR;
    TABLE[hash_str("YEN")] = YEN;
    TABLE[hash_str("QTZ")] = QTZ;

    // 1USD -> USD, EUR, YEN, QTZ
    float USD_table[4] = {1.0f, 0.91f, 146.78f, 7.80f};
    // 1EUR-> USD, EUR, YEN, QTZ
    float EUR_table[4] = {1.09f, 1.0f, 160.62f, 8.54f};
    // 1YEN-> USD, EUR, YEN, QTZ
    float YEN_table[4] = {0.0068f, 0.0062f, 1.0f, 0.053f};
    // 1QTZ-> USD, EUR, YEN, QTZ
    float QTZ_table[4] = {0.13f, 0.12f, 18.83f, 1.0f};

    float* conversion_table[4];
    
    conversion_table[USD] = USD_table;
    conversion_table[EUR] = EUR_table;
    conversion_table[YEN] = YEN_table;
    conversion_table[QTZ] = QTZ_table; 

    float amount;
    printf("Amount to convert: ");
    scanf("%f", &amount);

    printf("Please enter the currency you want to convert from\n");
    printf("(possible values: USD, EUR, YEN, QTZ)\n");
    
    char currency[CURRENCY_STR_LENGTH];
    scanf("%s", currency);

    if (strlen(currency) > CURRENCY_STR_LENGTH) {
        exit(1);
    }
    
    upper(currency);
    printf("Selected currency: %s\n", currency);

    printf("Please enter the currency to convert\n");
    printf("(possible values: USD, EUR, YEN, QTZ)\n");

    char target_currency[CURRENCY_STR_LENGTH];
    scanf("%s", target_currency);

    if (strlen(target_currency) > CURRENCY_STR_LENGTH) {
        exit(1);
    }

    upper(target_currency);
    printf("Target currency: %s\n", target_currency);

    enum Currency source = TABLE[hash_str(currency)];
    enum Currency target = TABLE[hash_str(target_currency)];

    if (source == NONE || target == NONE) {
        printf("Invalid currency detected!\n");
        exit(1);
    }

    float factor = conversion_table[(int)source][(int)target];
    float result = amount * factor;
    printf("Result: %f\n", result);
    return 0;
}

