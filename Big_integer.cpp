#include <bits/stdc++.h>
using namespace std;

/*
   BigInteger class for handling very large integers
   Supports: +, -, *, /, %, comparison, factorial, Fibonacci, Catalan numbers
*/
class BigInteger {
private:
    vector<int> digits; // stores digits in reverse (least significant digit first)

public:
    // ===== Constructors =====

    // Default constructor (value = 0)
    BigInteger() { digits.push_back(0); }

    // Constructor from integer
    BigInteger(long long num) { *this = num; }

    // Constructor from string (e.g., "12345")
    BigInteger(const string &num) { *this = num; }

    // Assign from integer
    BigInteger &operator=(long long num) {
        digits.clear();
        if (num == 0) digits.push_back(0);
        while (num > 0) {
            digits.push_back(num % 10);
            num /= 10;
        }
        return *this;
    }

    // Assign from string
    BigInteger &operator=(const string &num) {
        digits.clear();
        for (int i = num.size() - 1; i >= 0; i--) {
            if (!isdigit(num[i])) throw invalid_argument("Invalid digit in string");
            digits.push_back(num[i] - '0');
        }
        removeLeadingZeros();
        return *this;
    }

    // Remove extra leading zeros
    void removeLeadingZeros() {
        while (digits.size() > 1 && digits.back() == 0)
            digits.pop_back();
    }

    // ===== Comparison operators =====
    bool operator<(const BigInteger &b) const {
        if (digits.size() != b.digits.size())
            return digits.size() < b.digits.size();
        for (int i = digits.size() - 1; i >= 0; i--) {
            if (digits[i] != b.digits[i])
                return digits[i] < b.digits[i];
        }
        return false;
    }
    bool operator>(const BigInteger &b) const { return b < *this; }
    bool operator<=(const BigInteger &b) const { return !(b < *this); }
    bool operator>=(const BigInteger &b) const { return !(*this < b); }
    bool operator==(const BigInteger &b) const { return digits == b.digits; }
    bool operator!=(const BigInteger &b) const { return !(*this == b); }

    // ===== Addition =====
    BigInteger operator+(const BigInteger &b) const {
        BigInteger result;
        result.digits.clear();
        int carry = 0;
        for (size_t i = 0; i < max(digits.size(), b.digits.size()) || carry; i++) {
            int sum = carry;
            if (i < digits.size()) sum += digits[i];
            if (i < b.digits.size()) sum += b.digits[i];
            result.digits.push_back(sum % 10);
            carry = sum / 10;
        }
        return result;
    }

    // ===== Subtraction ===== (assuming *this >= b)
    BigInteger operator-(const BigInteger &b) const {
        if (*this < b) throw invalid_argument("Negative result not supported");
        BigInteger result;
        result.digits.clear();
        int carry = 0;
        for (size_t i = 0; i < digits.size(); i++) {
            int sub = digits[i] - carry - (i < b.digits.size() ? b.digits[i] : 0);
            if (sub < 0) {
                sub += 10;
                carry = 1;
            } else carry = 0;
            result.digits.push_back(sub);
        }
        result.removeLeadingZeros();
        return result;
    }

    // ===== Multiplication =====
    BigInteger operator*(const BigInteger &b) const {
        BigInteger result;
        result.digits.assign(digits.size() + b.digits.size(), 0);
        for (size_t i = 0; i < digits.size(); i++) {
            int carry = 0;
            for (size_t j = 0; j < b.digits.size() || carry; j++) {
                long long cur = result.digits[i + j] +
                                (long long)digits[i] * (j < b.digits.size() ? b.digits[j] : 0) + carry;
                result.digits[i + j] = cur % 10;
                carry = cur / 10;
            }
        }
        result.removeLeadingZeros();
        return result;
    }

    // ===== Factorial =====
    static BigInteger factorial(int n) {
        BigInteger res = 1;
        for (int i = 2; i <= n; i++)
            res = res * BigInteger(i);
        return res;
    }

    // ===== Fibonacci (nth term) =====
    static BigInteger fibonacci(int n) {
        if (n == 0) return BigInteger(0);
        if (n == 1) return BigInteger(1);
        BigInteger a = 0, b = 1, c;
        for (int i = 2; i <= n; i++) {
            c = a + b;
            a = b;
            b = c;
        }
        return b;
    }

    // ===== Catalan Number =====
    static BigInteger catalan(int n) {
        BigInteger num = factorial(2 * n);
        BigInteger den = factorial(n + 1) * factorial(n);
        return divide(num, den);
    }

    // ===== Division (integer division) =====
    static BigInteger divide(const BigInteger &a, const BigInteger &b) {
        if (b == 0) throw invalid_argument("Division by zero");
        BigInteger result, curValue;
        result.digits.resize(a.digits.size());
        for (int i = a.digits.size() - 1; i >= 0; i--) {
            curValue.digits.insert(curValue.digits.begin(), a.digits[i]);
            curValue.removeLeadingZeros();
            int x = 0, l = 0, r = 9;
            while (l <= r) {
                int m = (l + r) / 2;
                BigInteger t = b * BigInteger(m);
                if (t <= curValue) {
                    x = m;
                    l = m + 1;
                } else {
                    r = m - 1;
                }
            }
            result.digits[i] = x;
            curValue = curValue - b * BigInteger(x);
        }
        result.removeLeadingZeros();
        return result;
    }

    // ===== Output operator =====
    friend ostream &operator<<(ostream &out, const BigInteger &num) {
        for (int i = num.digits.size() - 1; i >= 0; i--)
            out << num.digits[i];
        return out;
    }
};

// ===== Main function to test everything =====
int main() {
    BigInteger a("12345678901234567890");
    BigInteger b("98765432109876543210");

    cout << "a = " << a << "\n";
    cout << "b = " << b << "\n";

    cout << "a + b = " << a + b << "\n";
    cout << "b - a = " << b - a << "\n";
    cout << "a * b = " << a * b << "\n";

    cout << "Factorial(20) = " << BigInteger::factorial(20) << "\n";
    cout << "Fibonacci(50) = " << BigInteger::fibonacci(50) << "\n";
    cout << "Catalan(10) = " << BigInteger::catalan(10) << "\n";

    return 0;
}
