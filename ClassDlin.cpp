#include "ClassDlin.h"
#include "cmath"
BigInt::BigInt(): sign(1), number(NULL){}

BigInt::BigInt(BigInt& num){
    this->sign=num.sign;
    this->number=num.number;
}

BigInt::BigInt(const BigInt& num){
    this->sign=num.sign;
    this->number=num.number;
}

BigInt& BigInt::operator=(BigInt& num){
    this->sign=num.sign;
    this->number=num.number;
    return *this;
}

BigInt& BigInt::operator=(const BigInt& num){
    this->sign=num.sign;
    this->number=num.number;
    return *this;
}

BigInt::BigInt(long long num){
    if(num < 0){
        this->sign = -1;
        num = -num;
    } else {
        this->sign = 1;
    }
    this->number.push_back(0);
    this->number.push_back(0);
    this->number.push_back(0);
    this->number[0] = num % this->BASE;
    this->number[1] = (num / this->BASE) % this->BASE;
    this->number[2] = (num / this->BASE) / this->BASE;
    this->fit();
}

BigInt::BigInt(int num){
    if(num < 0){
        this->sign = -1;
        num = -num;
    } else {
        this->sign = 1;
    }
    this->number.push_back(0);
    this->number.push_back(0);
    this->number[0] = num % this->BASE;
    this->number[1] = (num / this->BASE) % this->BASE;
}

BigInt::BigInt(double num){
    std::string tmp = std::to_string(num);
    int ind = tmp.find(".");
    tmp.erase(ind, tmp.end() - tmp.begin());
    BigInt* res = new BigInt(tmp);
    this->number.clear();
    (*this) = (*res);
}

BigInt::BigInt(std::string num){
    if(num.size() == 0){
        std::string mes = "Нулевая длина стринги";
        throw std::invalid_argument(mes);
    } else {
        if (num[0] == '-') {
            num = num.substr(1);
            sign = -1;
        }
        else {
            sign = 1;
        }
        for(int it = ((int)num.size());it > 0; it-=8){
            int F;
            std::string temp;
            if(it < 8){
                std::stringstream buff;
                buff << num.substr(0, it);
                buff>>F;
                number.push_back(F);
            } else {
                std::stringstream buff;
                buff << num.substr(it - 8, 8);
                buff >> F;
                number.push_back(F);
            }
        }
        this->fit();
    }
}

BigInt& BigInt::operator^(int step){
    BigInt* res = new BigInt("1");
    BigInt* base = new BigInt(*this);
    while(step){
        if(step % 2){
            (*res) = (*res)*(*base);
        }
        (*base) = (*base)*(*base);
        step >>=1;
    }
    return *res;
}


BigInt& operator+(BigInt& num){
    return num;
}

BigInt& operator-(const BigInt& num){
    auto tmp = new BigInt(num);
    tmp->sign=-tmp->sign;
    return *tmp;
}

BigInt& BigInt::operator+(BigInt& b){
    if(this->sign == -1){
        if(b.sign == -1){
            return -(-(*this) + (-b));
        } else {
            return (b - (-(*this)));
        }
    } else if(b.sign == -1){
        return ((*this) - (-b));
    }

    BigInt* result = new BigInt((*this));
    int mxLen = (this->number.size() > b.number.size()) ? this->number.size() : b.number.size();
    int carry = 0;
    for(int it = 0; it < mxLen || carry !=0; ++it){
        if(it == result->number.size()){
            result->number.push_back(0);
        }
        result->number[it]+= carry + ((b.number.size() > it) ? b.number[it] : 0);
        carry = result->number[it] > result->BASE;
        result->number[it] = result->number[it] - result->BASE * carry;
    }
    result->fit();
    return *result;
}

BigInt& BigInt::operator-(BigInt& b){
    if(this->sign == -1){
        if(b.sign == -1){
            return b - (-(*this));
        } else {
            return -(-(*this) + (-b));
        }
    } else if(b.sign==-1){
        return (*this) + (-b);
    }

    if((*this) < b){
        return -(b-(*this));
    }

    BigInt* result = new BigInt((*this));
    int carry = 0;
    for(int iter = 0; iter < b.number.size() || carry !=0; ++iter){
        result->number[iter]-= carry + (b.number[iter]);
        carry = result->number[iter] < 0;
        result->number[iter] = result->number[iter] + result->BASE * carry;
    }
    result->fit();
    return *result;
}



