#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGITS 310 

typedef struct {
    char digits[MAX_DIGITS]; 
    int length;
    int sign; 
} BigInt;

//  function to initialize a BigInt //
void initBigInt(BigInt *n, const char *value) {
    n->length = 0;
    n->sign = 1;

    if (value[0] == '-') {
        n->sign = -1;
        value++;
    }

    int len = strlen(value);
    if(len >MAX_DIGITS)
    {
        printf("number overflow");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < len; i++) {
        n->digits[i] = value[len - i - 1] - '0';
        n->length++;
    }
}

//  function to print a BigInt //
void printBigInt(const BigInt *n) {
    if (n->sign == -1) printf("-");
    for (int i = n->length - 1; i >= 0; i--) {
        printf("%d", n->digits[i]);
    }
    printf("\n");
}

//  function to compare two BigInts //
int compareBigIntAbs(const BigInt *a, const BigInt *b) {
    if (a->length != b->length) {
        return (a->length > b->length) ? 1 : -1;
    }
    for (int i = a->length - 1; i >= 0; i--) {
        if (a->digits[i] != b->digits[i]) {
            return (a->digits[i] > b->digits[i]) ? 1 : -1;
        }
    }
    return 0;
}

// Addition of two BigInts//
void addBigInt(const BigInt *a, const BigInt *b, BigInt *result) {
    if (a->sign == b->sign) {
        result->length = 0;
        result->sign = a->sign;

        int carry = 0;
        for (int i = 0; i < a->length || i < b->length || carry; i++) {
             if (result->length >= MAX_DIGITS) {
                printf("Error: Addition result exceeds %d digits.\n", MAX_DIGITS);
                exit(EXIT_FAILURE);
            }
            int digitA = (i < a->length) ? a->digits[i] : 0;
            int digitB = (i < b->length) ? b->digits[i] : 0;
            int sum = digitA + digitB + carry;
            result->digits[result->length++] = sum % 10;
            carry = sum / 10;
        }
    } else {
        BigInt bAbs = *b;
        bAbs.sign *= -1;
        subtractBigInt(a, &bAbs, result);
    }
}

// Subtraction of two BigInts //
void subtractBigInt(const BigInt *a, const BigInt *b, BigInt *result) {
    if (a->sign != b->sign) {
        BigInt bNeg = *b;
        bNeg.sign *= -1;
        addBigInt(a, &bNeg, result);
        return;
    }

    const BigInt *larger = a;
    const BigInt *smaller = b;
    result->sign = a->sign;

    if (compareBigIntAbs(a, b) < 0) {
        larger = b;
        smaller = a;
        result->sign *= -1;
    }

    result->length = 0;
    int borrow = 0;
    for (int i = 0; i < larger->length; i++) {
        int digitL = larger->digits[i];
        int digitS = (i < smaller->length) ? smaller->digits[i] : 0;
        int diff = digitL - digitS - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result->digits[result->length++] = diff;
    }

    while (result->length > 1 && result->digits[result->length - 1] == 0) {
        result->length--;
    }
}

// Multiplication of two BigInts //
void multiplyBigInt(const BigInt *a, const BigInt *b, BigInt *result) {
    for (int i = 0; i < MAX_DIGITS; i++) result->digits[i] = 0;
    result->length = 0;
    result->sign = a->sign * b->sign;

    for (int i = 0; i < a->length; i++) {
        int carry = 0;
        for (int j = 0; j < b->length || carry; j++) {
            if (i + j >= MAX_DIGITS) {
                printf("Error: Multiplication result exceeds %d digits.\n", MAX_DIGITS);
                exit(EXIT_FAILURE);
            }
            int digitA = a->digits[i];
            int digitB = (j < b->length) ? b->digits[j] : 0;
            int current = result->digits[i + j] + digitA * digitB + carry;
            result->digits[i + j] = current % 10;
            carry = current / 10;
        }
    }

    result->length = a->length + b->length;
     if (result->length > MAX_DIGITS) {
        printf("Error: Multiplication result exceeds %d digits.\n", MAX_DIGITS);
        exit(EXIT_FAILURE);
    }
    while (result->length > 1 && result->digits[result->length - 1] == 0) {
        result->length--;
    }
}

int main() {
    BigInt a, b, result;
    char inputA[MAX_DIGITS + 2], inputB[MAX_DIGITS + 2]; 
    printf("Enter the first signed number: ");
    scanf("%s", inputA);
    initBigInt(&a, inputA);

    printf("Enter the second signed number: ");
    scanf("%s", inputB);
    initBigInt(&b, inputB);

    printf("Addition: ");
    addBigInt(&a, &b, &result);
    printBigInt(&result);

    printf("Subtraction: ");
    subtractBigInt(&a, &b, &result);
    printBigInt(&result);

    printf("Multiplication: ");
    multiplyBigInt(&a, &b, &result);
    printBigInt(&result);

    return 0;
}
