#include "dfa.hpp"

int main(int argc, char *argv[]) {

    if (argc != 2)
    {
        cerr << "No argument given." << endl;
        return 1;
    }

    string filename = argv[1];
    DFA dfa0(filename);
    dfa0.print_dfa();
    dfa0.print_sets();

    return 0;
}