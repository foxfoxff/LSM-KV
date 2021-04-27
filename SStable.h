//
// Created by huyao on 2021/4/26.
//

#ifndef LSM_KV_SSTABLE_H
#define LSM_KV_SSTABLE_H

#include <string>
#include "MurmurHash3.h"
#include <vector>
const   int BFSIZE=10240;
class BloomFilter{
public:
    uint8_t bytes[BFSIZE];
    void put(uint64_t key){
        unsigned int hash[4] = {0};
        MurmurHash3_x64_128(&key, sizeof(key), 1, hash);
        for(uint32_t item : hash){
            setbit(item);
        }
    }
    void setbit(uint32_t index){
        int byteindex = index%BFSIZE;
        int bitindex =index%8;
        bytes[byteindex]=(1<<bitindex);
    }
};


class SStable{
private:
    struct Header{
        std::string dir;
        uint64_t timestamp;
        uint64_t num;
        uint64_t _max;
        uint64_t _min;
        Header(std::string dir,uint64_t timestamp,uint64_t num):dir(dir),timestamp(timestamp),num(num){
            _max=0;
            _min=0;
        }

    };
    struct key_offsets{
        uint64_t key;
        uint32_t offsets;
        key_offsets( uint64_t key,uint32_t offset):key(key),offsets(offset){}
    };
    BloomFilter bloomFilter;
    Header header;
    std::vector<key_offsets> keypairs;
public:
   SStable(std::string dir,uint64_t timestamp,uint64_t num,Node*keys):header(dir,timestamp,num)
    {

        header._min=keys->key;
        bloomFilter.put(header._min);
       // keypairs.emplace_back(keys->key,1);
        while(!keys->right){
            keys=keys->right;
            bloomFilter.put(keys->key);
        }
        header._max=keys->key;
    }


};
#endif //LSM_KV_SSTABLE_H
