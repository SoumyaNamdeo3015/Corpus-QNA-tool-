// Do NOT add any other includes
#include "dict.h"

Dict::Dict(){
    
    bankStorage2d.resize(1000003);   
    // Implement your function here    
}

Dict::~Dict(){
    bankStorage2d.clear();  
} 

int Dict::hash(string id) {
    long sum = 0;
    long x=14641;
    for (char c : id) {
        sum += static_cast<int>(c)*x;
        x=x+11;
    }
    return sum % (1000003); 
}

void Dict::createAccount(string id, long long int count) {
    Account x;
    x.id=id;
    x.balance=count;
    int h=hash(id);
    bankStorage2d[h].push_back(x);
}

void Dict::addTransaction(std::string id, long long int count) {
    int h=hash(id);
    if(bankStorage2d[h].size()!=0){
        for(int i=0;i<bankStorage2d[h].size();i++){
            if(id==bankStorage2d[h][i].id){
                bankStorage2d[h][i].balance+=count;
                return;
            }
        }
        createAccount(id,count);
    }
    else{
        createAccount(id,count);
    }
}

long long int Dict::getBalance(std::string id) {
    int h=hash(id);
    if(bankStorage2d[h].size()!=0){
        for(int i=0;i<bankStorage2d[h].size();i++){
            if(id==bankStorage2d[h][i].id){
                return bankStorage2d[h][i].balance;
            }
        }
    }  
    return 0;
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    string a="";
    for (int i=0; i<sentence.size(); i++){
        if (sentence[i]=='.' ||  sentence[i]==',' ||sentence[i]=='-' ||sentence[i]==':' ||sentence[i]=='!' ||sentence[i]=='[' ||sentence[i]=='(' ||sentence[i]==')' ||sentence[i]==']'||sentence[i]==';'||sentence[i]=='@'||sentence[i]=='?' || sentence[i]==' '|| static_cast<int>(sentence[i])==39 || static_cast<int>(sentence[i])==34){
            if (a!=""){
               
               addTransaction(a,1);
            }
            a="";
        }
        else if (static_cast<int>(sentence[i]) >=65 && static_cast<int>(sentence[i])<=90 ){
            char c= sentence[i]+32;
            a=a+c;
        }
        else{
            a=a+sentence[i];
        }
    }
    if (a!=""){
        
        addTransaction(a,1);
    }
    // Implement your function here  
    return;
}

long long int Dict::get_word_count(string word){
    // Implement your function here  
    return getBalance(word);
}

void Dict::dump_dictionary(string filename){
    
}


// int main(){

//     Dict* mydict = new Dict();
//     mydict->addTransaction("the",23135851162);
//     mydict->addTransaction("on",3750423199);
//     cout << endl;
//     cout << mydict->get_word_count("the");
//     cout << endl;
//     cout << mydict->get_word_count("on");


//     return 0;
// }