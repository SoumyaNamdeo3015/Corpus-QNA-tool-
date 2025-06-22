#pragma once
#include <iostream>
#include <fstream>
#include "Node.h"
#include "dict.h"
#include "search.h"
#include<chrono>

using namespace std;

class Data{

    public:
    int page_no, book_code, para_no;
    float score;
    bool ispresent;
    vector<int>wordvector;

    Data(int bkcode, int pg, int para){
        book_code = bkcode;
        page_no = pg;
        para_no = para;
        score = 0;
        ispresent = false;
    }

    
    void pirntData(){
        cout << "[" << book_code << "," << page_no <<"," <<para_no << ","<<score << "]"<< endl;
    }

    vector<int> getWordvector(){
        return wordvector;
    }

    ~Data(){};

};


class Element{

    public:
    string word;
    vector<Data*>vec;

    Element(){
        word = "";
    };

    Element(string w){
        word = w;
    }

    ~Element(){
        //write the destructor here
    }
};



static vector<int>mergeVector(vector<int>&v1, vector<int>&v2){

    vector<int>ans;
    int i=0;
    int j=0;
    while(i<v1.size() && j<v2.size()){

        if(v1[i]<v2[j]){
            ans.push_back(v1[i]);
            i++;
        }

        else if(v1[i]== v2[j]){
            ans.push_back(v1[i]);
            i++;
            j++;
        }

        else{
            ans.push_back(v2[j]);
            j++;
            }

        }

        while(i<v1.size()){
            ans.push_back(v1[i]);
            i++;
        }
    
        while(j<v2.size()){
            ans.push_back(v2[j]);
            j++;
        }
        return ans;
}






class HubNode{

    public:
    float hubscore;
    vector<Data*>datalist;
    vector<int>hubwordvector;

    HubNode(){
        hubscore = 0;
    }

    void insertHubdata(Data*d){
        hubscore += d->score;
        datalist.push_back(d);
        hubwordvector = mergeVector(hubwordvector,d->wordvector);
    }
    ~HubNode(){
    }


};


class Algorithms{

    public:

    //generates word list // can we optimize these ? 
    static vector<string>generateWordList(string sentence){

    vector<string> result;
    string a="";
    for (int i=0; i<sentence.size(); i++){
        if (sentence[i]=='.' ||  sentence[i]==',' ||sentence[i]=='-' ||sentence[i]==':' ||sentence[i]=='!' ||sentence[i]=='[' ||sentence[i]=='(' ||sentence[i]==')' ||sentence[i]==']'||sentence[i]==';'||sentence[i]=='@'||sentence[i]=='?' || sentence[i]==' '|| static_cast<int>(sentence[i])==39 || static_cast<int>(sentence[i])==34){
            if (a!=""){
               
               result.push_back(a);
            }
            a="";
        }
        else if (static_cast<int> (sentence[i]) >=65 && static_cast<int> (sentence[i])<=90 ){
            char c= sentence[i]+32;
            a=a+c;
        }
        else{
            a=a+sentence[i];
        }
    }
    if (a!=""){
        
        result.push_back(a);
    }
    // Implement your function here  
    return result;
    
    }

    //hash function for words  // can we create  a 
    //good hash function
    
    static int getHashcode(string word){        
    int code = 1;
    for(char i : word){
        code = (int(i) * ((code * 31 + int(i))% 1000000)) % 1000000 ;
    }
        return code;

    }

  
    //hash function for elements
    static int triplehash(int a, int b, int c, int&m){
        return (103*a+ b*107+c*109)%m;
    }
    
    //|x-y|
    static int differnce(int x, int y){
        if(x>y){

            return x-y;
        }
        return y-x;
    }

