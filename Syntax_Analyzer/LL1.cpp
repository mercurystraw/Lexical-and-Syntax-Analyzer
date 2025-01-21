#include<iostream>
#include<string>
#include<stack>
#include<iomanip>
#define M 5
#define N 8

using namespace std;

char VN[M] = {'E', 'M', 'T', 'N', 'F'}; //非终结符
char VT[N] = {'i', '+', '-', '*', '/', '(', ')', '$'};//终结符
stack<char> analyse_stack; //分析栈
string input; //输入串,设定为全局变量
int i ; //输入串指针
string PT[M][N] = { //构建分析表 parsing table
    {"E->TM", "Error", "Error", "Error", "Error", "E->TM", "Error", "Error"},
    {"Error", "M->+TM", "M->-TM", "Error", "Error", "Error", "M->NULL","M->NULL"},
    {"T->FN", "Error", "Error", "Error", "Error", "T->FN", "Error","Error"},
    {"Error", "N->NULL","N->NULL", "N->*FN", "N->/FN", "Error", "N->NULL","N->NULL"},
    {"F->i", "Error", "Error", "Error","Error", "F->(E)", "Error","Error"}
};

int findVN(char c) {  // 查找非终结符在 VN 中的索引
    for (int i = 0; i < M; i++) {
        if (VN[i] == c) return i;
    }
    return -1;
}
int findVT(char c) { // 查找终结符在 VT 中的索引
    for (int i = 0; i < N; i++) {
        if (VT[i] == c) return i;
    }
    return -1;
}

string formatWithSpaces(const string &s, int width) { //如果不满足width宽度，就在后面添加空格直至满足
    string result = s;
    while (result.length() < width) {
        result += " ";
    }
    return result.substr(0, width); // 确保不会超过指定宽度
}

string printStack(stack<char> s) { // 打印分析栈情况
    stack<char> temp;
    string result;
    while (!s.empty()) {
        temp.push(s.top());
        s.pop();
    }
    while (!temp.empty()) {
        result += temp.top();
        result += " ";
        temp.pop();
    }
    return formatWithSpaces(result, 18); // 固定宽度为18
}

string printInput(const string &input, int i) { // 打印输入串
    string result = input.substr(i); // 从指针位置开始截取
    return formatWithSpaces(result, 18); // 固定宽度为18
}

void init(){ // 初始化
    input = input + "$";
    i = 0; //指向输入串的第一个符号
    analyse_stack.push('$');
    analyse_stack.push(VN[0]);
    // 初始化 打印分析过程的界面
    cout << "---------------------------------分析过程---------------------------------------" << endl;
    cout << left;
    cout << setw(10) << "步骤"
         << setw(20) << "栈内"
         << setw(20) << "输入串"
         << setw(20) << "产生式"
         << setw(20) << "动作"
         << endl;
    cout << "--------------------------------------------------------------------------------" << endl;

    cout << setw(10) << "初态";
    cout << setw(20) << printStack(analyse_stack);
    cout << setw(20) << printInput(input, i);
    cout << setw(20) << "" << "初始化" << endl;
}

void LL1_parser(){
    init();
    //具体分析过程
    for(int n=1; i < input.length(); n++) { //n代表当前的分析轮数
        char top = analyse_stack.top(); //栈顶符号
        char cur_input = input[i]; //当前输入符号

        if(top == cur_input){
            analyse_stack.pop(); //匹配成功，栈顶符号出栈
            i++;

            cout << setw(10) << n;
            cout << setw(20) << printStack(analyse_stack);
            cout << setw(20) << printInput(input, i);
            cout << setw(20) << "" << top << "匹配成功" << endl;
        }
        else if(findVN(top)!=-1){
            int vn_index = findVN(top); //栈顶非终结符的索引
            int vt_index = findVT(cur_input); //当前输入符号的索引
            // 处理特殊情况，无法识别的终结符
            if (vt_index == -1) {
                cout << "Error: Unexpected character '" << cur_input << "'" << endl;
                return;
            }
            
            string production = PT[vn_index][vt_index]; //根据分析表获取产生式
            if (production == "Error") { //产生式不存在 无法分析
                cout << "Error: No production found for VN '" << top << "' and VT '" << cur_input << "'" << endl;
                return;
            }

            if(production.substr(3) == "NULL"){  //产生式右部为空
                //cout<<production<<endl;
                analyse_stack.pop(); //因为是空产生式，直接弹出栈顶的非终结符

                cout << setw(10) << n;
                cout << setw(20) << printStack(analyse_stack);
                cout << setw(20) << printInput(input, i);
                cout << setw(20) << production << "推导空串" << endl;
                continue;
            }
            else{
            //否则有对应的产生式
                analyse_stack.pop(); //栈顶非终结符出栈
                //cout<<production<<endl;

                // 从产生式的右部开始（跳过左部和箭头，长度为3）
                for (int j = production.length() - 1; j >= 3; j--) { 
                    analyse_stack.push(production[j]);
                }
                cout << setw(10) << n;
                cout << setw(20) << printStack(analyse_stack);
                cout << setw(20) << printInput(input, i);
                cout << setw(20) << production << "推导" << endl;
            }
        }
        else {
            cout << "Error: Unexpected stack top '" << top << "'" << endl;
            return;
        }
    }
    cout<<"分析成功！"<<endl; //分析结束
}

int main(){
    cout<<"请输入待分析的表达式：";
    cin>>input;
    cout<<"输入的表达式为"<<input<<endl;
    LL1_parser();
    return 0;
}


