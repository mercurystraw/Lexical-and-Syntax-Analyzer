#include<iostream>
#include<string>
#include<stack>
#include<iomanip>
#define M 5
#define N 8

using namespace std;

char VN[M] = {'E', 'M', 'T', 'N', 'F'}; //���ս��
char VT[N] = {'i', '+', '-', '*', '/', '(', ')', '$'};//�ս��
stack<char> analyse_stack; //����ջ
string input; //���봮,�趨Ϊȫ�ֱ���
int i ; //���봮ָ��
string PT[M][N] = { //���������� parsing table
    {"E->TM", "Error", "Error", "Error", "Error", "E->TM", "Error", "Error"},
    {"Error", "M->+TM", "M->-TM", "Error", "Error", "Error", "M->NULL","M->NULL"},
    {"T->FN", "Error", "Error", "Error", "Error", "T->FN", "Error","Error"},
    {"Error", "N->NULL","N->NULL", "N->*FN", "N->/FN", "Error", "N->NULL","N->NULL"},
    {"F->i", "Error", "Error", "Error","Error", "F->(E)", "Error","Error"}
};

int findVN(char c) {  // ���ҷ��ս���� VN �е�����
    for (int i = 0; i < M; i++) {
        if (VN[i] == c) return i;
    }
    return -1;
}
int findVT(char c) { // �����ս���� VT �е�����
    for (int i = 0; i < N; i++) {
        if (VT[i] == c) return i;
    }
    return -1;
}

string formatWithSpaces(const string &s, int width) { //���������width��ȣ����ں�����ӿո�ֱ������
    string result = s;
    while (result.length() < width) {
        result += " ";
    }
    return result.substr(0, width); // ȷ�����ᳬ��ָ�����
}

string printStack(stack<char> s) { // ��ӡ����ջ���
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
    return formatWithSpaces(result, 18); // �̶����Ϊ18
}

string printInput(const string &input, int i) { // ��ӡ���봮
    string result = input.substr(i); // ��ָ��λ�ÿ�ʼ��ȡ
    return formatWithSpaces(result, 18); // �̶����Ϊ18
}

void init(){ // ��ʼ��
    input = input + "$";
    i = 0; //ָ�����봮�ĵ�һ������
    analyse_stack.push('$');
    analyse_stack.push(VN[0]);
    // ��ʼ�� ��ӡ�������̵Ľ���
    cout << "---------------------------------��������---------------------------------------" << endl;
    cout << left;
    cout << setw(10) << "����"
         << setw(20) << "ջ��"
         << setw(20) << "���봮"
         << setw(20) << "����ʽ"
         << setw(20) << "����"
         << endl;
    cout << "--------------------------------------------------------------------------------" << endl;

    cout << setw(10) << "��̬";
    cout << setw(20) << printStack(analyse_stack);
    cout << setw(20) << printInput(input, i);
    cout << setw(20) << "" << "��ʼ��" << endl;
}

void LL1_parser(){
    init();
    //�����������
    for(int n=1; i < input.length(); n++) { //n����ǰ�ķ�������
        char top = analyse_stack.top(); //ջ������
        char cur_input = input[i]; //��ǰ�������

        if(top == cur_input){
            analyse_stack.pop(); //ƥ��ɹ���ջ�����ų�ջ
            i++;

            cout << setw(10) << n;
            cout << setw(20) << printStack(analyse_stack);
            cout << setw(20) << printInput(input, i);
            cout << setw(20) << "" << top << "ƥ��ɹ�" << endl;
        }
        else if(findVN(top)!=-1){
            int vn_index = findVN(top); //ջ�����ս��������
            int vt_index = findVT(cur_input); //��ǰ������ŵ�����
            // ��������������޷�ʶ����ս��
            if (vt_index == -1) {
                cout << "Error: Unexpected character '" << cur_input << "'" << endl;
                return;
            }
            
            string production = PT[vn_index][vt_index]; //���ݷ������ȡ����ʽ
            if (production == "Error") { //����ʽ������ �޷�����
                cout << "Error: No production found for VN '" << top << "' and VT '" << cur_input << "'" << endl;
                return;
            }

            if(production.substr(3) == "NULL"){  //����ʽ�Ҳ�Ϊ��
                //cout<<production<<endl;
                analyse_stack.pop(); //��Ϊ�ǿղ���ʽ��ֱ�ӵ���ջ���ķ��ս��

                cout << setw(10) << n;
                cout << setw(20) << printStack(analyse_stack);
                cout << setw(20) << printInput(input, i);
                cout << setw(20) << production << "�Ƶ��մ�" << endl;
                continue;
            }
            else{
            //�����ж�Ӧ�Ĳ���ʽ
                analyse_stack.pop(); //ջ�����ս����ջ
                //cout<<production<<endl;

                // �Ӳ���ʽ���Ҳ���ʼ�������󲿺ͼ�ͷ������Ϊ3��
                for (int j = production.length() - 1; j >= 3; j--) { 
                    analyse_stack.push(production[j]);
                }
                cout << setw(10) << n;
                cout << setw(20) << printStack(analyse_stack);
                cout << setw(20) << printInput(input, i);
                cout << setw(20) << production << "�Ƶ�" << endl;
            }
        }
        else {
            cout << "Error: Unexpected stack top '" << top << "'" << endl;
            return;
        }
    }
    cout<<"�����ɹ���"<<endl; //��������
}

int main(){
    cout<<"������������ı��ʽ��";
    cin>>input;
    cout<<"����ı��ʽΪ"<<input<<endl;
    LL1_parser();
    return 0;
}


