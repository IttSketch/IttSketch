#ifndef UNBIAS_SS_H
#define UNBIAS_SS_H

#include <random>
#include "abstract.h"
#include "streamsummary.h"

using namespace std;

class Unbiased_SS : public Abstract{
public:
    Unbiased_SS(int num, int _HIT):HIT(_HIT){
        SIZE = 138 + 34 * num;
        summary = new StreamSummary(SIZE);

        aae = are = pr = cr = 0;
        name = "Unbiased-SS";
    }
    ~Unbiased_SS(){
        delete summary;
    }

    void Init(const Data& data){
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

    int Query(const Data &data){
        return summary->Query(data);
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
    random_device rd;

    StreamSummary* summary;
};

#endif // UNBIAS_SS_H
