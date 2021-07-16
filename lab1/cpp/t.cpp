#include<iostream>
#include<regex>

using namespace std;

int main(){
    regex recv_time_expr(".*sendtime=(.*?)");
    std::smatch t;
    while(1){
        string s;
        getline(cin,s);
        regex_match(s,t,recv_time_expr);
        cout<<"match size "<<t.size()<<endl;
        for(auto i:t){
            cout<<i<<endl;
        }
        cout<<"find end"<<endl;
    }
}