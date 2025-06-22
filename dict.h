// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;


class Account {
    public:
        string id;
        long long int balance;
};

class Dict{

private:

    // You can add attributes/helper functions here
    void createAccount(std::string id, long long int count);
    int hash(std::string id) ;
    long long int getBalance(string id);
    vector<vector<Account>> bankStorage2d;

public: 
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void addTransaction(std::string id, long long int count) ;

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    long long int get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/    
};