#include <bits/stdc++.h>
using namespace std;

class DFA {
    string input;
    int number_states;   
    vector<int> alphabet;
    vector<string> states;
    vector<string> pairs_states;
    map<pair<string, int>, string> transitions;

    void build_dfa();
    void add_transition(const string& p, int a, const string& q);

    /* dec and cad sync private methods */
    vector<string> generate_pairs_states(const vector<string>& st); 
    bool is_unit(const string& state); 
    void dec_bfs(const string& pair_state, DFA& dec_dfa, map<string, list<string>>& adj, int& count);
    unordered_map<string, string> cad_bfs(const string& pair_state, DFA& dec_dfa, map<string, list<string>>& adj);
    vector<string> reconstruct_path(const string& start, unordered_map<string, string>& prev, DFA& dec_dfa);
    string find_sequence(const string& start, unordered_map<string, string>& prev, DFA& dec_dfa);
    string get_w(unordered_map<string, unordered_map<string, string>>& prevs, DFA& dec_dfa);

public:
    DFA();
    void build_dfa(int nstates);
    DFA create_dec_dfa();
    map<string, list<string>> get_adj_dec(DFA& dec_dfa);
    string dec_sync(DFA& dec_dfa, map<string, list<string>>& adj);
    string cad_sync(DFA& dec_dfa, map<string, list<string>>& adj);
};

DFA::DFA() = default;

void DFA::add_transition(const string& p, int a, const string& q) {
    transitions[make_pair(p, a)] = q;
}

vector<string> DFA::generate_pairs_states(const vector<string>& st) {
    vector<string> pairs_states;
    for (int i = 0; i < st.size(); ++i) {
        for (int j = i + 1; j < st.size(); ++j) {
            string pair = st[i] + st[j];
            pairs_states.push_back(pair);
        }
    }
    return pairs_states;
}

DFA DFA::create_dec_dfa() {
    DFA result;
    vector<string> pairs_states = generate_pairs_states(this->states);
    result.pairs_states = pairs_states;
    result.number_states = this->number_states + pairs_states.size();

    /* save states of new dfa */
    result.states.insert(result.states.end(), this->states.begin(), this->states.end());
    result.states.insert(result.states.end(), pairs_states.begin(), pairs_states.end());

    /* generate new transitions */
    result.transitions = this->transitions;
    for (const auto& p : pairs_states) {
        string s1 {p[0]};
        string s2 {p[1]};

        /* new transition with '0' */
        string s1_0 = this->transitions[pair<string, int>(s1, 0)];
        string s2_0 = this->transitions[pair<string, int>(s2, 0)];
        string s3_0;
        if (s1_0 == s2_0) {
            s3_0 += s1_0;
        } else if (s1_0 < s2_0) {
            s3_0 += s1_0;
            s3_0 += s2_0;
        } else {
            s3_0 += s2_0;
            s3_0 += s1_0;
        }

        /* new transition with '1' */
        string s1_1 = this->transitions[pair<string, int>(s1, 1)];
        string s2_1 = this->transitions[pair<string, int>(s2, 1)];
        string s3_1;
        if (s1_1 == s2_1) {
            s3_1 += s1_1;
        } else if (s1_1 < s2_1) {
            s3_1 += s1_1;
            s3_1 += s2_1;
        } else {
            s3_1 += s2_1;
            s3_1 += s1_1;
        }

        result.add_transition(p, 0, s3_0);
        result.add_transition(p, 1, s3_1);
    }
    return result;
}

bool DFA::is_unit(const string& state) {
    return state.size() == 1;
}

map<string, list<string>> DFA::get_adj_dec(DFA& dec_dfa) {
    unsigned int size = dec_dfa.states.size();
    map<string, list<string>> adj_list;
    for (const auto& s : dec_dfa.states) {
        list<string> l;
        string s_0 = dec_dfa.transitions[pair<string, int>(s, 0)];
        string s_1 = dec_dfa.transitions[pair<string, int>(s, 1)];
        l.push_back(s_0);
        if (s_0 != s_1)
            l.push_back(s_1);
        adj_list[s] = l;
    }
    return adj_list;
}

void DFA::dec_bfs(const string& pair_state, DFA& dec_dfa, map<string, list<string>>& adj, int& count) {
    string start = pair_state;

    /* mark all states as not visited */
    map<string, bool> visited;
    for (const auto& state : dec_dfa.states)
        visited[state] = false;
        
    /* create a queue for bfs */
    list<string> queue;

    /* mark the current state as visited and enqueue it */
    visited[start] = true;
    queue.push_back(start);

    list<string>::iterator it;
    while (!queue.empty()) {
        /* dequeue a state from queue */
        start = queue.front();
        queue.pop_front();
        
        /* get all adjacent states of the dequeued state "start"
           mark as visited and enqueue it if has not been visited */
        for (it = adj[start].begin(); it != adj[start].end(); ++it) {
            if (!visited[*it]) {
                visited[*it] = true;
                queue.push_back(*it);
                if (is_unit(*it)) {
                    ++count;        
                    queue.clear();
                    break;
                }
            }
        }
    }
}

string DFA::dec_sync(DFA& dec_dfa, map<string, list<string>>& adj) {
    string output;
    if (this->number_states == 1) {
        output = "SI";
        return output;
    }
    auto pairs_states = dec_dfa.pairs_states;
    int n_pairs = pairs_states.size();
    int count = 0;
    for (const auto& pair : pairs_states) {
        dec_bfs(pair, dec_dfa, adj, count);
    }
    output = (count == n_pairs) ? "SI" : "NO";
    return output;
}

