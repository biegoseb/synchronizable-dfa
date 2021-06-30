#ifndef DFA_HPP
#define DFA_HPP

#include <bits/stdc++.h>
using namespace std;

class DFA {
    string input;
    int number_states;
    int initial_state;
    int number_final_states;
    vector<string> states;
    unordered_set<string> non_final_states;
    unordered_set<string> final_states;
    map<pair<string, int>, string> transitions;

    void build_dfa();
    void add_final_state(const string& fs);
    void add_transition(const string& p, int a, const string& q);
    //DFA* getB(DFA origin);
    vector<string> power_set();
    DFA getB();
    map<string, list<string>> get_adjB();
    vector<string> bfs();
    
public:
    DFA(const string& input);
    void print_dfa();
    void print_sets();
    unordered_set<string> get_nfs();
    unordered_set<string> get_fs();
    map<pair<string, int>, string> get_transitions();
    void is_sync(); 
};

DFA::DFA(const string& input) : input(input) {
    build_dfa();
}

void DFA::build_dfa() {   
    fstream file("../input/" + input + ".txt", ios::in);

    /* first line */
    file >> number_states >> initial_state >> number_final_states;
    int fs;
    for (size_t i = 0; i < number_final_states; ++i) {   
        file >> fs;
        add_final_state(to_string(fs));
    }
    for (size_t i = 0; i < number_states; ++i) {
        string state = to_string(i);
        if (final_states.find(state) == final_states.end())
            non_final_states.insert(state);
    }
    
    /* transitions */
    int p, a, q;
    for (size_t i = 0; i < 2 * number_states; ++i) {
        file >> p >> a >> q;
        add_transition(to_string(p), a, to_string(q));
    }

    for (size_t i = 0; i < number_states; ++i) {
        string state = to_string(i);
        states.push_back(state);
    }
}

void DFA::add_final_state(const string& fs) {
    final_states.insert(fs);
}

void DFA::add_transition(const string& p, int a, const string& q) {
    transitions[make_pair(p, a)] = q;
}

void DFA::print_dfa() {
    cout << number_states << " " << initial_state << " " << number_final_states << " ";
    for (const auto& fs : final_states)
        cout << fs << " ";
    cout << endl;
    for (auto it = transitions.cbegin(); it != transitions.cend(); ++it)
        cout << it->first.first << " " << it->first.second << " " << it->second << endl;
    cout << endl;
    
}

void DFA::print_sets() {
    cout << "Non final states: ";
    for (auto it = non_final_states.cbegin(); it != non_final_states.cend(); ++it)
        cout << *it << " ";
    cout << endl;
    cout << "Final states    : ";
    for (auto it = final_states.cbegin(); it != final_states.cend(); ++it)
        cout << *it << " ";
    cout << endl;
    cout << "Power Set: ";
    for (const auto s : power_set()) {
        cout << s << " ";
    }
    cout << endl;
}

unordered_set<string> DFA::get_nfs() {
    return non_final_states;
}

unordered_set<string> DFA::get_fs() {
    return final_states;
}

map<pair<string, int>, string> DFA::get_transitions() {
    return transitions;
}

vector<string> DFA::power_set() {
    // O(2^n) algorithm
    vector<string> pset;
    unsigned int size = pow(2, number_states);
    int counter, i;

    for (counter = 0; counter < size; counter++) {
        string state;
        for (i = 0; i < number_states; i++) {           
            if (counter & (1 << i))
                state += states[i];           
        }
        pset.push_back(state);
    }
    return pset;
}

DFA DFA::getB() {
 //
}

map<string, list<string>> DFA::get_adjB() {
    auto pset = power_set();
    unsigned int size = pset.size();
    map<string, list<string>> adj;
    // transitions
    return adj;
}

vector<string> DFA::bfs() {
    auto pset = power_set();
    unsigned int size = pset.size();
    string start = pset.back(); // bigger set of power set
    vector<string> path;

    // mark all states as not visited
    map<string, bool> visited;
    for (const auto& state : pset)
        visited[state] = false;
        
    // create a queue for bfs
    list<string> queue;

    // mark the current state as visited and enqueue it
    visited[start] = true;
    queue.push_back(start);

    list<string>::iterator it;
    while (!queue.empty()) {
        // dequeue a state from queue
        start = queue.front();
        path.push_back(start);
        queue.pop_front();
        
        /* get all adjacent states of the dequeued state "start"
           mark as visited and enqueue it if has not been visited */
        auto adj = get_adjB(); 
        for (it = adj[start].begin(); it != adj[start].end(); ++it) {
            if (!visited[*it]) {
                visited[*it] = true;
                queue.push_back(*it);
            }
        }
    }
    return path;
}

void DFA::is_sync() {
    auto path = bfs();
    for (const auto& state : path) {
        if (state.size() == 1) {
            // if unit state
            // print path
        }         
        else {
            cout << "NO" << endl;
        }               
    }
}

/*DFA* DFA::getB(DFA origin) {
    DFA* DFA_B = this;

    for (int j = 0; j < ; ++j) {

    }
    double DFA_Bsize = 0;
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
    }
    return DFA_B;
}*/


#endif // DFA_HPP