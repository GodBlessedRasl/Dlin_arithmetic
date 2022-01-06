#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <bits/stdc++.h>
#include <map>
#include<stack>
#include<algorithm>
#include<math.h>
#include<cmath>
#include "ClassDlin.h"
#include<QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::to_poliz(const std::string& expr){
    std::stack<std::string>st;
    std::string cur = "";
    std::string poliz = "";
    for(size_t i = 0;i < expr.size(); ++i) {
        if(expr[i]<='9' && expr[i]>='0'){
            while(i<expr.size() && expr[i]<='9' && expr[i]>='0'){
                cur+=expr[i];
                ++i;
            }
            --i;
            poliz += cur;
            poliz+=" ";
            cur = "";
        }else if((expr[i]<='Z' && expr[i]>='A')||(expr[i]<='z' && expr[i]>='a')){
            while(i<expr.size() && ((expr[i]<='Z' && expr[i]>='A')||(expr[i]<='z' && expr[i]>='a')||expr[i]=='_')){
                cur+=expr[i];
                ++i;
            }
            --i;
            if(cur == "cos"){
                while(st.size() && (st.top() == "cos" || st.top() == "sin" || st.top() == "log")){
                    poliz+=st.top();
                    poliz+=" ";
                    st.pop();
                }
                st.push("cos");
            }else if(cur == "sin"){
                while(st.size() && (st.top() == "cos" || st.top() == "sin" || st.top() == "log")){
                    poliz+=st.top();
                    poliz+=" ";
                    st.pop();
                }
                st.push("sin");
            }else if(cur == "log"){
                while(st.size() && (st.top() == "cos" || st.top() == "sin" || st.top() == "log")){
                    poliz+=st.top();
                    poliz+=" ";
                    st.pop();
                }
                st.push("log");
            }else{
                poliz += cur;
                poliz+=" ";
            }
            cur = "";
        }else if(expr[i]=='-' && (i == 0 || expr[i-1] == '(')){
            while(st.size() && (st.top() == "cos" || st.top() == "sin" || st.top() == "log" || st.top() == "~")){
                poliz+=st.top();
                poliz+=" ";
                st.pop();
            }
            st.push("~");
        }else{
            if(expr[i] == '('){
                st.push("(");
            }else if(expr[i] == '['){
                st.push("[");
            }else if(expr[i] == ')'){
                while(st.top()!="("){
                    poliz+=st.top();
                    poliz+=" ";
                    st.pop();
                }
                st.pop();
            }else if(expr[i] == ']'){
                while(st.top()!="["){
                    poliz+=st.top();
                    poliz+=" ";
                    st.pop();
                }
                st.pop();
            }else if(expr[i] == '+'){
                while(st.size() && st.top()!="(" && st.top()!=")" && st.top()!="[" && st.top()!="]"){
                    poliz+=st.top();
                    poliz+=" ";
                    st.pop();
                }
                st.push("+");
            }else if(expr[i] == '-'){
                while(st.size() && st.top()!="(" && st.top()!=")" && st.top()!="[" && st.top()!="]"){
                    poliz+=st.top();
                    poliz+=" ";
                    st.pop();
                }
                st.push("-");
            }else if(expr[i] == '/'){
                while(st.size() && st.top() != "+" && st.top()
                      != "-" && st.top()!="(" && st.top()!=")" && st.top()!="[" && st.top()!="]"){
                    poliz+=st.top();
                    poliz+=" ";
                    st.pop();
                }
                st.push("/");
            }else if(expr[i] == '*'){
                while(st.size() && st.top() != "+" && st.top() != "-"
                      && st.top()!="(" && st.top()!=")" && st.top()!="[" && st.top()!="]"){
                    poliz+=st.top();
                    poliz+=" ";
                    st.pop();
                }
                st.push("*");
            }else if(expr[i] == '^'){
                while(st.size() && (st.top() == "cos" || st.top() == "sin" || st.top() == "log" || st.top() == "^")){
                    poliz+=st.top();
                    poliz+=" ";
                    st.pop();
                }
                st.push("^");
            }else if(expr[i] == '%'){
                while(st.size() && st.top() != "+" && st.top() != "-"
                      && st.top()!="(" && st.top()!=")" && st.top()!="[" && st.top()!="]"){
                    poliz+=st.top();
                    poliz+=" ";
                    st.pop();
                }
                st.push("%");

            }
        }
    }
    while(st.size()){
        poliz+=st.top();
        poliz+=" ";
        st.pop();
    }
    std::string ans = "";
    for(size_t i = 0;i < poliz.size(); ++i){
        if(poliz[i] == '~'){
            int col=1;
            while(i < poliz.size() && (poliz[i] == '~' || poliz[i] == ' ')){
                ++i;
                if(poliz[i] == '~'){
                    ++col;
                }
            }
            --i;
            if(col%2){
                ans+="~ ";
            }
        }else{
            ans+=poliz[i];
        }
    }
    ui->Poliz->setText(QString::fromStdString(ans));
    qDebug() << "Poliz rabit ";
}

