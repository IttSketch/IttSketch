#ifndef CF_SS_H
#define CF_SS_H

#include "abstract.h"
#include "streamsummary.h"
#include <random>
#include <iostream>

using namespace std;

class CF{
public:
    const int THRESHOLD;

    CF(uint _HASH_NUM, uint _LENGTH, int _THRESHOLD):
        HASH_NUM(_HASH_NUM), LENGTH(_LENGTH), THRESHOLD(_THRESHOLD){
        if(LENGTH != 0){
            counter = new uchar [LENGTH];
            memset(counter, 0 ,sizeof(uchar)*LENGTH);
        }
    }
    ~CF(){
        if(LENGTH != 0)
            delete []counter;
    }

    bool Init(const Data& data){
        if(LENGTH == 0)
            return true;

        int min_num = INT_MAX;
        uint* position = new uint [HASH_NUM];
        for(uint i = 0;i < HASH_NUM;++i){
            position[i] = data.Hash(i + 1) % LENGTH;
            min_num = MIN(counter[position[i]], min_num);
        }

        if(min_num >= THRESHOLD){
            delete []position;
            return true;
        }

        for(uint i = 0;i < HASH_NUM;++i){
            if(counter[position[i]] == min_num){
                counter[position[i]] += 1;
            }
        }

        delete []position;
        return false;
    }

private:
    const uint HASH_NUM;
    const uint LENGTH;

    uchar* counter;
};

class CF_SS : public Abstract{
public:
    CF_SS(int num, int _HIT):HIT(_HIT){
        SIZE = 130 + 34 * num;
        filter = new CF(2, 1856, 15);
        summary = new StreamSummary(SIZE);
        aae = are = pr = cr = 0;
        name = "SS+CF";
    }
    ~CF_SS(){
        delete summary;
        delete filter;
    }

    void Init(const Data& data){
        if(filter->Init(data)){
            if(!summary->Add_Data(data)){
                if(summary->isFull()){
                    if(rd() % (summary->Min_Num() + 1) == 0){
                        summary->Add_Counter(data, false);
                    }
                    else{
                        summary->Add_Min();
                    }
                }
                else{
                    summary->Add_Counter(data, true);
                }
            }
        }
    }

    int Query(const Data &data){
        int value = summary->Query(data);
        if(value == -1)
            return -1;
        else
            return value + 15;
    }

    void Check(HashMap mp){
        HashMap::iterator it;
        int value = 0, all = 0, hit = 0, size = 0;
        for(it = mp.begin();it != mp.end();++it){
            value = Query(it->first);
            if(it->second > HIT){
                all++;
                if(value > HIT){
                    hit += 1;
                    aae += abs(it->second - value);
                    are += abs(it->second - value) / (double)it->second;
                }
            }
            if(value > HIT)
                size += 1;
        }
        aae /= hit; are /= hit; cr = hit / (double)all;
        pr = hit / (double)size;
    }


private:
    struct Counter{
        Data data;
        int count;
        void Set(const Data& dat, int num){
            data = dat;
            count = num;
        }
    };
    uint SIZE;
    const int HIT;

    StreamSummary* summary;
    random_device rd;
    CF* filter;
};

#endif // CF_SS_H
