#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <unordered_map>
#include <algorithm>

#include "abstract.h"
#include "pie.h"
#include "interest.h"
#include "small_space.h"

using namespace std;

#define FILE_NUM 1
#define SKETCH_NUM 3

const string FOLDER  = "../../data/";
const string FILE_NAME[4] = {"sample.dat"};
//255,303,564,1094
//6250,10000,19537,20000(1600)
//stack,015,ip,web
void Test_Hitter(string PATH);

int Get_TopK(HashMap mp, int k){
    int size = mp.size();
    int *num = new int [size];
    int pos = 0;
    HashMap::iterator it;
    for(it = mp.begin();it != mp.end();++it){
        num[pos++] = it->second;
    }
    nth_element(num, num + size - k, num + size);
    int ret = num[size - k];
    delete []num;
    return ret;
}

int main(){
    for(int i = 0;i < FILE_NUM;++i){
        /*cout << FILE_NAME[i] << endl;

        HashMap mp;
        HashMap cycle;
        FILE* file = fopen((FOLDER + FILE_NAME[i]).c_str(),"rb");
        Data packet;
        uint num = 0;
        uint time = 0;
        uint t = 0;

        while(fread(packet.str, DATA_LEN, 1, file) > 0)
        {
            fread(&t, DATA_LEN, 1, file);
            if(num % 6250 == 0){
                time += 1;
                cycle.clear();
            }
            num++;
            if(mp.find(packet) == mp.end()){
                mp[packet] = 1;
                cycle[packet] = 1;
            }
            else if(cycle.find(packet) == cycle.end()){
                cycle[packet] = 1;
                mp[packet] += 1;
            }

        }
        cout << Get_TopK(mp, 2000) << endl;
        cout << time << endl;*/

        Test_Hitter(FOLDER + FILE_NAME[i]);
    }
}

void Test_Hitter(string PATH){
    for(int i = 1;i <= 6;++i){
        double memory = 1.54 + 0.06 * i;
        Abstract* sketch[SKETCH_NUM];
        sketch[0] = new Interest(i, 56);
        sketch[1] = new Small_Space(i, 56);
        sketch[2] = new PIE(i, 56, 1600);
        HashMap mp;
        HashMap cycle;
        FILE* file = fopen(PATH.c_str(),"rb");
        Data packet;
        uint num = 0;
        uint time = 0;

        while(fread(packet.str, DATA_LEN, 1, file) > 0)
        {
            if(num % 6556 == 0){
                time += 1;
                cycle.clear();
            }
            num++;

            if(mp.find(packet) == mp.end()){
                mp[packet] = 1;
                cycle[packet] = 1;
            }
            else if(cycle.find(packet) == cycle.end()){
                cycle[packet] = 1;
                mp[packet] += 1;
            }

            for(int j = 0;j < SKETCH_NUM;++j){
                sketch[j]->Init(packet, time);
            }
        }

        fclose(file);

        for(int j = 0;j < SKETCH_NUM;++j){
            sketch[j]->Check(mp);
        }

        for(int j = 0;j < SKETCH_NUM;++j){
            sketch[j]->print_aae(memory);
        }

        for(int j = 0;j < SKETCH_NUM;++j){
            sketch[j]->print_are(memory);
        }

        for(int j = 0;j < SKETCH_NUM;++j){
            sketch[j]->print_cr(memory);
        }

        for(int j = 0;j < SKETCH_NUM;++j){
            sketch[j]->print_pr(memory);
        }

        for(int j = 0;j < SKETCH_NUM;++j){
            delete sketch[j];
        }

    }
}

