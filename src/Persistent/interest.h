#ifndef PERSIST_H
#define PERSIST_H

#include <random>
#include "abstract.h"
#include "bitset.h"

using namespace std;

class Interest : public Abstract{
public:
    Interest(int num, int _HIT, uint _HASH_NUM = 3, uint _LENGTH = 80000):
        HASH_NUM(_HASH_NUM),LENGTH(_LENGTH){
        HIT = _HIT;
        record = 0;
        BUCKET_NUM = 670 + 30 * num;
        SIZE = BUCKET_NUM << 3;
        HIT = _HIT;
        bitset = new BitSet(LENGTH);

        counter = new Counter[SIZE];
        incast = new int [BUCKET_NUM];
        memset(counter, 0, SIZE*sizeof(Counter));
        memset(incast, 0, BUCKET_NUM*sizeof(int));
        aae = are = pr = cr = 0;
        name = "IttSketch(memory/20)";
    }
    ~Interest(){
        delete [] counter;
        delete [] incast;
        delete bitset;
    }

    void Init(const Data& data, uint time){
        if(time > record){
            record = time;
            bitset->Clear();
        }

        bool init = true;
        for(uint i = 0;i < HASH_NUM;++i){
            uint position = data.Hash(i) % LENGTH;
            if(!bitset->Get(position)){
                init = false;
                bitset->Set(position);
            }
        }

        if(!init){
            uint bucket_pos = data.Hash() % BUCKET_NUM;
            uint position = (bucket_pos << 3);

            int min_num = INT_MAX;
            uint min_pos = -1;
            for(uint i = 0;i < 8;++i){
                if(counter[position + i].count == 0){
                    counter[position + i].Set(data, 1);
                    return;
                }
                else if(counter[position + i].data == data){
                    counter[position + i].count += 1;
                    return;
                }

                if(counter[position + i].count < min_num){
                    min_num = counter[position + i].count;
                    min_pos = position + i;
                }
            }

            if(rd() % ( (min_num << 1) - incast[bucket_pos] + 1) == 0){
                counter[min_pos].Set(data, min_num + (incast[bucket_pos] / min_num));
                incast[bucket_pos] = 0;
            }
            else
                incast[bucket_pos] += 1;
        }

    }

    int Query(const Data &data){
        uint position = ((data.Hash() % BUCKET_NUM) << 3);
        for(uint i = 0;i < 8;++i){
            if(counter[position + i].data == data)
                return counter[position + i].count;
        }
        return -1;
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
    uint BUCKET_NUM;
    uint SIZE;
    uint record;

    Counter* counter;
    int* incast;
    random_device rd;

    BitSet* bitset;
    const uint HASH_NUM;
    const uint LENGTH;
};

#endif // PERSIST_H