void MainWindow::add_variables(const std::string &expr){
    for(size_t i = 0;i < expr.size(); ++i){
        if((expr[i]<='Z' && expr[i]>='A')||(expr[i]<='z' && expr[i]>='a')){
            std::string cur = "";
            while(i<expr.size() && ((expr[i]<='Z' && expr[i]>='A')||(expr[i]<='z' && expr[i]>='a')||expr[i]=='_')){
                cur+=expr[i];
                ++i;
            }
            --i;
            if(cur!="sin" && cur!="cos" && cur!="log"){
                bool includes = 0;
                for(int j = 0; j < ui->BASE->rowCount(); ++j){
                    if((ui->BASE->item(j, 0)->text()).toStdString() == cur){
                        includes = 1;
                        break;
                    }
                }
                if(!includes){
                    ui->BASE->insertRow(ui->BASE->rowCount());
                    ui->BASE->setItem(ui->BASE->rowCount()-1, 0,
                                      new QTableWidgetItem(QString::fromStdString(cur)));
                }
            }
        }
    }
    qDebug() << "ADD Variables rabit ";
}
void MainWindow::on_ENTER_clicked(){
    if(Check(ui->Expression->text().toStdString())){
        add_variables((ui->Expression->text()).toStdString());
        to_poliz((ui->Expression->text()).toStdString());
    }
    ui->Expression->setText(QString::fromStdString(""));
    qDebug() << "Enter rabit ";
}


