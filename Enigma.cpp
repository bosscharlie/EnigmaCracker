#include "Enigma.h"

Enigma::Enigma(string comb):comb(comb){
    rotorbox = get_rotor_box(comb);
    unordered_map<char,char> inverse_rotor1,inverse_rotor2,inverse_rotor3;
    for(int i=0;i<26;i++){
        inverse_rotor1[rotorbox[0]['A'+i]]='A'+i;
        inverse_rotor2[rotorbox[1]['A'+i]]='A'+i;
        inverse_rotor3[rotorbox[2]['A'+i]]='A'+i;
    }
    inverse_rotorbox = {inverse_rotor1,inverse_rotor2,inverse_rotor3};
}

vector<unordered_map<char,char>> Enigma::get_rotor_box(string comb){
    if(comb=="123")
        return {rotor1,rotor2,rotor3};
    else if(comb=="132")
        return {rotor1,rotor3,rotor2};
    else if(comb=="213")
        return {rotor2,rotor1,rotor3};
    else if(comb=="231")
        return {rotor2,rotor3,rotor1};
    else if(comb=="312")
        return {rotor3,rotor1,rotor2};
    else
        return {rotor3,rotor2,rotor1};
}

vector<char> Enigma::update_position(vector<char> pre_position){
    pre_position[2]=((pre_position[2]-'A'+1)%26)+'A';
    if(pre_position[2]!=rotor_notch[comb[2]-1-'0'])
        return pre_position;
    pre_position[1]=((pre_position[1]-'A'+1)%26)+'A';
    if(pre_position[1]!=rotor_notch[comb[1]-1-'0'])
        return pre_position;
    pre_position[0]=((pre_position[0]-'A'+1)%26)+'A';
    return pre_position;
}

char Enigma::encryption(vector<char> initial_position,char input){
    int rotor3_delta = initial_position[2]-ring_setting[2];
    char rotor3_out = ((rotorbox[2][((input-'A'+rotor3_delta+26)%26)+'A'] - 'A' - rotor3_delta +26))%26 + 'A';
    int rotor2_delta = initial_position[1]-ring_setting[1];
    char rotor2_out = ((rotorbox[1][((rotor3_out-'A'+rotor2_delta+26)%26)+'A'] - 'A' - rotor2_delta +26))%26 + 'A';
    int rotor1_delta = initial_position[0]-ring_setting[0];
    char rotor1_out = ((rotorbox[0][((rotor2_out-'A'+rotor1_delta+26)%26)+'A'] - 'A' - rotor1_delta +26))%26 + 'A';
    char reflector_out = reflector.at(rotor1_out);
    char rotor1_inverse_out = (inverse_rotorbox[0][((reflector_out-'A'+rotor1_delta+26)%26)+'A']-'A'-rotor1_delta+26)%26 + 'A';
    char rotor2_inverse_out = (inverse_rotorbox[1][((rotor1_inverse_out-'A'+rotor2_delta+26)%26)+'A']-'A'-rotor2_delta+26)%26 + 'A';
    char rotor3_inverse_out = (inverse_rotorbox[2][((rotor2_inverse_out-'A'+rotor3_delta+26)%26)+'A']-'A'-rotor3_delta+26)%26 + 'A';
    return rotor3_inverse_out;
}

vector<int> Enigma::generate_chain_list(vector<char> initial_position){
    vector<int> chain_list;
    vector<char> a0=initial_position;
    vector<char> a3=update_position(update_position(update_position(initial_position)));
    set<char> alpha;
    for(int i=0;i<26;i++){
        alpha.insert('A'+i);
    }
    int len;
    while(!alpha.empty()){
        char head=*alpha.begin();
        alpha.erase(head);
        len=1;
        char next_a0 = encryption(a0,head);
        char next = encryption(a3,next_a0);
        while(next != head){
            alpha.erase(next);
            next_a0 = encryption(a0,next);
            next = encryption(a3,next_a0);
            len++;
        }
        chain_list.push_back(len);
    }
    sort(chain_list.begin(),chain_list.end());
    return chain_list;
}

vector<vector<char>> Enigma::get_initp_seq(vector<char> pre_position,int num){
    vector<vector<char>> initp_seq;
    for(int i=0;i<num;i++){
        pre_position=update_position(pre_position);
        initp_seq.push_back(pre_position);
    }
    return initp_seq;
}