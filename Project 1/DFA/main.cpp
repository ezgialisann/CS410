
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

struct Key {
    std::string state;
    char variable;

    Key(std::string state, char variable): state(state), variable(variable){};
};

int main() {
    std::ifstream inputFile("C:/Users/ezgia/CLionProjects/DFA/example.txt");
    std::ofstream outputFile;
    outputFile.open("output.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }
    std::string line; // getting lines one by one
    // number of states
    int n_states = 0;
    if(std::getline(inputFile, line)){
        std::stringstream s(line);
        s >> n_states; // number of states equals to first line
    }
    // number of variables
    int n_variables = 0;
    if(std::getline(inputFile, line)){
        std::stringstream s(line);
        s >> n_variables; // number of states equals to second line
    }

    // number of goal states
    int n_goal_states = 0;
    if(std::getline(inputFile, line)){
        std::stringstream s(line);
        s >> n_goal_states; // number of states equals to third line
    }
    //cout<< n_goal_states << endl; // true
    // states
    std::vector<std::string> states;
    std::string state;
    if(std::getline(inputFile, line)){
        std::stringstream s(line);
        for(int i = 0; i<n_states;i++){
            s >> state;
            states.push_back(state);
        }
    }
    // goal states
    std::vector<std::string> goal_states;
    if(std::getline(inputFile, line)){
        std::stringstream s(line);
        for(int i = 0; i<n_goal_states;i++){
            s >> state;
            goal_states.push_back(state);
        }
    }

    // alphabet
    std::vector<std::string> variables{};
    if(std::getline(inputFile, line)){
        std::stringstream s(line);
        for(int i = 0; i<n_variables;i++){
            s >> state;
            variables.push_back(state);
        }
    }

    // total transitions = alphabet * number of states
    int total_transitions = n_variables * n_states;
    //cout<<total_transitions<<endl;
    // setting transitions
    std::vector<std::pair<Key, std::string>> tr_pairs;
    for(int i = 0; i< total_transitions; i++){
        if(std::getline(inputFile, line)){
            std::stringstream s(line);
            std::string from;
            std::string to;
            char c;
            s>> from;
            s>> c;
            s>> to;
            auto foundFrom = std::find(states.begin(), states.end(), from);
            auto foundTo = std::find(states.begin(), states.end(), to);
            if(foundFrom == states.end() || foundTo == states.end()){
                cout<< "Element found the transition "<<i+1<<" not an state, please enter a valid state which can be from the states! " << endl;
                return 0;
            }
            bool foundAlphabet = 0;
            for(const auto& var : variables){
                if(foundAlphabet!=1){
                    foundAlphabet = (var.at(0) == c);
                }
            }
            if(!foundAlphabet){
                cout<< "Element '"<<c<<"' is not in the alphabet, please enter a valid character which can be from the alphabet! " << endl;
                return 0;
            }
            Key key(from,c);
            auto p1 = std::make_pair(key,to);
            tr_pairs.push_back(p1);
        }
    }
    /*// transition control
    for(const auto& p : tr_pairs){
        cout << "From the state " << p.first.state << " with symbol " << p.first.variable << " to: " << p.second<< std::endl;
    }*/

    // to control the given transitions can create a DFA or not.
    vector<int> dfaControl(states.size());
    vector<int> alphabetCount(variables.size());
    for(int i = 0; i<states.size();i++){
        for (const auto& p: tr_pairs){
            if(p.first.state == states.at(i) ){ // find the transitions with the same state but different symbols
                for(int k = 0; k< variables.size();k++){
                    if(p.first.variable == variables.at(k).at(0)){
                        dfaControl.at(i)++;
                    }
                }
            }
        }
    }
    // control for symbol occurence
    for (const auto& p: tr_pairs){
        for(int k = 0; k< variables.size();k++){
            if(p.first.variable == variables.at(k).at(0)){
                alphabetCount.at(k)++;
            }
        }
    }
    // control for the transitions are correct for constructing dfa
    for(int i = 0; i < dfaControl.size(); i++){
        if(dfaControl.at(i) != variables.size()){
            cout<< "This transitions can not provide a DFA, please check for again"<<endl;
            return 0;
        }
        //cout<<"printing the dfa control vector "<< dfaControl.at(i)<< endl;
    }
    // control for the transitions are exactly true, one symbol from alphabet should appear total_transitions/2 times
    for(int i = 0; i< alphabetCount.size(); i++){
        //cout<<alphabetCount.at(i)<<endl;
        if(alphabetCount.at(i) != (total_transitions/2) ){ // if one alphabet going from a state is more than 1 in one transition or empty
            cout<< "This transitions can not provide a DFA, please check for again, more than one transition"<<endl;
            return 0;
        }
    }

    //int alphabet{n_variables};
    while(std::getline(inputFile, line)){
        std::stringstream s(line);
        std::string inputString;
        s >> inputString;
        auto followingState = states.at(0);
        // std::cout << followingState << " ";
        for(int i = 0; i <inputString.size();i++){
            auto found = 0;
            for(const auto& var : variables){
                if(found!=1){
                    found = (var.at(0) == inputString.at(i));
                }
            }
            if(!found){
                cout<<"Transition alphabet is not found! The symbol: "<< inputString.at(i)<< " is not an element of alphabet!" <<endl;
                return 0;
            }
            for(const auto& p : tr_pairs){
                if(p.first.state == followingState && p.first.variable == inputString.at(i)){ //
                    followingState = p.second;
                    outputFile << followingState << " ";
                    break;
                }
            }
        }
        outputFile << "(route taken)" << endl;
        bool flag;
        for(const auto& goal: goal_states){
            if(goal == followingState){
                flag = true;
                break;
            }else{
                flag = false;
            }
        }
        if(flag){
            outputFile<<"Accepted"<<std::endl;
        }else{
            outputFile<<"Rejected"<<std::endl;
        }
    }
    cout<< "The program done succesfully!"<<endl;
    outputFile.close();
    inputFile.close();
    return 0;
}


/*
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

struct Key {
    std::string state;
    char variable;

    Key(std::string state, char variable): state(state), variable(variable){};
};

int main() {
    std::ifstream inputFile("C:/Users/ezgia/CLionProjects/DFA/example4.txt");
    std::ofstream outputFile;
    outputFile.open("output.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }
    std::string line; // getting lines one by one
    // number of states
    int n_states = 0;
    if(std::getline(inputFile, line)){
        std::stringstream s(line);
        s >> n_states; // number of states equals to first line
    }
    // number of variables
    int n_variables = 0;
    if(std::getline(inputFile, line)){
        std::stringstream s(line);
        s >> n_variables; // number of states equals to second line
    }

    // number of goal states
    int n_goal_states = 0;
    if(std::getline(inputFile, line)){
        std::stringstream s(line);
        s >> n_goal_states; // number of states equals to third line
    }
    //cout<< n_goal_states << endl; // true
    // states
    std::vector<std::string> states;
    std::string state;
    if(std::getline(inputFile, line)){
        std::stringstream s(line);
        for(int i = 0; i<n_states;i++){
            s >> state;
            states.push_back(state);
        }
    }
    // goal states
    std::vector<std::string> goal_states;
    if(std::getline(inputFile, line)){
        std::stringstream s(line);
        for(int i = 0; i<n_goal_states;i++){
            s >> state;
            goal_states.push_back(state);
        }
    }

    // alphabet
    std::vector<std::string> variables{};
    if(std::getline(inputFile, line)){
        std::stringstream s(line);
        for(int i = 0; i<n_variables;i++){
            s >> state;
            variables.push_back(state);
        }
    }

    // total transitions = alphabet * number of states
    int total_transitions = n_variables * n_states;
    //cout<<total_transitions<<endl;
    // setting transitions
    std::vector<std::pair<Key, std::string>> tr_pairs;
    for(int i = 0; i< total_transitions; i++){
        if(std::getline(inputFile, line)){
            std::stringstream s(line);
            std::string from;
            std::string to;
            char c;
            s>> from;
            s>> c;
            s>> to;
            auto foundFrom = std::find(states.begin(), states.end(), from);
            auto foundTo = std::find(states.begin(), states.end(), to);
            if(foundFrom == states.end() || foundTo == states.end()){
                cout<< "Element found the transition "<<i+1<<" not an state, please enter a valid state which can be from the states! " << endl;
                return 0;
            }
            bool foundAlphabet = 0;
            for(const auto& var : variables){
                if(foundAlphabet!=1){
                    foundAlphabet = (var.at(0) == c);
                }
            }
            if(!foundAlphabet){
                cout<< "Element '"<<c<<"' is not in the alphabet, please enter a valid character which can be from the alphabet! " << endl;
                return 0;
            }
            Key key(from,c);
            auto p1 = std::make_pair(key,to);
            tr_pairs.push_back(p1);
        }
    }
    */
/*//*
/ transition control
    for(const auto& p : tr_pairs){
        cout << "From the state " << p.first.state << " with symbol " << p.first.variable << " to: " << p.second<< std::endl;
    }*//*


    // to control the given transitions can create a DFA or not.
    vector<int> dfaControl(states.size());
    vector<int> alphabetCount(variables.size());
    for(int i = 0; i<states.size();i++){
        for (const auto& p: tr_pairs){
            if(p.first.state == states.at(i) ){ // find the transitions with the same state but different symbols
                for(int k = 0; k< variables.size();k++){
                    if(p.first.variable == variables.at(k).at(0)){
                        dfaControl.at(i)++;
                    }
                }
            }
        }
    }
    // control for symbol occurence
    for (const auto& p: tr_pairs){
        for(int k = 0; k< variables.size();k++){
            if(p.first.variable == variables.at(k).at(0)){
                alphabetCount.at(k)++;
            }
        }
    }
    // control for the transitions are correct for constructing dfa
    for(int i = 0; i < dfaControl.size(); i++){
        if(dfaControl.at(i) != variables.size()){
            cout<< "This transitions can not provide a DFA, please check for again"<<endl;
            return 0;
        }
        //cout<<"printing the dfa control vector "<< dfaControl.at(i)<< endl;
    }
    // control for the transitions are exactly true, one symbol from alphabet should appear total_transitions/2 times
    for(int i = 0; i< alphabetCount.size(); i++){
        //cout<<alphabetCount.at(i)<<endl;
        if(alphabetCount.at(i) != (total_transitions/2) ){ // if one alphabet going from a state is more than 1 in one transition or empty
            cout<< "This transitions can not provide a DFA, please check for again, more than one transition"<<endl;
            return 0;
        }
    }

    //int alphabet{n_variables};
    while(std::getline(inputFile, line)){
        std::stringstream s(line);
        std::string inputString;
        s >> inputString;
        auto followingState = states.at(0);
       // std::cout << followingState << " ";
        for(int i = 0; i <inputString.size();i++){
            auto found = 0;
            for(const auto& var : variables){
                if(found!=1){
                    found = (var.at(0) == inputString.at(i));
                }
            }
            if(!found){
                cout<<"Transition alphabet is not found! The symbol: "<< inputString.at(i)<< " is not an element of alphabet!" <<endl;
                return 0;
            }
            for(const auto& p : tr_pairs){
                if(p.first.state == followingState && p.first.variable == inputString.at(i)){ //
                    followingState = p.second;
                    outputFile << followingState << " ";
                    break;
                }
            }
        }
        outputFile << "(route taken)" << endl;
        bool flag;
        for(const auto& goal: goal_states){
            if(goal == followingState){
                flag = true;
                break;
            }else{
                flag = false;
            }
        }
        if(flag){
            outputFile<<"Accepted"<<std::endl;
        }else{
            outputFile<<"Rejected"<<std::endl;
        }
    }
    cout<< "The program done succesfully!"<<endl;
    outputFile.close();
    inputFile.close();
    return 0;
}
*/
