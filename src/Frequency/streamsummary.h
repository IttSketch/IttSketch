#ifndef STREAMSUMMARY_H
#define STREAMSUMMARY_H

#include "data.h"
#include "definition.h"
#include <iostream>
#include <unordered_map>

using namespace std;

// node for double linked list
template<class T, class P>
class Node{
public:
    T ID;
    P* parent;
    Node* prev;
    Node* next;

    Node(T temp){
        ID = temp;
        prev = NULL;
        next = NULL;
        parent = NULL;
    }

    void Delete(){
        Connect(prev, next);
    }

    void Connect(Node* prev, Node* next){
        if(prev != NULL)
            prev->next = next;
        if(next != NULL)
            next->prev = prev;
    }
};

class StreamCounter;
class StreamBucket;

class StreamCounter: public Node<Data, StreamBucket>{
public:
    StreamCounter(const Data& data):Node(data){}
};

class StreamBucket: public Node<int, StreamCounter>{
public:
    StreamBucket(int num):Node(num){}
};

class StreamSummary{
public:
    StreamSummary(uint _MAX_SIZE):MAX_SIZE(_MAX_SIZE){
        map.clear();
        first = new StreamBucket(0);
    }
    ~StreamSummary(){
        StreamBucket* temp = first;
        while(temp->next != NULL){
            Delete_Bucket(temp);
            temp = (StreamBucket*)temp->next;
            delete temp->prev;
        }
        Delete_Bucket(temp);
        delete temp;
    }

    bool isFull(){
        return map.size() >= MAX_SIZE;
    }
    // min_num of all the node
    int Min_Num(){
        if(!isFull())
            return 0;
        else
            return first->next->ID;
    }

    int Size(int num){
        int ret = 0;
        unordered_map<Data, StreamCounter*, My_Hash>::iterator it;
        for(it = map.begin();it != map.end();++it){
            if(it->second->parent->ID > num){
                ret++;
            }
        }
        return ret;
    }

    int Query(const Data& data){
        if(map.find(data) != map.end())
            return map[data]->parent->ID;
        else
            return -1;
    }

    //add the min counter (used for unbias)
    void Add_Min(){
        Add_Data(first->next->parent->ID);
    }

    //whether the data is in unordered_map
    bool Add_Data(const Data& data){
        if(map.find(data) == map.end())
            return false;

        bool del = false;
        StreamCounter* temp = map[data];
        StreamBucket* prev = temp->parent;
        temp->Connect(temp->prev, temp->next);
        if(prev->parent == temp){
            prev->parent = (StreamCounter*)temp->next;
            if(temp->next == NULL)
                del = true;
        }

        map.erase(temp->ID);
        Add_Bucket(temp->parent, temp);
        map[temp->ID] = temp;

        if(del){
            prev->Delete();
            delete prev;
        }

        return true;
    }

    //if not in map, add to the list
    StreamCounter* Add_Counter(const Data& data, bool empty){
        StreamBucket* prev;
        if(empty)
            prev = first;
        else
            prev = (StreamBucket*)first->next;

        StreamCounter* add = new StreamCounter(data);
        Add_Bucket(prev, add);
        map[data] = add;

        if(empty)
            return NULL;
        else
            return Remove();
    }

private:
    const uint MAX_SIZE;
    StreamBucket* first;
    unordered_map<Data, StreamCounter*, My_Hash> map;

    // remove the min node in the streamsummary
    StreamCounter* Remove(){
        StreamCounter* temp = first->next->parent;
        first->next->parent = (StreamCounter*)temp->next;
        map.erase(temp->ID);
        temp->Delete();
        if(temp->next == NULL){
            Node<int, StreamCounter>* bucket = first->next;
            bucket->Delete();
            delete bucket;
        }
        return temp;
    }
    //delete all the mode in the bucket
    void Delete_Bucket(StreamBucket* bucket){
        if(bucket->parent != NULL){
            StreamCounter* temp = bucket->parent;
            if(temp->next != NULL){
                map.erase(temp->ID);
                temp = (StreamCounter*)temp->next;
                delete temp->prev;
            }
            map.erase(temp->ID);
            delete temp;
        }
    }
    //new a new counter next to prev, and add counter to new bucket
    void Add_Bucket(StreamBucket* prev, StreamCounter* counter){
        if(prev->next == NULL){
            prev->Connect(prev, new StreamBucket(prev->ID + 1));
        }
        else if(prev->next->ID - prev->ID > 1){
            StreamBucket* temp = (StreamBucket*)prev->next;
            StreamBucket* add = new StreamBucket(prev->ID + 1);
            prev->Connect(prev, add);
            add->Connect(add, temp);
        }

        Counter2Bucket(counter, (StreamBucket*)prev->next);
    }
    //add counter to bucket
    void Counter2Bucket(StreamCounter* counter, StreamBucket* bucket){
        StreamCounter* temp = bucket->parent;
        bucket->parent = counter;
        counter->prev = NULL;
        counter->parent = bucket;
        counter->Connect(counter, temp);
    }
};

#endif // STREAMSUMMARY_H
