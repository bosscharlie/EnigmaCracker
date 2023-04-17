#include"EnigmaCracker.h"

void generate_cracker_table(){
    tablefile = fopen("cracker_table.txt","w");
    vector<string> combs = {"123","132","213","231","312","321"};
    int cnt=0;
    for(auto comb: combs){
        fprintf(tablefile,"%s:{\n",comb.c_str());
        vector<char> initial_position={'A','A','A'};
        Enigma enigma(comb);
        while(true){
            fprintf(tablefile,"%c%c%c:",initial_position[0],initial_position[1],initial_position[2]);
            cnt++;
            initial_position=enigma.update_position(initial_position);
            vector<int> chain_list = enigma.generate_chain_list(initial_position);

            fprintf(tablefile,"(");
            for(int i=0;i<chain_list.size()-1;i++){
                fprintf(tablefile,"%d,",chain_list[i]);
            }
            fprintf(tablefile,"%d",chain_list[chain_list.size()-1]);
            fprintf(tablefile,")\n");

            if(initial_position[0]=='A' && initial_position[1]=='A' && initial_position[2]=='A')
                break;
        }
        fprintf(tablefile,"}\n");
    }
    fclose(tablefile);
}

void load_cracker_table(){
    tablefile = fopen("cracker_table.txt","r");
    for(int i=0;i<6;i++){
        string comb;
        comb.resize(3);
        fscanf(tablefile,"%s",&comb[0]);
        unordered_map<string,string> chain_table;
        for(int j=0;j<26*26*26;j++){
            string chain;
            chain.resize(40);
            fscanf(tablefile,"%s",&chain[0]);
            chain.resize(chain.find_first_of(')')+1);
            string initial_position=chain.substr(0,chain.find_first_of(':'));
            string chain_list=chain.substr(chain.find_first_of(':')+1,chain.length());
            chain_table.insert(make_pair(initial_position,chain_list));
            query_table[comb][chain_list].push_back(initial_position);
        }
        chain_tables.insert(make_pair(comb,chain_table));
        fscanf(tablefile,"%s",&comb[0]);
    }
    fclose(tablefile);
}

void rejewski_crack(){
    load_cracker_table();
    for(auto kv:query_table){
        vector<string> possible_list = kv.second[chain1];
        for(string possible_initp : possible_list){
            Enigma enigma(kv.first);
            vector<char> nextp = enigma.update_position({possible_initp[0],possible_initp[1],possible_initp[2]});
            string next_position(nextp.begin(),nextp.end());
            string possible_chain2 = chain_tables[kv.first][next_position];
            if(strcmp(chain2.c_str(),possible_chain2.c_str())!=0)
                continue;
            nextp = enigma.update_position(nextp);
            next_position=string(nextp.begin(),nextp.end());
            string possible_chain3 = chain_tables[kv.first][next_position];
            if(strcmp(chain3.c_str(),possible_chain3.c_str())!=0)
                continue;
            cout<<"Possible Solution: Rotors Order: "<<kv.first<<" Initial Position: "<<possible_initp<<endl;
        }
    }
}

void dfs(int root,vector<bool> visited,vector<int> path){
    for(int node : neighbors[root]){
        if(visited[node]==true){
            if(path.end()-find(path.begin(),path.end(),node)>=3){
                for(auto it=find(path.begin(),path.end(),node);it!=path.end();it++){
                    cout<<items[*it]<<" ";
                }
                cout<<endl;
            }
            continue;
        }
        visited[node]=true;
        vector<int> newpath=path;
        newpath.push_back(node);
        dfs(node,visited,newpath);
    }
}

void find_loop(){
    for(int i=0;i<plaintext.length();i++){
        neighbors[item_index[plaintext[i]]].insert(item_index[ciphertext[i]]);
        neighbors[item_index[ciphertext[i]]].insert(item_index[plaintext[i]]);
    }
    vector<bool> visited(item_index.size());
    visited[0]=true;
    vector<int> path;
    path.push_back(0);
    cout<<"All cycles:"<<endl;
    dfs(0,visited,path);
}

