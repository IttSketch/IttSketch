#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <unordered_map>
#include <algorithm>

#include "abstract.h"
#include "cf_ss.h"
#include "cm_heap.h"
#include "cu_heap.h"
#include "interest.h"
#include "unbias_ss.h"
#include "ss.h"

using namespace std;

#define FILE_NUM 1
#define SKETCH_NUM 6

const string FOLDER  = "../../data/";
const string FILE_NAME[3] = {"sample.dat"};
//336,2064,2866,551
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
        /*cout << FILE_NAME[i] << endl;

        HashMap mp;
        FILE* file = fopen((FOLDER + FILE_NAME[i]).c_str(),"rb");
        Data packet;
        int t;

        while(fread(packet.str, DATA_LEN, 1, file) > 0)
        {
            fread(&t, DATA_LEN, 1, file);
            if(mp.find(packet) == mp.end())
                mp[packet] = 1;
            else
                mp[packet] += 1;
        }
        cout << Get_TopK(mp, 2000) << endl;*/

        Test_Hitter(FOLDER + FILE_NAME[i]);
    }
}

void Test_Hitter(string PATH){
    for(int i = 1;i <= 6;++i){
        int hit = 55;
        int memory = 16 + 4 * i;
        HashMap mp;
        Abstract* sketch[SKETCH_NUM];
        sketch[0] = new CM_Heap(i, hit);
        sketch[1] = new CU_Heap(i, hit);
        sketch[2] = new SS(i, hit);
        sketch[3] = new CF_SS(i, hit);
        sketch[4] = new Unbiased_SS(i, hit);
        sketch[5] = new Interest(i, hit);

        FILE* file = fopen(PATH.c_str(),"rb");
        Data packet;

        while(fread(packet.str, DATA_LEN, 1, file) > 0)
        {
            if(mp.find(packet) == mp.end())
                mp[packet] = 1;
            else
                mp[packet] += 1;

            for(int j = 0;j < SKETCH_NUM;++j){
                sketch[j]->Init(packet);
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
