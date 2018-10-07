#ifndef SS_H
#define SS_H

#include "abstract.h"
#include "streamsummary.h"

class SS : public Abstract{
public:
    SS(int num, int _HIT):HIT(_HIT){
        SIZE = 138 + 34 * num;
        summary = new StreamSummary(SIZE);

        aae = are = pr = cr = 0;
        name = "SS";
    }
    ~SS(){
        delete summary;
    }

    void Init(const Data& data){
        if(!summary->Add_Data(data)){
            StreamCounter* ret = summary->Add_Counter(data, !summary->isFull());
            if(ret != NULL) delete ret;
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

    StreamSummary* summary;
};
#endif // SS_H