unordered_map<string, string> DFA::cad_bfs(const string& pair_state, DFA& dec_dfa, map<string, list<string>>& adj) {
    string start = pair_state;
    unordered_map<string, string> prev;

    /* mark all states as not visited */
    map<string, bool> visited;
    for (const auto& state : dec_dfa.states)
        visited[state] = false;
        
    /* create a queue for bfs */
    list<string> queue;

    /* mark the current state as visited and enqueue it */
    visited[start] = true;
    queue.push_back(start);

    list<string>::iterator it;
    while (!queue.empty()) {
        /* dequeue a state from queue */
        start = queue.front();
        queue.pop_front();
        
        /* get all adjacent states of the dequeued state "start"
           mark as visited and enqueue it if has not been visited */
        for (it = adj[start].begin(); it != adj[start].end(); ++it) {
            if (!visited[*it]) {
                visited[*it] = true;
                prev[*it] = start;
                queue.push_back(*it);
                if (is_unit(*it)) {
                    queue.clear();
                    break;
                }
            }
        }
    }
    return prev;
}

vector<string> DFA::reconstruct_path(const string& start, unordered_map<string, string>& prev, DFA& dec_dfa) {
    /* reconstruct path going backwards from end */
    string end = prev.begin()->first;
    vector<string> path {end}; // end -> start

    while (path.back() != start) {
        /* inserts the prev state of the current state */
        path.push_back(prev[path.back()]);
    }
    reverse(path.begin(), path.end()); // start -> end

    /* if start and end are connected return the path */
    if (path[0] == start) {
        return path;
    }
    path.clear();
    return path;
}

string DFA::find_sequence(const string& start, unordered_map<string, string>& prev, DFA& dec_dfa) {
    string seq;
    vector<string> path = reconstruct_path(start, prev, dec_dfa);
    for (int i = 0; i < (int)path.size() - 1; ++i) {
        /* validate with '0' and '1' */
        for (const auto& c : this->alphabet) {
            if (dec_dfa.transitions[pair<string, int>(path[i], c)] == path[i + 1]) {
                seq += to_string(c);
            }
        }
    }
    return seq;
}

string DFA::get_w(unordered_map<string, unordered_map<string, string>>& prevs, DFA& dec_dfa) {
    vector<string> x = this->states; // copy of orginal states set S
    string seq; // empty sequence t
    while (x.size() > 1) {
        /* pick any pair of X */
        vector<string> pairs = generate_pairs_states(x);
        string pair = pairs.back();

        /* find a sequence t' taking Si and Sj to the same state */
        string seq_temp = find_sequence(pair, prevs[pair], dec_dfa);

        /* updates X */
        set<string> x_temp;
        for (auto st : x) {
            string st_out;
            string st_temp = st;
            for (auto c : seq_temp) {
                int ic = c - '0';
                st_out = dec_dfa.transitions[make_pair(st_temp, ic)];
                st_temp = st_out;
            }
            x_temp.insert(st_out);
        }
        x.clear();
        for (auto st : x_temp) {
            x.push_back(st);
        }

        /* updates sequence t = t.t' */
        seq += seq_temp;
    }
    return seq;
}

string DFA::cad_sync(DFA& dec_dfa, map<string, list<string>>& adj) {
    srand(time(0));
    string output;
    if (this->number_states == 1) {
        string s = {"01E"};
        int r = rand() % 3;
        output = s[r];
        return output;
    }
    unordered_map<string, unordered_map<string, string>> prevs;
    auto pairs_states = dec_dfa.pairs_states;
    for (const auto& p : pairs_states) {
        unordered_map<string, string> prev = cad_bfs(p, dec_dfa, adj);
        prevs[p] = prev;
    }
    output = get_w(prevs, dec_dfa);
    return output;
}

void DFA::build_dfa(int nstates) {
    number_states = nstates;
    for (int i = 1; i < nstates + 1; ++i) {
        string state = to_string(i);
        states.push_back(state);
    }
    int p, q;
    for (int i = 1; i < nstates + 1; ++i) {
        p = i;
        cin >> q;
        add_transition(to_string(p), 0, to_string(q));
    }
    for (int i = 1; i < nstates + 1; ++i) {
        p = i;
        cin >> q;
        add_transition(to_string(p), 1, to_string(q));
    }
    alphabet = {0 , 1};
}

void magical_stones2() {
    int ncases;
    cin >> ncases;
    while (ncases--) {
        string output;
        int nstates;
        cin >> nstates;
        DFA dfa;
        dfa.build_dfa(nstates);
        DFA dec_dfa = dfa.create_dec_dfa();
        auto adj = dfa.get_adj_dec(dec_dfa);
        string is_dec = dfa.dec_sync(dec_dfa, adj);
        if (is_dec == "SI") {
            string out_temp = dfa.cad_sync(dec_dfa, adj);
            for (auto c : out_temp) {
                if (c == '0') {
                    output.push_back('W');
                } else if (c == '1') {
                    output.push_back('B');
                }
            }
        } else {
            output = "impossible";
        }
        cout << output << endl;
    }
}

int main() {
  magical_stones2();
}