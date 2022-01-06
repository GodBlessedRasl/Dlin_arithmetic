#include "Dlinka.h"
#include "cmath"
BigInt::BigInt(): sign(1), number(NULL){}

BigInt::BigInt(BigInt& prot){
    this->sign=prot.sign;
    this->number=prot.number;
}

BigInt::BigInt(const BigInt& prot){
    this->sign=prot.sign;
    this->number=prot.number;
}

BigInt& BigInt::operator=(BigInt& prot){
    this->sign=prot.sign;
    this->number=prot.number;
    return *this;
}

BigInt& BigInt::operator=(const BigInt& prot){
    this->sign=prot.sign;
    this->number=prot.number;
    return *this;
}

BigInt::BigInt(long long inp){
    if(inp < 0){
        this->sign = -1;
        inp = -inp;
    } else {
        this->sign = 1;
    }
    this->number.push_back(0);
    this->number.push_back(0);
    this->number.push_back(0);
    this->number[0] = inp % this->BASE;
    this->number[1] = (inp / this->BASE) % this->BASE;
    this->number[2] = (inp / this->BASE) / this->BASE;
    this->fit();
}

BigInt::BigInt(int inp){
    if(inp < 0){
        this->sign = -1;
        inp = -inp;
    } else {
        this->sign = 1;
    }
    this->number.push_back(0);
    this->number.push_back(0);
    this->number[0] = inp % this->BASE;
    this->number[1] = (inp / this->BASE) % this->BASE;
}

BigInt::BigInt(double inp){
    std::string sm = std::to_string(inp);
    int ind = sm.find(".");
    //    std::cout << " fd " << ind << "   " << buf << "\n";
    sm.erase(ind, sm.end() - sm.begin());
    //    std::cout << " fd " << ind << "   " << buf << "\n";
    BigInt* res = new BigInt(sm);
    this->number.clear();
    (*this) = (*res);
}

BigInt::BigInt(std::string inp){
    if(inp.size() == 0){
        std::string message = "zero-lenght string";
        throw std::invalid_argument(message);
    } else {
        if (inp[0] == '-') {
            inp = inp.substr(1);
            sign = -1;
        }
        else {
            sign = 1;
        }
        for(int iter = ((int)inp.size());iter > 0; iter-=8){
            int intBUFF;
            std::string temp;
            if(iter < 8){
                std::stringstream buff;
                buff << inp.substr(0, iter);
                buff>>intBUFF;
                number.push_back(intBUFF);
            } else {
                std::stringstream buff;
                buff << inp.substr(iter - 8, 8);
                buff >> intBUFF;
                number.push_back(intBUFF);
            }
        }
        this->fit();
    }
}

BigInt& BigInt::operator^(int pow){
    BigInt* res = new BigInt("1");
    BigInt* base = new BigInt(*this);
    while(pow){
        if(pow % 2){
            (*res) = (*res)*(*base);
        }
        (*base) = (*base)*(*base);
        pow >>=1;
    }
    return *res;
}


BigInt& operator+(BigInt& in){
    return in;
}

BigInt& operator-(const BigInt& in){
    auto tmp = new BigInt(in);
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
    for(int iter = 0; iter < mxLen || carry !=0; ++iter){
        if(iter == result->number.size()){
            result->number.push_back(0);
        }
        result->number[iter]+= carry + ((b.number.size() > iter) ? b.number[iter] : 0);
        carry = result->number[iter] > result->BASE;
        result->number[iter] = result->number[iter] - result->BASE * carry;
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



BigInt& BigInt::operator<<(uint64_t mv){

    if (this->number.size() == 0) {
        this->number.push_back(0);
        return *this;
    }
    for(uint64_t it = 0; it < mv; ++it){
        this->number.insert(this->number.begin(), 0);
    }
    return *this;
}
BigInt& BigInt::operator>>(uint64_t mv){

    if (this->number.size() == 0) {
        this->number.push_back(0);
        return *this;
    }
    this->number.erase(this->number.begin(),this->number.begin() + mv);
    return *this;
}


BigInt& BigInt::operator/(BigInt& right){
    if(right.number.size() == 0){
        throw std::invalid_argument("division by zero");
    }
    BigInt b(right), current, lf((*this));
    BigInt* result = new BigInt();
    b.sign = 1;
    current.sign = 1;
    result->sign = 1;
    lf.sign = 1;
    b<<(this->number.size()-right.number.size());
    result->number.resize(this->number.size());
    for (long long it = static_cast<long long>(this->number.size()-right.number.size()); it >=0; --it) {
        //        std::cout << current << " bef  ";
        //        current<<1;

        //        current.number[0] = left.number[it];
        //        std::cout << current << " af\n";
        current.fit();

        long long x = 0, lhs = 0, rhs = this->BASE;
        while (lhs <= rhs) {
            //            std::cout << " b: " << b << " ";
            int mid = (rhs + lhs) / 2;
            BigInt tt = BigInt(mid);
            BigInt t = b * tt;
            //            std::cout <<"  t: " <<  t <<"  lf: " << lf << " mid " << tt << "  iter ";
            if (t <= lf) {
                //                std::cout << "true\n";
                x = mid;
                lhs = mid + 1;
            } else {
                //                std::cout << "false\n";
                rhs = mid - 1;
            }
            result->number[it] = x;
        }
        //        std::cout << x << " preres\n";
        lf = lf - b * (*new BigInt(x));
        b>>1;

    }
    result->sign = this->sign / right.sign;
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
        //        std::cout << "yeep  " << result->number[0] << "  yes ";
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
/*LNum&*/ double BigInt::operator*(double num){
    std::stringstream buf;
    buf << *this;
    double b;
    buf >> b;
    return b * num;
//    LNum* tmp = new LNum(num);
//    LNum* res = new LNum((*this) * (*tmp));
//    delete tmp;
//    return *res;
}
/*LNum&*/double BigInt::operator/(double num){
    if(num == 0){
        throw std::invalid_argument("division by zero");
    }
    std::stringstream buf;
    buf << *this;
    double b;
    buf >> b;
    return b / num;
//    LNum* tmp = new LNum(num);
//    LNum* res = new LNum((*this) / (*tmp));
//    delete tmp;
//    return *res;
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
    //    std::cout << "{";
    //        for(auto elem: this->number){
    //            std::cout << elem << ", ";
    //        }
    //        std::cout << "    base"  << this->BASE << "    }\n";
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

int main(){
    std::string a;
    std::string b;
    std::cin>>a>>b;
    BigInt f(a);
    BigInt s(b);
    std::cout<<log(f);
}
