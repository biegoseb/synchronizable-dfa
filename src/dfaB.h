#ifndef UNTITLED1_DFAB_H
#define UNTITLED1_DFAB_H

#include "dfa.hpp"
#include <bits/stdc++.h>
using namespace std;

class DFA_B
{
    int number_states;
    int number_u_states;
    string initial_state;
    unordered_set<string> states;
    unordered_set<string> u_states;

    map<pair<string, string>, int> transitions;

    void add_transition(string p, int a, string q);
    void powerDFA(unordered_set<string> S, unordered_set<string> set, int n);

public:
    DFA_B(DFA A);
    void print_dfa();
    void print_sets();
    unordered_set<string> get_states();
    unordered_set<string> get_ustates();
    map<pair<string, string>, int> get_transitions();
    //void validate_string(const string& str);
};

DFA_B::DFA_B(DFA A)
{
    this->u_states = A.get_nfs();
    for (int i = 0; i < A.get_fs().size(); i++) {
        this->u_states.insert(A.get_fs()[i]);
    }
    this->number_u_states = A.get_nfs().size()+ A.get_fs().size();
    this->number_states = pow(2, number_u_states) - 1;
    powerDFA(states, u_states, number_u_states);
}

void DFA_B::powerDFA(unordered_set<string> S, unordered_set<string> set, int n) {
    if(n == 0){
        return;
    }
    states.insert(set[n-1]);
    powerDFA(S, set,n-1);
    S.erase(S.end());
    powerDFA(S, set,n-1);

}

void DFA_B::powerDFA(unordered_set<string> uS) {
    int count = 0;
    for (int k = 0; k < number_states; k++) {
        for (int i = 0; i < number_states-k; i++) {
            for (int j = i+1; j < number_states-k; j++) {
                count = 1;
                if(uS[j].size() == 1){
                    uS.insert(uS[i]+','+uS[j]);
                    if (transitions[uS[i],0] != transitions[uS[j],0]){
                        add_transition(uS.end(),0,transitions[uS[i],0]+','+transitions[uS[j],0])
                    } else{add_transition(uS.end(),0,transitions[uS[i],0]}
                    if (transitions[uS[i],1] != transitions[uS[j],1]){
                        add_transition(uS.end(),1,transitions[uS[i],1]+','+transitions[uS[j],1])
                    } else{add_transition(uS.end(),1,transitions[uS[i],1]}
                }else{
                    for (int l = 0; l < uS[j].size(); l =l+2) {
                        if (uS[j][l] = uS[i]){
                            
                        }
                    }
                }

            }
        }
    }
}

void DFA_B::add_transition(string p, string a, int q)
{
    transitions[make_pair(p, a)] = q;
}

void DFA_B::print_dfa()
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



unordered_set<string> DFA_B::get_states()
{
    return states;
}

map<pair<string, string>, int> DFA_B::get_transitions()
{
    return transitions;
}

#endif //UNTITLED1_DFAB_H
