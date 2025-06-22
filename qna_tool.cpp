#include <assert.h>
#include <sstream>
#include "qna_tool.h"

using namespace std;

QNA_tool::QNA_tool(){
    // Implement your function here
    document = new Chaining();
    csvfile = new Dict();
    ifstream file;
    file.open("unigram_freq.csv");
    string line;
    
    getline(file,line);

    while(getline(file,line)){

        long long int result =0;
        string word = "";
        int i=0;
        while(line[i]!=','){
            word+=line[i];
            i++;
        }
        i++;

        while(i<line.size()){
            int number = line[i]-'0';
            result = result*10 + number;
            i++;
        }   

    csvfile->addTransaction(word,result);
}
    file.close();
}


QNA_tool::~QNA_tool(){
    // Implement your function here 
    delete document;
    delete csvfile; 
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here
    
    vector<string>wordList = Algorithms::generateWordList(sentence);
    for(int i=0;i<wordList.size();i++){
        document->insertElement(wordList[i],book_code,page,paragraph);
    }
    return;
}


Node* QNA_tool::get_top_k_para(string question, int k) {
    // Implement your function here
    vector<Data*>result;
    vector<string>wordlist = Algorithms::generateWordList(question);
    for(int i=0;i<wordlist.size();i++){


        vector<Data*> searchresult = document->searchElements(wordlist[i]);
       
        for(int j=0;j<searchresult.size();j++){
            searchresult[j]->wordvector.push_back(i); //part2
            if(searchresult[j]->ispresent == false){
                result.push_back(searchresult[j]);
                searchresult[j]->ispresent = true;
            }

            searchresult[j]->score += double(searchresult.size()+1)/double(1+csvfile->get_word_count(wordlist[i]));
        } 
    }

   
    Algorithms::mergeSort(result,0,result.size()-1);
 
    Node* head= nullptr;
    Node* tail = head;

    

    for(int i=0;i<k;i++){

        if(head==nullptr){
            Node* newNode=  new Node(result[i]->book_code, result[i]->page_no, result[i]->para_no, 0,0);
            head = newNode;
            tail = newNode;
            head->left = nullptr;
            tail->right = nullptr;
         
        }

        else{
            Node* newNode=  new Node(result[i]->book_code, result[i]->page_no, result[i]->para_no, 0,0);
            
          
            tail->right = newNode;
            newNode->left = tail;
            tail=newNode;
            head->left = nullptr;
            tail->right = nullptr;
          
        
        }
    }

    for(int i=0;i<result.size();i++){
        result[i]->score =0;
        result[i]->ispresent=false;
    }

    return head;
}

void  QNA_tool::query(string question, string filename){
    // Implement your function here
    vector<Data*>result;

    vector<string>wordlist = Algorithms::modifyQuery(question);

    for(int i=0;i<wordlist.size();i++){

        
        
        vector<Data*> searchresult = document->searchElements(wordlist[i]);
        for(int j=0;j<searchresult.size();j++){
            searchresult[j]->wordvector.push_back(i);
            if(searchresult[j]->ispresent == false){
                result.push_back(searchresult[j]);
                searchresult[j]->ispresent = true;
            }
            
            // if(wordlist[i]=="gandhi" || wordlist[i] == "mahatma" || wordlist[i]=="gandhiji"){
            //    searchresult[j]->score += float(1)/float(1+searchresult.size());
            // }

            // else{
            // searchresult[j]->score += float(searchresult.size()+1)/float(1+csvfile->get_word_count(wordlist[i]));

            // if(wordlist[i]=="gandhi" || wordlist[i] == "mahatma" || wordlist[i]=="gandhiji"){
            //     searchresult[j]->score =  (searchresult[j]->score)/float(100000);
            // }
            // }
            
            searchresult[j]->score += float(1)/float(1+searchresult.size());
        }

        // cout << "freq for " << wordlist[i] <<  " is " << score1 << endl;
    }

    Algorithms::mergeSortdifferent(result,0,result.size()-1);

    vector<HubNode*>hubvector = Algorithms::hubMaker(result);
    
    int n =  wordlist.size();
    Algorithms :: updateHub(hubvector,n);

    Algorithms::mergeHUBSort(hubvector,0,hubvector.size()-1);



    vector<Data*>finalresult;

    for(int i=0;i<5;i++){
        //  cout <<  "size of hub "<< i+1 << " is "<< hubvector[i]->datalist.size() << endl;
        for(int j=0;j<hubvector[i]->datalist.size();j++){
            
            finalresult.push_back(hubvector[i]->datalist[j]);
        }
    }

    Algorithms :: mergeSort(finalresult,0,finalresult.size()-1);



    Node* head = nullptr;
    Node* tail = head;

    int k = 7;

    for(int i=0;i<k;i++){
        

        if(head==nullptr){
            Node* newNode=  new Node(finalresult[i]->book_code, finalresult[i]->page_no, finalresult[i]->para_no, 0,0);
            head = newNode;
            tail = newNode;
            head->left = nullptr;
            tail->right = nullptr;
        }

        else{
            Node* newNode=  new Node(finalresult[i]->book_code, finalresult[i]->page_no, finalresult[i]->para_no, 0,0);

            tail->right = newNode;
            newNode->left = tail;
            tail=newNode;
            head->left = nullptr;
            tail->right = nullptr;
        }
    }

    std::cout << "Q: " << question << endl;
    
    std::cout << "A: " << endl;
    query_llm("api_call.py",head,k,"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",question);


    for(int i=0;i<hubvector.size();i++){

        for(int j=0;j<hubvector[i]->datalist.size();j++){
            hubvector[i]->datalist[j]->score = 0;
            hubvector[i]->datalist[j]->ispresent = false;
        }
    }

}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph){

    cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;
    
    std::string filename = "mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ',')) {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            
            // Check if the element is a number or a string
            if (token[0] == '\'') {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph)
        ){
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}

void QNA_tool::query_llm(string filename, Node* root, int k, string API_KEY, string question){

    // first write the k paragraphs into different files

    Node* traverse = root;
    int num_paragraph = 0;

    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
    outfile << question;
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();
 
    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";

    system(command.c_str());

    

    return;
}
//api-key:
// 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