bool MainWindow::Check(std::string s) {
    if (s == "") {
        ui->ERRORS->setText(QString::fromStdString("ERROR: Пустой ввод"));
        return 0;
    }
    try {
        for (size_t i = 1; i < s.size(); ++i) {
            if ((s[i] == '-' || s[i] == '+' || s[i] == '^' || s[i] == '%' || s[i] == '*' || s[i] == '/') &&
                (s[i - 1] == '-' || s[i - 1] == '+' || s[i - 1] == '^' || s[i - 1] == '%' || s[i - 1] == '*' || s[i - 1] == '/')) {
                std::string thrw = "ERROR: две операции подряд";
                throw thrw;
            }
        }
        if (s[0] == '-') {
            std::reverse(s.begin(), s.end());
            s.pop_back();
            std::reverse(s.begin(), s.end());
            return Check(s);
        } else if (s[0] >= '0' && s[0] <= '9') {
            size_t i = 0;
            while (s[i] >= '0' && s[i] <= '9') {
                ++i;
                if (i == s.size()) {
                    break;
                }
            }
            if (i == s.size()) {
                ui->ERRORS->setText(QString::fromStdString("EVERYTHING IS OKAY"));
                return 1;
            } else {
                if (s[i] == '-' || s[i] == '+' || s[i] == '^' || s[i] == '%' || s[i] == '*' || s[i] == '/') {
                    ++i;
                    std::string ans_ = "";
                    for(;i < s.size(); ++i) {
                        ans_ += s[i];
                    }
                    return Check(ans_);
                } else {
                    std::string thrw = "ERROR: недопустимый символ в числе";
                    throw thrw;
                }
            }
        } else if (s[0] == '(') {
            size_t i = 1;
            int skobki = 1;
            std::string next_1 = "";
            bool done = false;
            for (; i < s.size(); ++i) {
                if (s[i] == '(') {
                    ++skobki;
                } else if (s[i] == ')') {
                    --skobki;
                }
                if (skobki == 0) {
                    done = true;
                    if (!Check(next_1)) {
                        return 0;
                    }
                    break;
                }
                next_1 += s[i];
            }
            if (!done) {
                std::string thrw = "ERROR: некорректная последовательность скобок";
                throw thrw;
            } else if (done && Check(next_1)) {
                ++i;
                if (i == s.size()) {
                    ui->ERRORS->setText(QString::fromStdString("EVERYTHING IS OKAY"));
                    return 1;
                }
                if (s[i] == '-' || s[i] == '+' || s[i] == '^' || s[i] == '%' || s[i] == '*' || s[i] == '/') {
                    ++i;
                    std::string ans_ = "";
                    for(;i < s.size(); ++i) {
                        ans_ += s[i];
                    }
                    return Check(ans_);
                } else {
                    std::string thrw = "ERROR: неопознанный символ";
                    throw thrw;
                }
            }
        } else if (s[0] == '[') {
            size_t i = 1;
            int skobki = 1;
            std::string next_1 = "";
            bool done = false;
            for (; i < s.size(); ++i) {
                if (s[i] == '[') {
                    ++skobki;
                } else if (s[i] == '[') {
                    --skobki;
                }
                if (skobki == 0) {
                    done = true;
                    if (!Check(next_1)) {
                        return 0;
                    }
                    break;
                }
                next_1 += s[i];
            }
            if (!done) {
                std::string thrw = "ERROR: некорректная последовательность скобок";
                throw thrw;
            } else if (done && Check(next_1)) {
                ++i;
                if (i == s.size()) {
                    ui->ERRORS->setText(QString::fromStdString("EVERYTHING IS OKAY"));
                    return 1;
                }
                if (s[i] == '-' || s[i] == '+' || s[i] == '^' || s[i] == '%' || s[i] == '*' || s[i] == '/') {
                    ++i;
                    std::string ans_ = "";
                    for(;i < s.size(); ++i) {
                        ans_ += s[i];
                    }
                    return Check(ans_);
                } else {
                    std::string thrw = "ERROR: неопознанный символ";
                    throw thrw;
                }
            }
        } else if (s.size() >= 6) {
            if (s[0] == 's' && s[1] == 'i' && s[2] == 'n' && s[3] == '(') {
                size_t i = 4;
                int skobki = 1;
                std::string next_1 = "";
                bool done = false;
                for (; i < s.size(); ++i) {
                    if (s[i] == '(') {
                        ++skobki;
                    } else if (s[i] == ')') {
                        --skobki;
                    }
                    if (skobki == 0) {
                        done = true;
                        if (!Check(next_1)) {
                            return 0;
                        }
                        break;
                    }
                    next_1 += s[i];
                }
                if (!done) {
                    std::string thrw = "ERROR: некорректная последовательность скобок";
                    throw thrw;
                } else if (done && Check(next_1)) {
                    ++i;
                    if (i == s.size()) {
                        ui->ERRORS->setText(QString::fromStdString("EVERYTHING IS OKAY"));
                        return 1;
                    }
                    if (s[i] == '-' || s[i] == '+' || s[i] == '^' || s[i] == '%' || s[i] == '*' || s[i] == '/') {
                        ++i;
                        std::string ans_ = "";
                        for(;i < s.size(); ++i) {
                            ans_ += s[i];
                        }
                        return Check(ans_);
                    } else {
                        std::string thrw = "ERROR: неопознанный символ";
                        throw thrw;
                    }
                }
            } else if (s[0] == 's' && s[1] == 'i' && s[2] == 'n' && s[3] == '[') {
                size_t i = 4;
                int skobki = 1;
                std::string next_1 = "";
                bool done = false;
                for (; i < s.size(); ++i) {
                    if (s[i] == '[') {
                        ++skobki;
                    } else if (s[i] == ']') {
                        --skobki;
                    }
                    if (skobki == 0) {
                        done = true;
                        if (!Check(next_1)) {
                            return 0;
                        }
                        break;
                    }
                    next_1 += s[i];
                }
                if (!done) {
                    std::string thrw = "ERROR: неправильная последовательность скобок";
                    throw thrw;
                } else if (done && Check(next_1)) {
                    ++i;
                    if (i == s.size()) {
                        ui->ERRORS->setText(QString::fromStdString("EVERYTHING IS OKAY"));
                        return 1;
                    }
                    if (s[i] == '-' || s[i] == '+' || s[i] == '^' || s[i] == '%' || s[i] == '*' || s[i] == '/') {
                        ++i;
                        std::string ans_ = "";
                        for(;i < s.size(); ++i) {
                            ans_ += s[i];
                        }
                        return Check(ans_);
                    } else {
                        std::string thrw = "ERROR: неопознанный символ";
                        throw thrw;
                    }
                }
            } else if (s[0] == 'c' && s[1] == 'o' && s[2] == 's' && s[3] == '(') {
                size_t i = 4;
                int skobki = 1;
                std::string next_1 = "";
                bool done = false;
                for (; i < s.size(); ++i) {
                    if (s[i] == '(') {
                        ++skobki;
                    } else if (s[i] == ')') {
                        --skobki;
                    }
                    if (skobki == 0) {
                        done = true;
                        if (!Check(next_1)) {
                            return 0;
                        }
                        break;
                    }
                    next_1 += s[i];
                }
                if (!done) {
                    std::string thrw = "ERROR: неправильная последовательность скобок";
                    throw thrw;
                } else if (done && Check(next_1)) {
                    ++i;
                    if (i == s.size()) {
                        ui->ERRORS->setText(QString::fromStdString("EVERYTHING IS OKAY"));
                        return 1;
                    }
                    if (s[i] == '-' || s[i] == '+' || s[i] == '^' || s[i] == '%' || s[i] == '*' || s[i] == '/') {
                        ++i;
                        std::string ans_ = "";
                        for(;i < s.size(); ++i) {
                            ans_ += s[i];
                        }
                        return Check(ans_);
                    } else {
                        std::string thrw = "ERROR: неопознанный символ";
                        throw thrw;
                    }
                }
            } else if (s[0] == 'c' && s[1] == 'o' && s[2] == 's' && s[3] == '[') {
                size_t i = 4;
                int skobki = 1;
                std::string next_1 = "";
                bool done = false;
                for (; i < s.size(); ++i) {
                    if (s[i] == '(') {
                        ++skobki;
                    } else if (s[i] == ')') {
                        --skobki;
                    }
                    if (skobki == 0) {
                        done = true;
                        if (!Check(next_1)) {
                            return 0;
                        }
                        break;
                    }
                    next_1 += s[i];
                }
                if (!done) {
                    std::string thrw = "ERROR: неправильная последовательность скобок";
                    throw thrw;
                } else if (done && Check(next_1)) {
                    ++i;
                    if (i == s.size()) {
                        ui->ERRORS->setText(QString::fromStdString("EVERYTHING IS OKAY"));
                        return 1;
                    }
                    if (s[i] == '-' || s[i] == '+' || s[i] == '^' || s[i] == '%' || s[i] == '*' || s[i] == '/') {
                        ++i;
                        std::string ans_ = "";
                        for(;i < s.size(); ++i) {
                            ans_ += s[i];
                        }
                        return Check(ans_);
                    } else {
                        std::string thrw = "ERROR: неопознанный символ";
                        throw thrw;
                    }
                }
            } else if (s[0] == 'l' && s[1] == 'o' && s[2] == 'g' && s[3] == '(') {
                size_t i = 4;
                std::string next_1 = "";
                bool done = false;
                int brackets = 1;
                for(;i < s.size(); ++i) {
                    if (s[i] == ',' && brackets == 1) {
                        if (!Check(next_1)) {
                            return 0;
                        }
                        done = true;
                        break;
                    }
                    if (s[i] == '(') {
                        brackets += 1;
                    }
                    if (s[i] == ')') {
                        brackets -= 1;
                    }
                    next_1 += s[i];
                }
                if (i == s.size()) {
                    std::string thrw = "ERROR";
                    throw thrw;
                }
                if (!done) {
                    std::string thrw = "ERROR: отсутствует запятая между аргументами";
                    throw thrw;
                }
                if (done && Check(next_1)) {
                    ++i;
                    int skobki = 1;
                    std::string next_1 = "";
                    bool done = false;
                    for (; i < s.size(); ++i) {
                        if (s[i] == '(') {
                            ++skobki;
                        } else if (s[i] == ')') {
                            --skobki;
                        }
                        if (skobki == 0) {
                            done = true;
                            if (!Check(next_1)) {
                                return 0;
                            }
                            break;
                        }
                        next_1 += s[i];
                    }
                    if (!done) {
                        std::string thrw = "ERROR: неправильная последовательность скобок";
                        throw thrw;
                    } else if (done && Check(next_1)) {
                        ++i;
                        if (i == s.size()) {
                            ui->ERRORS->setText(QString::fromStdString("EVERYTHING IS OKAY"));
                            return 1;
                        }
                        if (s[i] == '-' || s[i] == '+' || s[i] == '^' || s[i] == '%' || s[i] == '*' || s[i] == '/') {
                            ++i;
                            std::string ans_ = "";
                            for(;i < s.size(); ++i) {
                                ans_ += s[i];
                            }
                            return Check(ans_);
                        } else {
                            std::string thrw = "ERROR: неопознанный символ";
                            throw thrw;
                        }
                    }
                }
            } else if (s[0] == 'l' && s[1] == 'o' && s[2] == 'g' && s[3] == '[') {
                size_t i = 4;
                std::string next_1 = "";
                bool done = false;
                int brackets = 1;
                for(;i < s.size(); ++i) {
                    if (s[i] == ',' && brackets == 1) {
                        if (!Check(next_1)) {
                            return 0;
                        }
                        done = true;
                        break;
                    }
                    if (s[i] == '[') {
                        brackets += 1;
                    }
                    if (s[i] == ']') {
                        brackets -= 1;
                    }
                    next_1 += s[i];
                }
                if (i == s.size()) {
                    std::string thrw = "ERROR";
                    throw thrw;
                }
                if (!done) {
                    std::string thrw = "ERROR: нет запятой между аргументами";
                    throw thrw;
                }
                if (done && Check(next_1)) {
                    ++i;
                    int skobki = 1;
                    std::string next_1 = "";
                    bool done = false;
                    for (; i < s.size(); ++i) {
                        if (s[i] == '[') {
                            ++skobki;
                        } else if (s[i] == ']') {
                            --skobki;
                        }
                        if (skobki == 0) {
                            done = true;
                            if (!Check(next_1)) {
                                return 0;
                            }
                            break;
                        }
                        next_1 += s[i];
                    }
                    if (!done) {
                        std::string thrw = "ERROR: неправильная последовательность скобок";
                        throw thrw;
                    } else if (done && Check(next_1)) {
                        ++i;
                        if (i == s.size()) {
                            ui->ERRORS->setText(QString::fromStdString("EVERYTHING IS OKAY"));
                            return 1;
                        }
                        if (s[i] == '-' || s[i] == '+' || s[i] == '^' || s[i] == '%' || s[i] == '*' || s[i] == '/') {
                            ++i;
                            std::string ans_ = "";
                            for(;i < s.size(); ++i) {
                                ans_ += s[i];
                            }
                            return Check(ans_);
                        } else {
                            std::string thrw = "ERROR: неопознанный символ";
                            throw thrw;
                        }
                    }
                }
            } else if ((s[0] >= 'a' && s[0] <= 'z') || (s[0] >= 'A' && s[0] <= 'Z')) {
                size_t i = 0;
                while ((s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') || s[i] == '_') {
                    ++i;
                    if (i == s.size()) {
                        break;
                    }
                }
                if (i == s.size()) {
                    ui->ERRORS->setText(QString::fromStdString("EVERYTHING IS OKAY"));
                    return 1;
                } else {
                    if (s[i] == '-' || s[i] == '+' || s[i] == '^' || s[i] == '%' || s[i] == '*' || s[i] == '/') {
                        ++i;
                        std::string ans_ = "";
                        for(;i < s.size(); ++i) {
                            ans_ += s[i];
                        }
                        return Check(ans_);
                    } else {
                        std::string thrw = "ERROR: неопознанный символ";
                        throw thrw;
                    }
                }
            } else {
                std::string thrw = "ERROR: неопознанный символ";
                throw thrw;
            }
        } else {
            if ((s[0] >= 'a' && s[0] <= 'z') || (s[0] >= 'A' && s[0] <= 'Z')) {
                size_t i = 0;
                while ((s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') || s[i] == '_') {
                    ++i;
                    if (i == s.size()) {
                        break;
                    }
                }
                if (i == s.size()) {
                    ui->ERRORS->setText(QString::fromStdString("EVERYTHING IS OKAY"));
                    return 1;
                } else {
                    if (s[i] == '-' || s[i] == '+' || s[i] == '^' || s[i] == '%' || s[i] == '*' || s[i] == '/') {
                        ++i;
                        std::string ans_ = "";
                        for(;i < s.size(); ++i) {
                            ans_ += s[i];
                        }
                        return Check(ans_);
                    } else {
                        std::string thrw = "ERROR: неопознанный символ";
                        throw thrw;
                    }
                }
            } else {
                std::string thrw = "ERROR: неопознанный символ";
                throw thrw;
            }
        }
    } catch (std::string s_) {
        ui->ERRORS->setText(QString::fromStdString("" + s_));
        return 0;
    }
    ui->ERRORS->setText(QString::fromStdString("EVERYTHING IS OKAY"));
    return 1;
}


void MainWindow::on_LOAD_clicked()
{
    std::map<std::string, std::string>mp;
    bool anyth = 1;
    for(size_t i = 0; i < ui->BASE->rowCount(); ++i){
        if(ui->BASE->item(i, 1)->text().toStdString().size() == 0){
            anyth = 0;
        }else{
            mp[ui->BASE->item(i, 0)->text().toStdString()] = ui->BASE->item(i, 1)->text().toStdString();
        }
    }
    qDebug() << "OK1 ";
    if(anyth){
        std::string expr = ui->Poliz->text().toStdString();
        std::stack<BigInt>st;
        std::string cur = "";
        for(int i = 0;i < (int)expr.size(); ++i) {
            if(expr[i]<='9' && expr[i]>='0'){
                while(i<(int)expr.size() && expr[i]<='9' && expr[i]>='0'){
                    cur+=expr[i];
                    ++i;
                }
                --i;
                st.push(BigInt(cur))
;                cur = "";
            }else if((expr[i]<='Z' && expr[i]>='A')||(expr[i]<='z' && expr[i]>='a')){
                while((int)i<expr.size() && ((expr[i]<='Z' && expr[i]>='A')||(expr[i]<='z' && expr[i]>='a')||expr[i]=='_')){
                    cur+=expr[i];
                    ++i;
                }
                --i;
                if(cur == "cos"){
                    BigInt now = st.top();
                    st.pop();
                    st.push(BigInt(cos(now)));
                }else if(cur == "sin"){
                    BigInt now = st.top();
                    st.pop();
                    st.push(BigInt(sin(now)));
                }else if(cur == "log"){
                    BigInt a = st.top();
                    st.pop();
                    BigInt b = st.top();
                    st.pop();
                    st.push(a+b);
                    st.push(BigInt(log(b,a)));
                }else{
                    st.push(BigInt(mp[cur]));
                }
                cur = "";
            }else if(expr[i] == '~'){
                BigInt now = st.top();
                st.pop();
                st.push(-now);
            }else{
                if(expr[i] == '+'){
                    BigInt a = st.top();
                    st.pop();
                    BigInt b = st.top();
                    st.pop();
                    st.push(a+b);
                }else if(expr[i] == '-'){
                    BigInt a = st.top();
                    st.pop();
                    BigInt b = st.top();
                    st.pop();
                    st.push(b-a);
                }else if(expr[i] == '/'){
                    BigInt a = st.top();
                    st.pop();
                    BigInt b = st.top();
                    st.pop();
                    st.push(b/a);
                }else if(expr[i] == '*'){
                    BigInt a = st.top();
                    st.pop();
                    BigInt b = st.top();
                    st.pop();
                    st.push(a*b);
                }else if(expr[i] == '^'){
                    BigInt a = st.top();
                    st.pop();
                    BigInt b = st.top();
                    st.pop();
                    st.push(b^(a.to_int()));
                }else if(expr[i] == '%'){
                    BigInt a = st.top();
                    st.pop();
                    BigInt b = st.top();
                    st.pop();
                    st.push(b%a);
                }else{
                    continue;
                }
            }
        }
        qDebug() << "Load rabit ";
        ui->Expression->setText(QString::fromStdString(st.top().tostr()));
        ui->BASE->setRowCount(0);
        qDebug() << "Load rabit ";
    }
}

void MainWindow::on_CLEAR_clicked()
{
        ui->BASE->setRowCount(0);
        ui->Expression->setText("");
        ui->Poliz->setText("");
        ui->ERRORS->setText(QString::fromStdString("EVERYTHING IS OK"));
}
