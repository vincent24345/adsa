#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

//Convert base 10 integer to base B
string convert_to_base_b(long long num, int base) {
    if (num == 0) return "0";
    string result = "";
    while (num > 0) {
        result += to_string(num % base);
        num /= base;
    }
    reverse(result.begin(), result.end());
    return result;
}