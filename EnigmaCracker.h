#ifndef EC_H
#define EC_H
#define NUM_THREADS 6

#include"Enigma.h"
#include<iostream>
#include<sys/time.h>
#include<pthread.h>
#include<cstring>

using namespace std;

pthread_mutex_t mu=PTHREAD_MUTEX_INITIALIZER;

string chain1="(1,1,1,1,5,5,6,6)";
string chain2="(2,2,3,3,4,4,4,4)";
string chain3="(13,13)";

//0:A 1:B 2:C 3:D 4:F 5:J 6:K 7:O 8:Q 9:S 10:T 11:W 12:X
string plaintext= "ASTWBKDJXSAXQFCJT";
string ciphertext="STWTJDJXSAQBJQJOD";
unordered_map<char,int> item_index{{'A',0},{'B',1},{'C',2},{'D',3},{'F',4},{'J',5},{'K',6},{'O',7},{'Q',8},{'S',9},{'T',10},{'W',11},{'X',12}};
vector<char> items={'A','B','C','D','F','J','K','O','Q','S','T','W','X'};
vector<set<int>> neighbors(item_index.size());
vector<pair<char,vector<vector<int>>>> cycles={
        {'J',{{4,11,7},{4,11,8,0,10,12},{4,11,8,1,16,6},{6,16,1,0,10,12},{6,16,1,8,7},{7,8,9,10,12}}},
        {'W',{{2,3}}},
        {'A',{{0,9}}},
};

FILE *tablefile;

unordered_map<string,unordered_map<string,string>> chain_tables;

unordered_map<string,unordered_map<string,vector<string>>> query_table;

void generate_cracker_table();

void load_cracker_table();

void rejewski_crack();

void find_loop();

void turing_crack();

#endif