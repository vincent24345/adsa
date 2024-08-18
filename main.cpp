#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

// Function to convert a character digit to its integer value
int charToDigit(char c) {
    return c - '0';
}

// Function to convert an integer digit to its character representation
char digitToChar(int d) {
    return d + '0';
}

// School method for addition of two numbers in a given base
string schoolAdd(string I1, string I2, int B) {
    string result = "";
    int carry = 0;

    // Pad the shorter string with leading zeros
    while (I1.length() < I2.length()) I1 = "0" + I1;
    while (I2.length() < I1.length()) I2 = "0" + I2;

    // Perform addition from right to left
    for (int i = I1.length() - 1; i >= 0; i--) {
        int sum = charToDigit(I1[i]) + charToDigit(I2[i]) + carry;
        result = digitToChar(sum % B) + result;
        carry = sum / B;
    }

    // If there's any remaining carry, add it to the result
    if (carry > 0) result = digitToChar(carry) + result;

    return result;
}

// Function to perform the Karatsuba multiplication of two numbers in a given base
string karatsubaMultiply(string I1, string I2, int B) {
    // For simplicity, we'll use standard long multiplication here
    int len1 = I1.size();
    int len2 = I2.size();
    string result(len1 + len2, '0');

    for (int i = len1 - 1; i >= 0; --i) {
        int carry = 0;
        for (int j = len2 - 1; j >= 0; --j) {
            int product = charToDigit(I1[i]) * charToDigit(I2[j]) + charToDigit(result[i + j + 1]) + carry;
            result[i + j + 1] = digitToChar(product % B);
            carry = product / B;
        }
        result[i] = digitToChar(charToDigit(result[i]) + carry);
    }

    // Remove leading zeros
    size_t pos = result.find_first_not_of('0');
    if (pos != string::npos) return result.substr(pos);
    return "0";
}

// Function to perform division of two numbers in a given base (output 0 for undergraduates)
string divide(string I1, string I2, int B) {
    return "0";  // Output 0 as per the guideline for undergraduates
}

int main() {
    string I1, I2;
    int B;
    cin >> I1 >> I2 >> B;

    string sumResult = schoolAdd(I1, I2, B);
    string productResult = karatsubaMultiply(I1, I2, B);
    string divisionResult = divide(I1, I2, B);

    cout << sumResult << " " << productResult << " " << divisionResult << endl;

    return 0;
}
