#include <stdio.h>
#include <ctype.h>


#include <stdbool.h>
#include <stdlib.h>

// Checks if the input contains only digits.
bool valid(const char number[]) {
    for (int i = 0; number[i] != '\0'; i++) {
        if (!isdigit(number[i])) {
            return false;
        }
    }
    return true;
}

// Displays the correct program usage message.
void usage(void) {
    printf("Usage:\n ./credit NUMBER\n");
}

// Returns the number of digits in an integer.
int get_size(long n) {
    int count = 0;
    while (n != 0) {
        n /= 10;
        count++;
    }
    return count;
}

// Implements the Luhn Algorithm to verify the card's validity.
bool checksum(int size, long n) {
    int sum = 0;
    bool double_digit = false;

    while (n > 0) {
        int digit = n % 10;
        n /= 10;

        if (double_digit) {
            int product = digit * 2;
            sum += (product / 10) + (product % 10);
        } else {
            sum += digit;
        }

        double_digit = !double_digit;
    }

    return (sum % 10 == 0);
}

// Gets the first two digits of the card number.
int header(int size, long n) {
    while (size > 2) {
        n /= 10;
        size--;
    }
    return n;
}

// Checks if a number starts with any value within a given range.
bool starts_with_range(long n, int min, int max) {
    while (n > max) {
        n /= 10;
    }
    return (n >= min && n <= max);
}

// Determines the card type based on the prefix and length.
const char* card_type(long n) {
    int size = get_size(n);

    if (!checksum(size, n)) {
        return "INVALID";
    }

    int h = header(size, n);

    if (size == 15 && (h == 34 || h == 37)) {
        return "AMEX";
    } else if (size == 16 && h >= 51 && h <= 55) {
        return "MASTERCARD";
    } else if ((size == 13 || size == 16) && (h / 10) == 4) {
        return "VISA";
    } else if (size == 14 && (h >= 300 && h <= 305 || h == 36 || h == 38)) {
        return "DINERS CLUB";
    } else if (size == 16 && (h == 6011 || starts_with_range(n, 622126, 622925) || (h >= 644 && h <= 649) || h / 10 == 65)) {
        return "DISCOVER";
    } else if (size >= 16 && size <= 19 && starts_with_range(n, 3528, 3589)) {
        return "JCB";
    } else if (size >= 16 && size <= 19 && h / 10 == 62) {
        return "UNIONPAY";
    }

    return "INVALID";
}

int main(void) {
    char temp[25];

    // Prompt user for card number input
    printf("Enter card number: ");
    if (scanf("%24s", temp) != 1 || !valid(temp)) {
        usage();
        return EXIT_FAILURE;
    }

    // Convert input to long integer
    long num = strtol(temp, NULL, 10);
    if (num < 0) {
        usage();
        return EXIT_FAILURE;
    }

    // Print the card type
    printf("%s\n", card_type(num));
    return EXIT_SUCCESS;
}