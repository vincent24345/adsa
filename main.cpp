#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

//convert character to integer
int charToInt(char ch) {
    return ch - '0';
}

//convert integer to character representation
char intToChar(int num) {
    return num + '0';
}

//school method for addition
string schoolAddition(string num1, string num2, int base) {
    string result = "";
    int carryOver = 0;

    //add zeros
    while (num1.length() < num2.length()) num1 = "0" + num1;
    while (num2.length() < num1.length()) num2 = "0" + num2;

    //addition right to left
    for (int i = num1.length() - 1; i >= 0; i--) {
        int sum = charToInt(num1[i]) + charToInt(num2[i]) + carryOver;
        result = intToChar(sum % base) + result;
        carryOver = sum / base;
    }

    //if statement for carry.
    if (carryOver > 0) result = intToChar(carryOver) + result;

    return result;
}

//function for karatsuba multiplication 
string karatsubaMultiplication(string num1, string num2, int base) {
    //long multiplication
    int length1 = num1.size();
    int length2 = num2.size();
    string result(length1 + length2, '0');

    for (int i = length1 - 1; i >= 0; --i) {
        int carryOver = 0;
        for (int j = length2 - 1; j >= 0; --j) {
            int product = charToInt(num1[i]) * charToInt(num2[j]) + charToInt(result[i + j + 1]) + carryOver;
            result[i + j + 1] = intToChar(product % base);
            carryOver = product / base;
        }
        result[i] = intToChar(charToInt(result[i]) + carryOver);
    }

    //remove leading zeros
    size_t position = result.find_first_not_of('0');
    if (position != string::npos) return result.substr(position);
    return "0";
}

//function for division of two numbers
string divideNumbers(string num1, string num2, int base) {
    return "0";
}

int main() {
    string number1, number2;
    int base;
    cin >> number1 >> number2 >> base;
    string additionResult = schoolAddition(number1, number2, base);
    string multiplicationResult = karatsubaMultiplication(number1, number2, base);
    string divisionResult = divideNumbers(number1, number2, base);
    cout << additionResult << " " << multiplicationResult << " " << divisionResult << endl;
    return 0;
}
