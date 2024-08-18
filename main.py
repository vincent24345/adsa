def base_addition(num_a, num_b, base):
    #initialise variables
    carry_over = 0
    sum_digits = []

    #pad shorter number with leading zeros
    max_length = max(len(num_a), len(num_b))
    num_a = num_a.zfill(max_length)
    num_b = num_b.zfill(max_length)

    #addition for least significant digit
    for i in range(max_length - 1, -1, -1):
        digit_sum = int(num_a[i], base) + int(num_b[i], base) + carry_over
        carry_over = digit_sum // base
        sum_digits.append(digit_sum % base)

    #check for carry
    if carry_over != 0:
        sum_digits.append(carry_over)

    #results in the original base
    return ''.join(str(digit) for digit in sum_digits[::-1])


def karatsuba_multiply(a, b, base):
    #base case
    if len(a) == 1 or len(b) == 1:
        return str(int(a, base) * int(b, base))

    #make sure equal length
    max_length = max(len(a), len(b))
    a = a.zfill(max_length)
    b = b.zfill(max_length)
    half_length = max_length // 2

    #split high and low
    high_a, low_a = a[:-half_length], a[-half_length:]
    high_b, low_b = b[:-half_length], b[-half_length:]

    #recurisve calls
    low_low = karatsuba_multiply(low_a, low_b, base)
    high_low_sum = karatsuba_multiply(str(int(low_a, base) + int(high_a, base)), str(int(low_b, base) + int(high_b, base)), base)
    high_high = karatsuba_multiply(high_a, high_b, base)
    
    result = (int(high_high, base) * (base ** (2 * half_length)) +
              (int(high_low_sum, base) - int(high_high, base) - int(low_low, base)) * (base ** half_length) +
              int(low_low, base))
    
    return str(result)

def main():
    input_line = input().strip()
    num1, num2, base = input_line.split()
    base = int(base)
    
    addition_result = base_addition(num1, num2, base)
    multiplication_result = karatsuba_multiply(num1, num2, base)
    
    print(f"{addition_result} {multiplication_result} 0")

if __name__ == "__main__":
    main()
