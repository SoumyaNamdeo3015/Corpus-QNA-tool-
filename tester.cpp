#include<sstream>
#include "Node.h"
#include "qna_tool.h"

using namespace std;

int main(){

    QNA_tool qna_tool;

    int num_books = 98;

    for(int i = 1; i <= num_books; i++){

        std::cout << "Inserting book " << i << std::endl;

        std::string filename = "mahatma-gandhi-collected-works-volume-";
        filename += to_string(i);
        filename += ".txt";

        std::ifstream inputFile(filename);

        if (!inputFile.is_open()) {
            std::cerr << "Error: Unable to open the input file mahatma-gandhi." << std::endl;
            return 1;
        }

        std::string tuple;
        std::string sentence;


        while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
            // Get a line in the sentence
            tuple += ')';

            std::vector<int> metadata;    
            std::istringstream iss(tuple);

            // Temporary variables for parsing
            std::string token;

            // Ignore the first character (the opening parenthesis)
            iss.ignore(1);

            // Parse and convert the elements to integers
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
                    metadata.push_back(num);
                } else {
                    // Convert the element to integer
                    int num = std::stoi(token);
                    metadata.push_back(num);
                }
            }

            // Now we have the string in sentence
            // And the other info in metadata
            // Add to the dictionary

            // Insert in the qna_tool
            qna_tool.insert_sentence(metadata[0], metadata[1], metadata[2], metadata[3], sentence);
        }

        inputFile.close();

    }

    // string question = "who were mahatma gandhi wife";

    /*
        Part-1 : Get the top-k paragraphs
        Checking Correctness
    */

    // Let's try to ask a simple question to the qna_tool
    // Node* head = qna_tool.get_top_k_para(question, 5);

    // vector<string> paras;
    // while(head != nullptr){
    //     string res = qna_tool.get_paragraph(head->book_code, head->page, head->paragraph);
    //     paras.push_back(res);
    //     head = head->right;
    // }

    // for(int i = 0; i < (int)paras.size(); i++){
    //     cout << paras[i] << endl << endl << endl;
    // }

    /*
        Part-2 : Query the LLM

    */

   
    std::vector<std::string> gandhiQuestions ={
        "Where and when was Mahatma Gandhi born?",
        "What was Gandhi's family background like?",
        "What was his education like during his early years?",
        "What role did Mahatma Gandhi play in South Africa?",
        "How did his experiences in South Africa influence his later activism in India?",
        "What were the major events during his time in South Africa?",
        "What is the concept of Satyagraha, and how did Gandhi employ it in his activism?",
        "Can you name some of the major movements led by Gandhi in India?",
        "How did nonviolent resistance contribute to India's independence?",
        "Describe the significance of the Salt March.",
        "What were the main objectives of the Salt Satyagraha?",
        "How did the Salt March impact the Indian independence movement?",
        "What was Gandhi's role in the Indian National Congress?",
        "How did he contribute to the framing of the Indian Constitution?",
        "What were his views on partition and the communal tensions during that time?",
        "What were Mahatma Gandhi's key philosophical and spiritual beliefs?",
        "How did his philosophy influence his lifestyle, including his choice of clothing and diet?",
        "What were his views on religious harmony and tolerance?",
        "How is Mahatma Gandhi remembered and celebrated in India and around the world?",
        "What impact did Gandhi have on the global understanding of nonviolence?",
        "How has his legacy influenced other leaders and movements?",
        "When and how did Mahatma Gandhi die?",
        "What were the circumstances surrounding his assassination?",
        "How did his death affect the trajectory of Indian politics?"
    };

    // for(int i=0;i<gandhiQuestions.size();i++){
    
    // cout << "question : " << i+1   <<" " << gandhiQuestions[i] << endl; 
    // qna_tool.query(gandhiQuestions[i], "api_call.py");
    
    // }

    // qna_tool.query("what were the views of mahatma on partition?","api_call.py");
    // qna_tool.query( "how did he contribute to the making of indian constitution","api_call.py");
    // qna_tool.query("How did his death affect the trajectory of Indian politics?","api_call.py");
    // qna_tool.query("effect of tea and coffee according to gandhi","api_call.py");
    // qna_tool.query("who was godse?","api_call.py");
    // qna_tool.query("when did mahatma gandhi died?","api_call.py");
    while(true){
        cout <<"enter question :" << endl;
        string s;
        getline(cin,s);
    qna_tool.query(s,"api_call.py");
    }

    


    return 0;
}

