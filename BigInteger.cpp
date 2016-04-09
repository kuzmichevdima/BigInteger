#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

class BigInteger {
    private:
    vector <int> * digits;
    bool negative;

    void cutZeros() {
        while(digits->size() > 1 && digits->back() == 0)
            digits->pop_back();
        if (digits->size() == 1 && (*digits)[0] == 0)
            negative = false;
    }

    public:
    BigInteger() {
        digits = new vector <int> ();
        negative = false;
    }
    BigInteger(vector <int> _digits, bool _negative) {
        digits = new vector <int> (_digits);
        negative = _negative;
    }

    BigInteger(string s) {
        digits = new vector <int> ();
        if (s[0] == '-') {
            s = s.substr(1, (int)s.length() - 1);
            negative = true;
        }
        else negative = false;
        for (int j = (int)s.length() - 1; j >= 0; j--)
            digits->push_back(s[j] - '0');
    }


    BigInteger(long long x) : BigInteger(to_string(x)) {}

    BigInteger abs() const {
        if (negative) {
            BigInteger result = *this;   
            result.setNegative(false);
            return result;
        }
        else return *this;
    }

    BigInteger opposite() const {
        BigInteger result = *this;
        result.changeSign();
        return result;
    }

    void addDigit(int digit) {
        digits->push_back(digit);
    }

    bool isZero () const {
        return digits->size() == 1 && (*digits)[0] == 0;
    }

    void changeSign () {
        if (isZero())
            return;
        negative = !negative;
    }
    bool isNegative() const {
        return negative;
    }

    vector <int> getDigits() const {
        return *digits;
    }
    size_t size() const {
        return digits->size();
    }

    void setNegative(bool _negative) {
        negative = _negative;
    }

    static BigInteger readNewInstance() {
        string s;
        cin >> s;
        return BigInteger(s);
    }
 
    BigInteger& operator =(const BigInteger & another) {
        digits = new vector <int>(another.getDigits());
        negative = another.isNegative();
        return *this;
    }

    BigInteger(const BigInteger & another) {
        digits = new vector <int> (another.getDigits());
        negative = another.isNegative();
    }


    string toString() const {
        string result = "";
        if (negative)
            result += "-";
        for (int j = (int)digits->size() - 1; j >= 0; j--)
            result += '0' + (*digits)[j];
        return result;
    }

    void print() const {
        cout << toString() << endl;
    }

    bool operator < (BigInteger another) {
        if (negative != another.isNegative())
            return negative;
        bool absLess;
        if (digits->size() != another.size())
            absLess = digits->size() < another.size();
        else {   
            absLess = false;
            for (int i = (int)digits->size() - 1; i >= 0; i--)
                if ((*this)[i] != another[i]) {
                    absLess = (*this)[i] < another[i];
                    break;
                }
        }
        return absLess ^ negative;
    }

    BigInteger operator + (BigInteger another) {
        if (another.isZero())
            return *this;
        if (negative != another.isNegative()) {
            return *this - another.opposite();
        }
        BigInteger result = BigInteger();
        result.setNegative(negative);
        int i, carry = 0;
        for (i = 0; i < (int)min(digits->size(), another.size()); i++) {
            int s = (*this)[i] + another[i] + carry;
            result.addDigit(s % 10);
            carry = s / 10;
        }
        while(i < (int)digits->size()) {
            int s = (*this)[i] + carry;
            result.addDigit(s % 10);
            carry = s / 10;
            i++;
        }
        while(i < (int)another.size()) {
            int s = another[i] + carry;
            result.addDigit(s % 10);
            carry = s / 10;
            i++;
        }
        if (carry != 0)
            result.addDigit(carry);
        //printf("in operator +");
        //result.print();
        result.cutZeros();
        return result;
    }

    BigInteger operator - (BigInteger another) {
        if (negative != another.isNegative()) {
            another.changeSign();
            return *this+another;
        }
        if (abs() < another.abs()) {
            BigInteger result = another - *this;
            result.changeSign();
            return result;
        }
        vector <int> d;
        int carry = 0;
        BigInteger result = BigInteger();
        result.setNegative(negative);
        for (int i = 0; i < (int)digits->size(); i++) {
            int s = (*digits)[i] - (i < (int)another.size() ? another[i] : 0) + carry;
            if (s < 0) {
                s += 10;
                carry = -1;
            }
            else carry = 0;
            result.addDigit(s);
        }
        assert(carry == 0);
        result.cutZeros();
        return result;
    }