BigInt& BigInt::operator<<(uint64_t num){

    if (this->number.size() == 0) {
        this->number.push_back(0);
        return *this;
    }
    for(uint64_t it = 0; it < num; ++it){
        this->number.insert(this->number.begin(), 0);
    }
    return *this;
}
BigInt& BigInt::operator>>(uint64_t num){

    if (this->number.size() == 0) {
        this->number.push_back(0);
        return *this;
    }
    this->number.erase(this->number.begin(),this->number.begin() + num);
    return *this;
}


BigInt& BigInt::operator/(BigInt& num){
    if(this->number<num.number){
        BigInt* result = new BigInt();
        result->number={0};
        result->sign = 1;
        return *result;
    }
    if(num.number.size() == 0){
        throw std::invalid_argument("Деление на ноль");
    }
    BigInt b(num), current, lf((*this));
    BigInt* result = new BigInt();
    b.sign = 1;
    current.sign = 1;
    result->sign = 1;
    lf.sign = 1;
    b<<(this->number.size()-num.number.size());
    result->number.resize(this->number.size());
    for (long long it = static_cast<long long>(this->number.size()-num.number.size()); it >=0; --it) {
        current.fit();
        long long x = 0, lhs = 0, rhs = this->BASE;
        while (lhs <= rhs) {
            int mid = (rhs + lhs) / 2;
            BigInt tt = BigInt(mid);
            BigInt t = b * tt;
            if (t <= lf) {
                x = mid;
                lhs = mid + 1;
            } else {
                rhs = mid - 1;
            }
            result->number[it] = x;
        }
        lf = lf - b * (*new BigInt(x));
        b>>1;

    }
    result->sign = this->sign / num.sign;
    result->fit();
    return *result;
}

BigInt& BigInt::operator*(BigInt& b){

    BigInt* result = new BigInt(this->karatsuba_mul(*this, b));
    result->sign = this->sign * b.sign;
    this->fit();
    b.fit();
    result->fit();
    return *result;

}

BigInt& BigInt::operator%(BigInt& b){
    BigInt* res = new BigInt(((*this)- (((*this) / b) * b)));
    if (this->sign == -1){
        (*res) = b - (*res);
    }
    return *res;
}

BigInt& BigInt::karatsuba_mul(BigInt& a, BigInt& b){
    BigInt* result = new BigInt;
    result->sign = a.sign * b.sign;
    int m_size = (a.number.size() > b.number.size())? (a.number.size()) : (b.number.size());
    int l_size = (a.number.size() < b.number.size())? (a.number.size()) : (b.number.size());
    if(l_size == 0){
        result->number = {0};
        return *result;
    }
    if(m_size == 1){
        result->number = {a.number[0]*b.number[0], 0};
        return *result;
    }
    m_size = m_size + m_size % 2;
    a.to_size(m_size);
    b.to_size(m_size);

    int h_size = m_size / 2;

    result->number = std::vector<int64_t>(m_size * 2, 0);
    BigInt B; B.number = std::vector<int64_t>(a.number.begin(), a.number.begin() + h_size);
    BigInt A; A.number = std::vector<int64_t>(a.number.begin() + h_size, a.number.end());
    BigInt D; D.number = std::vector<int64_t>(b.number.begin(), b.number.begin() + h_size);
    BigInt C; C.number = std::vector<int64_t>(b.number.begin() + h_size, b.number.end());


    BigInt AC; AC = karatsuba_mul(A, C);

    BigInt BD; BD = karatsuba_mul(B, D);


    BigInt AplB; AplB.number = std::vector<int64_t>(h_size);
    BigInt CplD; CplD.number = std::vector<int64_t>(h_size);


    for(int it = 0; it < h_size; ++it){
        AplB.number[it] = A.number[it] + B.number[it];
        CplD.number[it] = C.number[it] + D.number[it];
    }

    BigInt SKOBKI; SKOBKI = karatsuba_mul(AplB, CplD);
    for(int it = 0; it < m_size; ++it){
        SKOBKI.number[it] -= AC.number[it] + BD.number[it];
    }

    for(int it = 0; it < m_size; ++it){
        result->number[it] = BD.number[it];
    }

    for(int it = m_size; it < m_size * 2; ++it){
        result->number[it] = AC.number[it - m_size];
    }

    for(int it = h_size; it < m_size + h_size; ++it){
        result->number[it] += SKOBKI.number[it-h_size];
    }
    return *result;

}


