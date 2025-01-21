#include<iostream>
#include<queue>
#include<string>
#include<cstring>
#include<vector>
#include <fstream>
#include <sstream>
using namespace std;

class LexicalAnalyzer{
private:
    string input; // 输入字符
    vector<string> tokens; //输出的tokens
    vector<string> values; //tokens对应的values
    int state; // 当前状态
    int pos; //记录下一个token分析开始的位置
    int cur; // 当前字符的位置指针

public:
    //初始化
    void init(string i){
        input = i + '#'; //加#作为终结符
        tokens.clear();
        values.clear();
        state = 1;
        pos = 0;
        cur = 0;
    }

    bool isLetter(char c){
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }
    bool isZero(char c){
        return c == '0';
    }
    bool isNonZeroDigit(char c){
        return c >= '1' && c <= '9';
    }
    bool isDigit(char c){
        return isNonZeroDigit(c) || isZero(c);
    }
    bool isPoint(char c){
        return c == '.';
    }
    bool isOp(char c){ // 判断是否是运算符
        return c == '+' || c == '-' || c == '*' || c == '=' ||c == '/' || c == '<' || c == '>';
    }
    bool isDlmtr(char c){ // 判断是否是分隔符delimiter
        return c == ',' || c == ';' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}'|| c == '"'|| c == '\'';
    }
    bool isKword(string s){ // 判断是否是关键字keyword
       return s == "if" || s == "else" || s == "while" || s == "for" || s == "return" || s == "break" || s == "continue" || s == "void" || s == "int" 
       || s == "double" || s == "float" || s == "char" || s == "bool" || s == "string" || s == "true" || s == "false" || s == "const";
    }
    char curChar(){
        return input[cur];
    }
    void analysisFail(){
        cout<<"Lexical Analysis Failed For Unknown Lexeme!"<<endl;
        exit(1);

    }
    void printTokens(){
        for(int i = 0; i < tokens.size(); i++){
            cout<<tokens[i]<<" ";
            cout<<values[i]<<endl;
        }
    }

    bool isIdentifier(string s){
        cur = pos;
        state = 1;
        while(1){
            switch(state){
                case 1: // 初始状态
                    if(isLetter(curChar())){
                        cur++;
                        state = 2;
                        break;
                    }
                    else{
                        return false;
                    }
                case 2:
                    if(isLetter(curChar()) || isDigit(curChar())){
                        cur++;
                        break;
                    }
                    else{
                        tokens.push_back("<Identifier>");
                        values.push_back(s.substr(pos, cur-pos));
                        pos = cur;
                        return true;
                    }
            }
        }
    }

    bool isInteger(string s){ //整数
        cur = pos;
        state = 1;
        while(1){
            switch(state){
                case 1: // 初始状态
                    if(isNonZeroDigit(curChar())){
                        state = 3;
                        cur++;
                        break;
                    }
                    else if (isZero(curChar()) ){ //如果为0直接进入终态了
                        cur++;
                        tokens.push_back("<Integer>");
                        values.push_back(s.substr(pos, cur-pos));
                        pos = cur;
                        return true;
                    }
                    else return false;
                case 3: //终态
                    if(isDigit(curChar())){
                        cur++;
                        break;
                    }
                    else{ 
                        tokens.push_back("<Integer>");
                        values.push_back(s.substr(pos, cur-pos));
                        pos = cur;
                        return true;
                    }
            }
        }
    }

    bool isDecimal(string s){
        cur = pos;
        state = 1;
        while(1){
            switch(state){
                case 1: // 初始状态
                    if(isNonZeroDigit(curChar())){
                        state = 5;
                        cur++;
                        break;
                    }
                    else if(isZero(curChar())){
                        state = 4;
                        cur++;
                        break;
                    }
                    else return false;
                case 4 :
                    if(isPoint(curChar())){
                        state = 6;
                        cur++;
                        break;
                    }
                    else return false;
                case 5:
                    if(isDigit(curChar())){
                        cur++;
                        break;
                    }
                    else if(isPoint(curChar())){
                        state = 6;
                        cur++;
                        break;
                    }
                    else return false;
                case 6:
                    if(isDigit(curChar())){
                        state = 7;
                        cur++;
                        break;
                    }
                    else return false;
                case 7: // 终态
                    if(isDigit(curChar())){
                        cur++;
                        break;
                    }
                    else{
                        tokens.push_back("<Decimal>");
                        values.push_back(s.substr(pos, cur-pos));
                        pos = cur;
                        return true;
                    }
            }
        }
    }

    bool isOperator(string s){
        cur = pos;
        state = 1;
        while(1){
            switch(state){
                case 1: // 初始状态
                    if(isOp(curChar())){
                        state = 8;
                        cur++;
                        break;
                    }
                    else return false;
                case 8: // 终态
                    tokens.push_back("<Operator>");
                    values.push_back(s.substr(pos, cur-pos));
                    pos = cur;
                    return true;
            }
        }
    }

    bool isDelimiter(string s){
        cur = pos;
        state = 1;
        while(1){
            switch(state){
                case 1: // 初始状态
                    if(isDlmtr(curChar())){
                        state = 9;
                        cur++;
                        break;
                    }
                    else return false;
                case 9: // 终态
                    tokens.push_back("<Delimiter>");
                    values.push_back(s.substr(pos, cur-pos));
                    pos = cur;
                    return true;
            }
        }
    }

    bool isKeyword(string s){
        cur = pos;
        state = 1;
        while(1){
            switch(state){
                case 1: // 初始状态
                    if(isKword(s)){ //直接用字符串做比较判断是否为关键字
                        state = 10;
                        cur = pos + s.length();
                        break;
                    }
                    else return false;
                case 10: // 终态
                    tokens.push_back("<Keyword>");
                    values.push_back(s);
                    pos = cur;
                    return true;
            }
        }
    }

    bool isSpace(string s){  //似乎没怎么用到 ，因为读取word的时候自动忽略空白字符了。
        cur = pos;
        state = 1;
        while(1){
            switch(state){
                case 1: // 初始状态
                    if(s == " " || s == "\t" || s == "\n" || s == "\r"){
                        state = 11;
                        cur++;
                        break;
                    }
                    else return false;
                case 11: // 终态
                    tokens.push_back("<WhiteSpace>");
                    values.push_back(s);
                    pos = cur;
                    return true; 
            }
        }
    }

    /*bool isAnnotation(string s){
        cur = pos;
        state = 1;
        while(1){
            switch(state){
                case 1: // 初始状态
                    if(s == "/"){
                        state = 12;
                        cur ++;
                        break;
                    }
                    else return false;
                case 12: 
                    if(s == "/") {
                        state = 13;
                        cur = pos+s.length()-1;
                        break;
                    }
                    else return false;
                case 13: 
                    tokens.push_back("<Annotation>");
                    values.push_back(s.substr(pos, cur-pos));
                    pos = cur;
                    return true;
            }
        }
    }*/
    void analyse(string s){
        while(curChar() != '#'){ // 当没读取到终结符时，说明字符串未分析完
            //由于小数的终态是在整数的基础上更深入，根据最长匹配原则，先判断小数，再判断整数；同理keyword也在identifier前面判断
            //使用continue是为了避免多个token间没有空格只识别出第一个token就返回的情况。
            if(isKeyword(s))         continue;
            else if(isIdentifier(s)) continue;
            else if(isDecimal(s))    continue;
            else if(isInteger(s))    continue;
            else if(isOperator(s))   continue;
            else if(isDelimiter(s))  continue;
            else analysisFail();
        }
    }

    void analyseFile(const string& filePath){ //分析代码文件
        LexicalAnalyzer la;
        string line;

        ifstream inputFile(filePath);
        if (!inputFile.is_open()) {
            cout << "无法打开文件: " << filePath << endl;
            return;
        }

        while (getline(inputFile, line)) { // 逐行读取
            istringstream iss(line);
            string word;

            // 逐个读取word，包括空格，对每个word进行词法分析
            while (iss >> word) {
                if(word == "//"){ //如果读取到注释符号，将注释部分赋给word，因为字符流会默认跳过注释内容
                    size_t commentPos = line.find("//");
                    word = string(line.substr(commentPos));
                    la.tokens.clear();
                    la.values.clear();
                    la.tokens.push_back("<Annotation>");
                    la.values.push_back(word);
                    la.printTokens();
                    break;
                }
                else{
                // 处理token，调用词法分析器进行分析
                    la.init(word);
                    la.analyse(word);
                    la.printTokens();
                }
            }
        }

        inputFile.close();
    }
};

int main(){
    LexicalAnalyzer la;
    string input;
    //测试了一下有问题：一个是空格方面的处理，一个是分隔符分号的处理！！！
    string filePath = "test.txt";
    la.analyseFile(filePath);
    /*while(1){
        cout<<"Please input a string:"<<endl;
        cin>>input;
        la.init(input);
        la.analyse(input);
        la.printTokens();
    }*/
    return 0;
}


