#ifndef CLASSDLIN_H
#define CLASSDLIN_H
#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include <sstream>
#include <cmath>

class BigInt {
public:
    const int BASE =  100000000;
    BigInt();
    BigInt(std::string);
    BigInt(long long int num);
    BigInt(int num);
    BigInt(double num);
    BigInt(BigInt&);
    BigInt(const BigInt&);
    BigInt& operator=(BigInt&);
    BigInt& operator=(const BigInt&);

    BigInt& operator^(int pow);

    friend BigInt& operator+(const BigInt&);
    friend BigInt& operator-(const BigInt&);

    BigInt& operator+(BigInt&);
    BigInt& operator-(BigInt&);
    BigInt& operator*( BigInt&);
    BigInt& operator/(BigInt&);
    BigInt& operator<<(uint64_t);
    BigInt& operator>>(uint64_t);
    BigInt& operator%(BigInt&);

    BigInt& operator+(double num);
    BigInt& operator-(double num);
    double operator*(double num);
    double operator/(double num);

    BigInt& operator+(long long num);
    BigInt& operator-(long long num);
    BigInt& operator*(long long num);
    BigInt& operator/(long long num);


    friend bool operator==(const BigInt&, const BigInt&);
    friend bool operator>(const BigInt&, const BigInt&);
    friend bool operator<(const BigInt&, const BigInt&);
    friend bool operator>=(const BigInt&, const BigInt&);
    friend bool operator<=(const BigInt&, const BigInt&);
    friend bool operator!=(const BigInt&, const BigInt&);

    friend double sqrt(BigInt& inp);
    friend double log(BigInt& num);
    friend double log(double num);
    friend double log(BigInt& num);
    friend double log(BigInt& a, BigInt& b);
    friend double cos(BigInt& num);
    friend double sin(BigInt& num);
    friend std::ostream& operator<<(std::ostream&, const BigInt&);
    friend std::istream& operator>>(std::istream&, const BigInt&);



    std::vector<int64_t> number;
    int to_int(){
        if(number.size()>1){
            throw std::logic_error("Степень не инт");
        }
        return number[0] * sign;
    }

    std::string tostr(){
        std :: string ans = "";
        if(sign == -1){
            ans+='-';
        }
        ans+=number.empty() ? "0" : std::to_string(number.back());
        for (int i=(int)number.size()-2; i>=0; --i){
            std::string cur =std::to_string(number[i]);
            for(size_t i = 0;i<8-cur.size();++i){
                ans+='0';
            }
            ans+=std::to_string(number[i]);
        }
        return ans;
    }

    void fit();
protected:
    BigInt& karatsuba_mul(BigInt& fOper, BigInt& sOper);
private:
    void to_size(int sz);
    int sign;
};

#endif // CLASSDLIN_H
