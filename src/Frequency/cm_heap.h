#ifndef CM_HEAP_H
#define CM_HEAP_H

#include "abstract.h"

using namespace std;

class CM_Heap : public Abstract{
public:
    CM_Heap(int num, int _HIT):HIT(_HIT){
        heap_num = 0;
        CAPACITY = 160 + 40 * num;
        LENGTH = 320 + 80 * num;
        HASH_NUM = 2;
        counter = new int [LENGTH];
        heap = new KV[CAPACITY];
        memset(counter, 0 ,sizeof(int) * LENGTH);
        memset(heap, 0, sizeof(KV) * CAPACITY);
        aae = are = pr = cr = 0;
        name = "CM+Heap";
    }
    ~CM_Heap(){
        delete [] counter;
        delete [] heap;
    }

    void Init(const Data& data){
        int min_num = INT_MAX;
        for(uint i = 0;i < HASH_NUM;++i){
            uint position = data.Hash(i) % LENGTH;
            counter[position] += 1;
            min_num = min(min_num, counter[position]);
        }

        if(mp.find(data) != mp.end()){
            heap[mp[data]].first ++;
            heap_down(mp[data]);
        }
        else if(heap_num < CAPACITY){
            heap[heap_num].second = data;
            heap[heap_num].first = min_num;
            mp[data] = heap_num++;
            heap_up(heap_num - 1);
        }
        else if(min_num > heap[0].first){
            KV &kv = heap[0];
            mp.erase(kv.second);
            kv.second = data;
            kv.first = min_num;
            mp[data] = 0;
            heap_down(0);
        }
    }

    int Query(const Data &data){
        if(mp.find(data) == mp.end())
            return -1;
        else return heap[mp[data]].first;
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
    uint HASH_NUM;
    uint LENGTH;
    uint CAPACITY;
    const int HIT;
    int heap_num;

    int* counter;
    HashMap mp;
    KV* heap;

    void heap_down(int i){
        while (i < heap_num / 2) {
            int l_child = 2 * i + 1;
            int r_child = 2 * i + 2;
            int larger_one = i;
            if (l_child < heap_num && heap[l_child] < heap[larger_one]) {
                larger_one = l_child;
            }
            if (r_child < heap_num && heap[r_child] < heap[larger_one]) {
                larger_one = r_child;
            }
            if (larger_one != i) {
                swap(heap[i], heap[larger_one]);
                swap(mp[heap[i].second], mp[heap[larger_one].second]);
                heap_down(larger_one);
            } else {
                break;
            }
        }
    }

    void heap_up(int i) {
        while (i > 1) {
            int parent = (i - 1) / 2;
            if (heap[parent] <= heap[i]) {
                break;
            }
            swap(heap[i], heap[parent]);
            swap(mp[heap[i].second], mp[heap[parent].second]);
            i = parent;
        }
    }
};

#endif // CM_HEAP_H
