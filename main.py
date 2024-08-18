def karatsuba(x, y):
    # Base case for recursion
    if x < 10 or y < 10:
        return x * y

    # Calculate the size of the numbers
    n = max(len(str(x)), len(str(y)))
    half = n // 2

    # Split the digit sequences in the middle
    high1, low1 = divmod(x, 10**half)
    high2, low2 = divmod(y, 10**half)

    # Perform three multiplications
    z0 = karatsuba(low1, low2)
    z1 = karatsuba((low1 + high1), (low2 + high2))
    z2 = karatsuba(high1, high2)

    # Combine the results
    return (z2 * 10**(2*half)) + ((z1 - z2 - z0) * 10**half) + z0
# Test
a = 1234
b = 5678
result = karatsuba(a, b)
print(f"The result of {a} * {b} is {result}")
