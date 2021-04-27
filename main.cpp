//
// Created by huyao on 2021/4/23.
//
#include <iostream>
#include "kvstore.h"


int main(){
    KVStore testTree("what");
    testTree.put(1,"l");
    testTree.del(1);
    testTree.del(2);
    uint64_t key=55555;
    std::to_string(3.1415926);
    testTree.toSStable();
    /*unsigned int hash[4] = {0};
    MurmurHash3_x64_128(&key, sizeof(key), 1, hash);*/
      return 0;
}