    BigInteger operator * (long long x) {
        if (x == 0) {
            BigInteger result = BigInteger();
            result.addDigit(0);
            return result;
        }

        if (x < 0) {
            BigInteger result = *this * (-x);
            result.changeSign();
            return result;
        }
        BigInteger result = BigInteger();
        result.setNegative(negative);
        long long carry = 0;
        for (int j = 0; j < (int)digits->size(); j++) {
            long long s = x * (*digits)[j] + carry;
            result.addDigit(s % 10);
            carry = s / 10;
        }
        while(carry > 0) {
            result.addDigit(carry % 10);
            carry /= 10;
        }
        return result;
    }

    int operator [] (int pos) {
        assert(pos < (int)digits->size());
        return (*digits)[pos];
    }

    BigInteger operator * (BigInteger another) {
        BigInteger result = BigInteger();
        result.addDigit(0);
        for (int i = 0; i < (int)another.size(); i++) {
            BigInteger add = *this * another[i];
            vector <int> d(i, 0);
            for (int digit : add.getDigits())
                d.push_back(digit);
            //cout << "add:" << endl;
            //add.print();
            /*cout << "sum: " << endl;*/
            result = result + BigInteger(d, add.isNegative());
            /*cout << "result: " << endl;
            cout << "size = " << result.size() << endl;
            result.print();*/
        }
        //cout << "finally" << endl;
        //result.print();
        result.setNegative(negative ^ another.isNegative());
        result.cutZeros();
        return result;
    }

    void addBackDigit(int digit) {
        vector <int> new_digits;
        new_digits.push_back(digit);
        for (int d : *digits)
            new_digits.push_back(d);
        *digits = new_digits;
    }

    BigInteger operator / (BigInteger another) {
        assert(!another.isZero());
        if (another.isNegative())
        {
            BigInteger result = *this / another.opposite();
            result.changeSign();
            return result;
        }
        BigInteger cur = BigInteger();
        vector <int> result_d;
        for (int pos = (int)digits->size() - 1; pos >= 0; pos--) {
            cur.addBackDigit((*digits)[pos]);
            //cout << "cur: " << endl;
            //cur.print();
            for (int val = 1; val <= 10; val++) {
                if (cur < another * val) {
                   // cout << pos << " " << val - 1 << endl;
                    result_d.push_back(val - 1);
                    cur = cur - another * (val - 1);
                    break;
                }
            }
        }
        reverse(result_d.begin(), result_d.end());
        BigInteger result = BigInteger(result_d, negative);
        result.cutZeros();
        //cout << "result: ";
        //result.print();
        return result;
    }

    static BigInteger genRandom() {
        BigInteger result = BigInteger();
        if (rand() % 2 == 0) {
            result.setNegative(true);
        }
        for (int i = 0; i < rand() % 20; i++) {
            result.addDigit(rand() % 10);        
        }
        return result;
    }

    ~BigInteger() {
        delete digits;
    }
};

const int TESTS_CNT = 10000;

void testPlusMinusOperators() {
    vector <pair<int, int >> tests;
    tests.push_back(make_pair(1000, 3));
    for (int it = 0; it < TESTS_CNT; it++) {
        tests.push_back(make_pair(rand() * (-1 + rand() % 2 * 2), rand() * (-1 + rand() % 2 * 2)));
    }
    for (int it = 0; it < TESTS_CNT; it++) {
        
        tests.push_back(make_pair(rand() * (-1 + rand() % 2 * 2), 0));
        tests.push_back(make_pair(0, rand() * (-1 + rand() % 2 * 2)));

        int x = rand() * (-1 + rand() % 2 * 2);
        tests.push_back(make_pair(x, -x));
    }
    for (auto p : tests) {
        long long a1 = p.first;
        long long b1 = p.second;
        BigInteger a = BigInteger(a1);
        BigInteger b = BigInteger(b1);
        cout << a1 << " " << b1 << endl;
        assert((a + b).toString() == to_string(a1 + b1));
        assert((a - b).toString() == to_string(a1 - b1));
        assert((a * b1).toString() == to_string(a1 * b1));
        assert((b * a1).toString() == to_string(a1 * b1));
        assert((a * b).toString() == to_string(a1 * b1));
        assert((b * a).toString() == to_string(a1 * b1));
        assert(b1 == 0 || (a / b).toString() == to_string(a1 / b1));
        assert(a1 == 0 || (b / a).toString() == to_string(b1 / a1));
    }
    cout << "+ and - tests OK." << tests.size() << " tests completed" << endl;
}


void testAll() {
    testPlusMinusOperators();
}

int main() {
    testAll();
}

