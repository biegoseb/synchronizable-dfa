#include "dfa.hpp"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        cerr << "No argument given." << endl;
        return 1;
    }

    string filename = argv[1];
    DFA dfa0(filename);
    //auto output = dfa0.dec_sync();
    //cout << output << endl;
    auto output = dfa0.cad_sync();
    cout << output << endl;

    return 0;
}