    //merge for sorting paragraphs on the basis of their scores:
    static void merge(std::vector<Data*>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;
        std::vector<Data*> leftSubarray;
        std::vector<Data*> rightSubarray;
        for (int i = 0; i < n1; i++) {
            leftSubarray.push_back(arr[left + i]);
        }
        for (int i = 0; i < n2; i++) {
            rightSubarray.push_back(arr[mid + 1 + i]);
        }

        int i = 0;
        int j = 0;
        int k = left; 

        while (i < n1 && j < n2) {
            if (leftSubarray[i]->score >= rightSubarray[j]->score) {
                arr[k] = leftSubarray[i];
                i++;
            } else {
                arr[k] = rightSubarray[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            arr[k] = leftSubarray[i];
            i++;
            k++;
        }
        while (j < n2) {
            arr[k] = rightSubarray[j];
            j++;
            k++;
        }
    }

    //mergesort for sorting paragraphs on the basis of their scores:
    static void mergeSort(std::vector<Data*>& arr, int left, int right){
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }


    //mergesort forhubNode:
    static void mergeHUB(vector<HubNode*>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        // Create temporary subarrays
        vector<HubNode*> leftSubarray;
        vector<HubNode*> rightSubarray;

        for (int i = 0; i < n1; i++) {
            leftSubarray.push_back(arr[left + i]);
        }

        for (int i = 0; i < n2; i++) {
            rightSubarray.push_back(arr[mid + 1 + i]);
        }

        int i = 0;
        int j = 0;
        int k = left; 

        while (i < n1 && j < n2) {
            if (leftSubarray[i]->hubscore >= rightSubarray[j]->hubscore) {
                arr[k] = leftSubarray[i];
                i++;
            } else {
                arr[k] = rightSubarray[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            arr[k] = leftSubarray[i];
            i++;
            k++;
        }

        while (j < n2) {
            arr[k] = rightSubarray[j];
            j++;
            k++;
        }
    }


    static bool compare(Data* d1, Data* d2){

        if(d1->book_code == d2->book_code){
            if(d1->page_no <= d2->page_no){
                return true;
            }
            return false;
        }

        else if(d1->book_code <= d2->book_code){
            return true;
        }

        return false;
    }


    static void mergeHUBSort(std::vector<HubNode*>& arr, int left, int right){
        if (left < right){
            int mid = left + (right - left) / 2;
            mergeHUBSort(arr, left, mid);
            mergeHUBSort(arr, mid + 1, right);
            mergeHUB(arr, left, mid, right);
        }
    }

    // mrege sort function for data but in different way:
    static void mergeDifferent(vector<Data*>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        vector<Data*> leftSubarray;
        vector<Data*> rightSubarray;

        for (int i = 0; i < n1; i++) {
            leftSubarray.push_back(arr[left + i]);
        }

        for (int i = 0; i < n2; i++) {
            rightSubarray.push_back(arr[mid + 1 + i]);
        }

        int i = 0;
        int j = 0;
        int k = left; 

        while (i < n1 && j < n2) {
            if (!compare(leftSubarray[i],rightSubarray[j])) {
                arr[k] = leftSubarray[i];
                i++;
            } else {
                arr[k] = rightSubarray[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            arr[k] = leftSubarray[i];
            i++;
            k++;
        }

        while (j < n2) {
            arr[k] = rightSubarray[j];
            j++;
            k++;
        }
    }

    static void mergeSortdifferent(std::vector<Data*>& arr, int left, int right){
        if (left < right){
            int mid = left + (right - left) / 2;
            mergeSortdifferent(arr, left, mid);
            mergeSortdifferent(arr, mid + 1, right);
            mergeDifferent(arr, left, mid, right);
        }
    }

    static bool isPresent(string& word, vector<string>&stop_words){
        for(int i=0;i<stop_words.size();i++){
            if(word == stop_words[i]){
                return true;
            }
        }
        return false;
    }

    static vector<string> modifyQuery(string question){

        vector<string>stop_words{
        "i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your", "yours", "yourself", "yourselves", "he", "him", "his", "himself", "she", "her", "hers", "herself", "it", "its", "itself", "they", "them", "their", "theirs", "themselves", "what", "which", "who", "whom","this", "that", "these", "those", "am", "is", "are", "was", "were", "be", 
        "been", "being", "have", "has", "had", "having", "do", "does", "did", "doing", "a", "an", "the", "and", "but", "if", "or", "because", "as", "until", "while", "of", "at", "by", "for", "with", "about", "against", "between", "into", "through", "during", 
        "before", "after", "above", "below", "to", "from", "up", "down", "in", "out", "on", "off", "over", "under", "again", "further","describe", "then", "once", "here", "there", "when", "beyond","later","respond""react","where", "why", "how", "all", "any", "both", "each", "few", "more", "most", "other", "some", "such", "no", "nor", "not", "only", "own", "same", "so", "than", "too", "very", "s", "t", "can", "will", "just", "should", "should've", "now", "d", "ll", "m", "o", "re", "ve", "y", "ain", "were","like","experiences","influence","major","events","time","concept","name","some","contribute","contributes","main","objective","objectives","impact","views","key","choice","choices","trajectory","circumstances","surrounding", "surroundings","understanding","remembered","ma", "explain","show","tell","specific", "generally", "particular", "basically", "essentially", "ultimately", "primarily", "fundamentally", "significantly", "importantly", "notably","consequently", "therefore", "moreover", "nonetheless", "nevertheless", "furthermore", "meanwhile", "subsequently", "initially", "previously", "currently", "traditionally", "consistently", "occasionally", "frequently", "specifically", "generally", "relatively", "completely", "significance","entirely", "thoroughly", "inherently", "inherently", "notably", "significantly", "considerably", "primarily", "predominantly", "substantially", "exceptionally", "occasionally",  "frequently", "ultimately", "inherently", "noticeably", "notably", "significantly", "substantially", "particularly", "fundamentally", "essentially", "basically","mostly", "necessarily", "never", "normally", "obviously", "often", "overall", "partly", "possibly", "practically", "presumably", "previously", "primarily", "probably", "promptly", "properly", "quickly", "quietly", "rarely", "readily", "really", "recently", "regularly", "relatively", "right", "roughly", "seldom", "seriously", "significantly", "similarly", "simply", "slightly", "slowly", "so", "sometimes", "somewhat","effect","effects","side", "soon", "specifically", "strongly", "surely", "surprisingly", "temporarily", "thereby", "thus", "together", "totally", "truly", "typically", "ultimately", "usually", "virtually", "well", "widely",  "nevertheless", "therefore", "nonetheless", "significantly", "consequently", "primarily", "predominantly","substantially", "exceptionally", "occasionally", "fundamentally", "essentially", "mostly", "considerably","inherently", "noticeably", "partly", "obviously", 
        "temporarily", "regularly", "virtually","consequences","consequence","largely","chiefly","mainly","extensively", "profoundly","exceedingly","sporadically","intermittently","periodically","steadily","steadfastly","persistently","steadily","perpetually","infrequently","infrequently","constantly","continuously", "habitually","routinely","conclusively","unequivocally","decisively","explicitly","implicitly","concretely","tangibly","unquestionably","undeniably","incontestably","indubitably","certainly","assuredly","definitely","definitively","unmistakably","unambiguously","unequivocally","emphatically","vividly","lucidly","palpably","tangibly","plainly","clearly","simply","obviously","apparently","evidently","patently","apparently","allegedly","reportedly","purportedly","supposedly","ostensibly","presumably","hypothetically","speculatively","theoretically","hypothetically","hypothetically","presumptively", "theoretically","suppositionally","conjecturally","purportedly","arguably","debatably","dubiously","doubtfully","questionably","controversially","disputably","disputedly","doubtfully"};

        vector<string>emp;
        vector<string> wordlist = generateWordList(question);
        for(int i=0;i<wordlist.size();i++){
            if(!isPresent(wordlist[i],stop_words)){
                emp.push_back(wordlist[i]);
            }
        }
        return emp;
    }


static vector<HubNode*> hubMaker(vector<Data*>datavector){

        vector<HubNode*>hubresults;
        HubNode* hub1 = new HubNode();
        hub1->insertHubdata(datavector[0]);
        hubresults.push_back(hub1);
        

        for(int i=1;i<datavector.size();i++){

        
            if(datavector[i]->book_code == datavector[i-1]->book_code && differnce(datavector[i-1]->page_no, datavector[i]->page_no) < 2){

               hubresults.back()->insertHubdata(datavector[i]);

            }

            else{
                HubNode* newhubnode = new HubNode();
                newhubnode->insertHubdata(datavector[i]);
                hubresults.push_back(newhubnode);
            }
        }
        return hubresults;
    }


static void updateHub(vector<HubNode*>&hubvector, int&n){
    float factor = 100;
    for(int i=0;i<hubvector.size();i++){

        if(hubvector[i]->hubwordvector.size() == n){
         
            hubvector[i]->hubscore *= factor;
        }

    }
}

};


class Chaining{

    private:
    int  max_size;

    public:
    vector<Element*>*word_storage; //main array for hashing for words 
    vector<Data*>*data_storage; // main array for hashing for datas 


    //constructor
    Chaining(){
        max_size = 1000000;
        word_storage = new vector<Element*>[max_size];
        data_storage = new vector<Data*>[max_size];   
    }

    //insert a given Data node in the hash table  //returns the pointer inserted data
    Data* insertData(int book_code, int page_no, int para_no){
        int hashcode = Algorithms::triplehash(book_code,page_no,para_no,max_size);
        Data* newData = new Data(book_code,page_no,para_no);
        data_storage[hashcode].push_back(newData);
        return newData;
    }

    
    //search a particular data present in the hash table , if not present the returns nullptr
    Data* searchData(int book_code, int page_no, int para_no){       
        int hashcode = Algorithms::triplehash(book_code,page_no,para_no,max_size);
        vector<Data*> subvector = data_storage[hashcode]; //chaining list for given hash code
        for(int i=0;i<subvector.size();i++){ 
            if(subvector[i]->book_code == book_code && subvector[i]->page_no == page_no && subvector[i]->para_no == para_no){
                return subvector[i];
            }
        }
        return nullptr;
    }


    //insert element node in the word_storage hash table: 
    void insertElement(string word, int book_code, int page_no, int para_no){
       int hashcode = Algorithms :: getHashcode(word); 
       for(int i=0;i<word_storage[hashcode].size();i++){

        //vector stored at respective hash index
        if(word_storage[hashcode][i]->word == word){
            if (searchData(book_code,page_no,para_no)==nullptr){
                word_storage[hashcode][i]->vec.push_back(insertData(book_code,page_no,para_no));
            }
            else{
                word_storage[hashcode][i]->vec.push_back(searchData(book_code,page_no,para_no));
            }
            return;
        }
       }
        Element *e = new Element(word); //new word which is not present in data_storage:
        if (searchData(book_code,page_no,para_no)==nullptr){
            e->vec.push_back(insertData(book_code,page_no,para_no));
        }
        else{
            e->vec.push_back(searchData(book_code,page_no,para_no));
        }
        word_storage[hashcode].push_back(e);
    }


    //returns the vector of the word from word_storage
    vector<Data*> searchElements(string word){

        vector<Data*>emp;
        int hashcode = Algorithms :: getHashcode(word);
        vector<Element*>subvector = word_storage[hashcode];
        
        for(int i=0;i<subvector.size();i++){
            
            if(subvector[i]->word == word){
      
               return subvector[i]->vec;
            }
        }
        return emp;
    }

    
    ~Chaining(){


        //delete data_storage pointers , elements,hubNode will also get deleted:
        for(int i=0;i<1000000;i++){
            for(int j=0;j<data_storage[i].size();j++){
                delete data_storage[i][j];
            }
        }   

        //deleting the dynamic array:
        delete []word_storage;
        delete []data_storage;
    }

};



class QNA_tool {

private:

    // You are free to change the implementation of this function
    void query_llm(string filename, Node* root, int k, string API_KEY, string question);
    // filename is the python file which will call ChatGPT API
    // root is the head of the linked list of paragraphs
    // k is the number of paragraphs (or the number of nodes in linked list)
    // API_KEY is the API key for ChatGPT
    // question is the question asked by the user
    // You can add attributes/helper functions here

public:

    Chaining* document;
    Dict* csvfile;

    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    
    QNA_tool(); // Constructor
    ~QNA_tool(); // Destructor

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);
    // This function is similar to the functions in dict and search 
    // The corpus will be provided to you via this function
    // It will be called for each sentence in the corpus

    Node* get_top_k_para(string question, int k);
    // This function takes in a question, preprocess it
    // And returns a list of paragraphs which contain the question
    // In each Node, you must set: book_code, page, paragraph (other parameters won't be checked)

    std::string get_paragraph(int book_code, int page, int paragraph);
    // Given the book_code, page number, and the paragraph number, returns the string paragraph.
    // Searches through the corpus.

    void query(string question, string filename);
    // This function takes in a question and a filename.
    // It should write the final answer to the specified filename.

    /* -----------------------------------------*/
    /* Please do not touch the code above this line */

    // You can add attributes/helper functions here
};