void* turing_crack_percomb(void* combarg){
    string comb = *(string *)combarg;
    timeval start_time,end_time,duration;
    gettimeofday(&start_time,NULL);
    vector<char> initial_position={'A','A','A'};
    Enigma enigma(comb);
    vector<vector<char>> possible_initp;
    while(true){
        vector<vector<char>> initp=enigma.get_initp_seq(initial_position,plaintext.length());
        for(int i=0;i<25;i++){
            char text=i+'A';
            bool satisfied=true;
            for(int j=0;j<cycles[0].second.size();j++){
                vector<int> cycle=cycles[0].second[j];
                for(int index:cycle){
                    text=enigma.encryption(initp[index],text);
                }
                if(text!=i+'A')
                    satisfied=false;
            }
            if(satisfied){
                possible_initp.push_back({initial_position[0],initial_position[1],initial_position[2],char('A'+i)});
                pthread_mutex_lock(&mu);
                cout<<"Possible Solution: Rotors Order: "<<comb<<" Initial Position: "<<initial_position[0]<<initial_position[1]<<initial_position[2]<<" Letter for J:"<<char('A'+i)<<endl;
                pthread_mutex_unlock(&mu);
            }
        }
        initial_position=enigma.update_position(initial_position);
        if(initial_position[0]=='A' && initial_position[1]=='A' && initial_position[2]=='A')
            break;
    }

    bool valid_char[26];
    memset(valid_char,1,sizeof(valid_char));
    for(vector<char> initial_position:possible_initp){
        vector<vector<char>> initp=enigma.get_initp_seq(initial_position,plaintext.length());
        valid_char[initial_position[3]-'A']=false;
        valid_char[cycles[0].first-'A']=false;
        for(int i=1;i<cycles.size();i++){
            bool cycle_satisfied=false;
            for(int j=0;j<26;j++){
                bool satisfied=true;
                char text='A'+j;
                for(int k=0;k<cycles[i].second.size();k++){
                    vector<int> cycle=cycles[i].second[k];
                    for(int index:cycle){
                        text=enigma.encryption(initp[index],text);
                    }
                    if(text!='A'+j){
                        satisfied=false;
                        break;
                    }
                }
                if(satisfied){
                    cycle_satisfied=true;
                    if(i==cycles.size()-1){
                        pthread_mutex_lock(&mu);
                        cout<<"Final Possible Solution: Rotors Order: "<<comb<<" Initial Position: "<<initial_position[0]<<initial_position[1]<<initial_position[2]<<" Letter for "<<cycles[0].first<<":"<<initial_position[3]<<endl;
                        pthread_mutex_unlock(&mu);
                        break;
                    }
                }
            }
            if(!cycle_satisfied)
                break;
        }
    }

    gettimeofday(&end_time,NULL);
    timersub(&end_time,&start_time,&duration);
    pthread_mutex_lock(&mu);
    cout<<"Thread for comb "<<comb<<" Running Time: "<<duration.tv_sec<<"."<<duration.tv_usec<<endl;
    pthread_mutex_unlock(&mu);
    pthread_exit(NULL);
}

void turing_crack(){
    pthread_t threads[NUM_THREADS];
    vector<string> combs = {"123","132","213","231","312","321"};
    int code;
    for(int i=0;i<combs.size();i++){
        code=pthread_create(&threads[i],NULL,turing_crack_percomb,(void*)&combs[i]);
        if(code){
            cout<<"Error when create thread "<<i<<endl;
            exit(-1);
        }
    }
    pthread_exit(NULL);
}

int main(){
    /* Rejewski method */
    cout<<"Rejewski method:"<<endl;
    generate_cracker_table();
    rejewski_crack();
    /* Turing method */
    cout<<"Turing method:"<<endl;
    find_loop();
    turing_crack();
    return 0;
}