#ifndef EN_H
#define EN_H

#include<unordered_map>
#include<vector>
#include<string>
#include<set>
#include<algorithm>

using namespace std;

class Enigma{

private:
    const vector<char> rotor_notch={'R','F','W'};
    const char ring_setting[3]={'D','E','S'};
    const std::unordered_map<char,char> rotor1{{'A','E'},{'B','K'},{'C','M'},{'D','F'},
        {'E','L'},{'F','G'},{'G','D'},{'H','Q'},{'I','V'},{'J','Z'},{'K','N'},
        {'L','T'},{'M','O'},{'N','W'},{'O','Y'},{'P','H'},{'Q','X'},{'R','U'},
        {'S','S'},{'T','P'},{'U','A'},{'V','I'},{'W','B'},{'X','R'},{'Y','C'},{'Z','J'}};
        const std::unordered_map<char,char> rotor2{{'A','A'},{'B','J'},{'C','D'},{'D','K'},
        {'E','S'},{'F','I'},{'G','R'},{'H','U'},{'I','X'},{'J','B'},{'K','L'},
        {'L','H'},{'M','W'},{'N','T'},{'O','M'},{'P','C'},{'Q','Q'},{'R','G'},
        {'S','Z'},{'T','N'},{'U','P'},{'V','Y'},{'W','F'},{'X','V'},{'Y','O'},{'Z','E'}};
    const std::unordered_map<char,char> rotor3{{'A','B'},{'B','D'},{'C','F'},{'D','H'},
        {'E','J'},{'F','L'},{'G','C'},{'H','P'},{'I','R'},{'J','T'},{'K','X'},
        {'L','V'},{'M','Z'},{'N','N'},{'O','Y'},{'P','E'},{'Q','I'},{'R','W'},
        {'S','G'},{'T','A'},{'U','K'},{'V','M'},{'W','U'},{'X','S'},{'Y','Q'},{'Z','O'}};
    const std::unordered_map<char,char> reflector{{'A','Y'},{'B','R'},{'C','U'},{'D','H'},
        {'E','Q'},{'F','S'},{'G','L'},{'H','D'},{'I','P'},{'J','X'},{'K','N'},
        {'L','G'},{'M','O'},{'N','K'},{'O','M'},{'P','I'},{'Q','E'},{'R','B'},
        {'S','F'},{'T','Z'},{'U','C'},{'V','W'},{'W','V'},{'X','J'},{'Y','A'},{'Z','T'}};

    string comb;
    vector<unordered_map<char,char>> rotorbox;
    vector<unordered_map<char,char>> inverse_rotorbox;
    vector<unordered_map<char,char>> get_rotor_box(string comb);

public:
    Enigma(string comb);
    char encryption(vector<char> initial_position,char input);
    vector<char> update_position(vector<char> pre_position);
    vector<int> generate_chain_list(vector<char> initial_position);
    vector<vector<char>> get_initp_seq(vector<char> pre_position,int num);
};

#endif