#ifndef DFA_HPP
#define DFA_HPP

#include <bits/stdc++.h>
using namespace std;

class DFA 
{
    string input;
    int number_states;
    int initial_state;
    int number_final_states;
    unordered_set<int> non_final_states;
    unordered_set<int> final_states;
    map<pair<int, int>, int> transitions;

    void build_dfa();
    void add_final_state(int fs);
    void add_transition(int p, int a, int q);
    DFA* getB(DFA origin);
    
public:
    DFA(const string& input);
    void print_dfa();
    void print_sets();
    unordered_set<string> get_nfs();
    unordered_set<string> get_fs();
    map<pair<string, string>, string> get_transitions();
    //void validate_string(const string& str);
};

DFA::DFA(const string& input) : input(input) 
{
    build_dfa();
}

void DFA::build_dfa()
{   
    fstream file("../input/" + input + ".txt", ios::in);

    /* first line */
    file >> number_states >> initial_state >> number_final_states;
    int fs;
    for (size_t i = 0; i < number_final_states; ++i)
    {   
        file >> fs;
        add_final_state(fs);
    }
    for (size_t i = 0; i < number_states; ++i)
    {
        if (final_states.find(i) == final_states.end())
            non_final_states.insert(i);
    }
    
    /* transitions */
    int p, a, q;
    for (size_t i = 0; i < 2 * number_states; ++i)
    {
        file >> p >> a >> q;
        add_transition(p, a, q);
    }
}

void DFA::add_final_state(int fs)
{
    final_states.insert(fs);
}

void DFA::add_transition(int p, int a, int q)
{
    transitions[make_pair(p, a)] = q;
}

void DFA::print_dfa()
{
    cout << number_states << " " << initial_state << " " << number_final_states << " ";
    for (const auto& fs : final_states)
        cout << fs << " ";
    cout << endl;
    for (auto it = transitions.cbegin(); it != transitions.cend(); ++it)
        cout << it->first.first << " " << it->first.second << " " << it->second << endl;
    cout << endl;
    
}

void DFA::print_sets()
{
    cout << "Non final states: ";
    for (auto it = non_final_states.cbegin(); it != non_final_states.cend(); ++it)
        cout << *it << " ";
    cout << endl;
    cout << "Final states    : ";
    for (auto it = final_states.cbegin(); it != final_states.cend(); ++it)
        cout << *it << " ";
    cout << endl;
}



unordered_set<int> DFA::get_nfs()
{
    return non_final_states;
}
unordered_set<int> DFA::get_fs()
{
    return final_states;
}

map<pair<int, int>, int> DFA::get_transitions()
{
    return transitions;
}

DFA* DFA::getB(DFA origin) {
    DFA* DFA_B = this;

    for (int j = 0; j < ; ++j) {

    }
    /*double DFA_Bsize = 0;
    DFA_Bsize = 2 * (pow(2, (origin.number_states)) - 1);
    DFA_B->number_states = (DFA_Bsize/2)-1;
    unordered_set<int> Bstates = origin.non_final_states;

    for (int k = 0; k < origin.final_states.size(); k++) {
        Bstates.insert(origin.final_states[k]);
    }

    for (int i = 0; i < DFA_Bsize; i++) {
        for (int j = 0; j < origin.number_final_states; j++) {
            if(Bstates[i] = origin.final_states[j]){

            }
        }
    }*/
    return DFA_B;
}


#endif // DFA_HPP