BigInt& BigInt::operator+(double num){
    BigInt* tmp = new BigInt(num);
    BigInt* res = new BigInt((*this) + (*tmp));
    delete tmp;
    return *res;
}
BigInt& BigInt::operator-(double num){
    BigInt* tmp = new BigInt(num);
    BigInt* res = new BigInt((*this) - (*tmp));
    delete tmp;
    return *res;
}
double BigInt::operator*(double num){
    std::stringstream buf;
    buf << *this;
    double b;
    buf >> b;
    return b * num;
}
double BigInt::operator/(double num){
    if(num == 0){
        throw std::invalid_argument("division by zero");
    }
    std::stringstream buf;
    buf << *this;
    double b;
    buf >> b;
    return b / num;
}

BigInt& BigInt::operator+(long long num){
    BigInt* tmp = new BigInt(num);
    BigInt* res = new BigInt((*this) + (*tmp));
    delete tmp;
    return *res;
}
BigInt& BigInt::operator-(long long num){
    BigInt* tmp = new BigInt(num);
    BigInt* res = new BigInt((*this) - (*tmp));
    delete tmp;
    return *res;
}
BigInt& BigInt::operator*(long long num){
    BigInt* tmp = new BigInt(num);
    BigInt* res = new BigInt((*this) * (*tmp));
    delete tmp;
    return *res;
}
BigInt& BigInt::operator/(long long num){
    BigInt* tmp = new BigInt(num);
    BigInt* res = new BigInt((*this) / (*tmp));
    delete tmp;
    return *res;
}



bool operator==(const BigInt& a, const BigInt& b){
    if(a.sign != b.sign){
        return false;
    }

    if(a.number == b.number){
        return true;
    } else {
        return false;
    }
}

bool operator>(const BigInt& a, const BigInt& b){
    if(a.sign == -1){
        if(b.sign == -1){
            return (-b) > (-a);
        } else {
            return false;
        }
    } else if(b.sign == -1){
        return true;
    }

    if(a.number.size() != b.number.size()){
        return a.number.size() > b.number.size();
    } else {
        for(int iter = a.number.size()-1; iter >=0; --iter){
            if(a.number[iter] != b.number[iter]){
                return a.number[iter] > b.number[iter];
            }
        }
        return false;
    }
}

bool operator>=(const BigInt& a, const BigInt& b){
    return operator>(a, b) || operator==(a, b);
}

bool operator<(const BigInt& a, const BigInt& b){
    return ! operator>=(a, b);
}

bool operator<=(const BigInt& a, const BigInt& b){
    return !operator>(a, b);
}


void BigInt::fit(){
    number.push_back(0);
    for(uint64_t iter = 0; iter < number.size() - 1; ++iter){
        number[iter + 1] = number[iter + 1]  + number[iter] / BASE;
        number[iter] %=BASE;
    }
    for(int iter = number.size()-1; iter>=0; --iter){
        if(number[iter] == 0){
            number.pop_back();
        } else {
            return;
        }
    }
    return;
}


void BigInt::to_size(int nSize){
    int sz = number.size();
    if(nSize>=sz){
        for(int it=0; it< (nSize-sz);++it){
            number.push_back(0);
        }
    }
    return;
}

double sqrt(BigInt& inp){
    std::stringstream buf;
    long double num;
    buf << inp;
    buf >> num;
    return sqrt(num);
}

double log(BigInt& num){
    std::stringstream buf;
    long double num1;
    buf << num;
    buf >> num1;
    return log(num1);
}

double sin(BigInt& num){
    std::stringstream buf;
    long double num1;
    buf << num;
    buf >> num1;
    return sin(num1);
}

double cos(BigInt& num){
    std::stringstream buf;
    long double num1;
    buf << num;
    buf >> num1;
    return cos(num1);
}

double log(BigInt& a, BigInt& b){
    return log(a) / log(b);
}

std::ostream& operator<<(std::ostream& out, const BigInt& num){
    std::string ans;
    if(num.number.size()==0){
        ans = "0";
    } else {
        if(num.sign == -1){
            ans = "-";
        }
        std::stringstream buff;
        buff << num.number[num.number.size() - 1];
        std::string temp;
        buff >> temp;
        ans +=temp;
        buff.clear();
        for(int it = num.number.size()-2; it >=0; --it){
            buff << num.number[it];
            temp = "";
            buff >> temp;
            for(uint64_t it = 0; it < (8 - temp.length()); ++it){
                ans+="0";
            }
            ans+=temp;
            buff.clear();
        }
    }

    out << ans;
    return out;
}

//int main(){
//    std::string a;
//    std::string b;
//    std::cin>>a>>b;
//    BigInt f(a);
//    BigInt s(b);
//    std::cout<<f/s;
//}
