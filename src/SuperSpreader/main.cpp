#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <unordered_map>
#include <algorithm>

#include "abstract.h"
#include "interest.h"
#include "opensketch.h"
#include "olf.h"
#include "tlf.h"

using namespace std;

#define FILE_NUM 1
#define SKETCH_NUM 4

const string FOLDER  = "../../data/";
const string FILE_NAME[4] = {"sample.dat"};
//67,68,66,70
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
        StreamMap sp;

        FILE* file = fopen((FOLDER + FILE_NAME[i]).c_str(),"rb");
        Data from;
        Data to;

        while(fread(from.str, DATA_LEN, 1, file) > 0)
        {
            fread(&to.str, DATA_LEN, 1, file);

            Stream stream(from, to);
            if(sp.find(stream) == sp.end()){
                sp[stream] = 1;
                if(mp.find(from) == mp.end())
                    mp[from] = 1;
                else
                    mp[from] += 1;
            }
        }
        cout << Get_TopK(mp, 250) << endl;
        cout << mp.size() << endl;
        cout << sp.size() << endl;*/

        Test_Hitter(FOLDER + FILE_NAME[i]);
    }
}

void Test_Hitter(string PATH){
    for(int i = 1;i <= 6;++i){
        double memory = 45 + 5 * i;
        Abstract* sketch[SKETCH_NUM];
        sketch[0] = new OpenSketch(i, 20);
        sketch[1] = new Interest(i, 20);
        sketch[2] = new OLF(i, 20);
        sketch[3] = new TLF(i, 20);

        HashMap mp;
        StreamMap sp;

        FILE* file = fopen(PATH.c_str(),"rb");
        Data from;
        Data to;
        int num = 0;

        while(fread(from.str, DATA_LEN, 1, file) > 0)
        {
            fread(to.str, DATA_LEN, 1, file);

            Stream stream(from, to);
            if(sp.find(stream) == sp.end()){
                sp[stream] = 1;
                if(mp.find(from) == mp.end())
                    mp[from] = 1;
                else
                    mp[from] += 1;
            }

			for(int i = 0;i < SKETCH_NUM;++i){
				sketch[i]->Init(from, to);
			}
        }
        fclose(file);

		for(int i = 0;i < SKETCH_NUM;++i){
			sketch[i]->Check(mp);
		}

		for(int i = 0;i < SKETCH_NUM;++i){
			sketch[i]->print_aae(memory);
		}
		for(int i = 0;i < SKETCH_NUM;++i){
			sketch[i]->print_are(memory);
		}
		for(int i = 0;i < SKETCH_NUM;++i){
			sketch[i]->print_cr(memory);
		}
		for(int i = 0;i < SKETCH_NUM;++i){
			sketch[i]->print_pr(memory);
		}

    }
}
