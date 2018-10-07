#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <unordered_map>
#include <algorithm>

#include "fr.h"
#include "fr_cf.h"
#include "abstract.h"
#include "interest.h"

using namespace std;

#define FILE_NUM 1
#define SKETCH_NUM 6

const string FOLDER  = "../../data/";
const string FILE_NAME[4] = {"sample.dat"};


const int hit = 170;
const int interval = 650000;
//30,1707,693,826
//800000,10000000,10000000,5000000
void Test_Hitter(string PATH);

int Get_TopK(HashMap mp, int k){
    int size = mp.size();
    int *num = new int [size];
    int pos = 0;
    unordered_map<Data, int, My_Hash>::iterator it;
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
        //cout << "Dataset,Memory(KB),Speed(Mops)" << endl;
/*
        cout << FILE_NAME[i] << endl;

        HashMap mp;
        FILE* file = fopen((FOLDER + FILE_NAME[i]).c_str(),"rb");
        Data packet;
        int t;
        int num = 0;

        while(num < interval)
        {
            fread(packet.str, DATA_LEN, 1, file);
            //fread(&t, DATA_LEN, 1, file);
            if(mp.find(packet) == mp.end())
                mp[packet] = 1;
            else
                mp[packet] += 1;
            num++;
        }
        cout << mp.size() << endl;

        while(num < 2 * interval)
        {
            fread(packet.str, DATA_LEN, 1, file);
            //fread(&t, DATA_LEN, 1, file);
            if(mp.find(packet) == mp.end())
                mp[packet] = -1;
            else
                mp[packet] -= 1;
            num++;
        }
        cout << mp.size() << endl;


        for(HashMap::iterator it = mp.begin();it != mp.end();++it){
            it->second = abs(it->second);
        }

        cout << Get_TopK(mp, 500) << endl;
*/

        Test_Hitter(FOLDER + FILE_NAME[i]);
    }
}

void Test_Hitter(string PATH){
    for(int i = 1;i <= 6;++i){
        double memory = 0.15 + 0.05 * i;
        HashMap mp;
        Abstract* sketch[SKETCH_NUM];
        sketch[0] = new Interest(i, hit);
        sketch[1] = new FR(i, hit);
        sketch[2] = new FR_CF(i, hit);
        sketch[3] = new Interest(i, hit);
        sketch[4] = new FR(i, hit);
        sketch[5] = new FR_CF(i, hit);

        FILE* file = fopen(PATH.c_str(),"rb");
        Data packet;
        int num = 0;

        while(num < interval)
        {
            fread(packet.str, DATA_LEN, 1, file);
            if(mp.find(packet) == mp.end())
                mp[packet] = 1;
            else
                mp[packet] += 1;
            num++;

            for(int j = 0;j < SKETCH_NUM / 2;++j){
                sketch[j]->Init(packet);
            }
        }

        while(num < 2 * interval)
        {
            fread(packet.str, DATA_LEN, 1, file);
            if(mp.find(packet) == mp.end())
                mp[packet] = -1;
            else
                mp[packet] -= 1;
            num++;

            for(int j = SKETCH_NUM / 2;j < SKETCH_NUM;++j){
                sketch[j]->Init(packet);
            }
        }
        fclose(file);

        for(int j = 0;j < SKETCH_NUM / 2;++j){
            sketch[j]->Check(mp, sketch[j+3]);
        }

        for(int j = 0;j < SKETCH_NUM / 2;++j){
            sketch[j]->print_cr(memory);
        }

        for(int j = 0;j < SKETCH_NUM / 2;++j){
            sketch[j]->print_pr(memory);
        }

        for(int j = 0;j < SKETCH_NUM;++j){
            delete sketch[j];
        }

    }
}
