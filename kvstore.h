#pragma once
#include <string>
#include "kvstore_api.h"
#include "MemTable.h"
#include "utils.h"
#include "SStable.h"
#include "MurmurHash3.h"
#include <list>

class KVStore : public KVStoreAPI {
	// You can add your implementation here
private:
    std::string  workingdir;
    MemTable *cur_memtab;
    std::list<SStable*> sstable;
    uint64_t timestamp;
    int level;
    int index;

public:
	KVStore(const std::string &dir);

	~KVStore();

	void put(uint64_t key, const std::string &s) override;

	std::string get(uint64_t key) override;

	bool del(uint64_t key) override;

	void reset() override;

	void toSStable();

	std::string getpath(){
	    auto path = workingdir+"/level_"+std::to_string(level);
	    return path;
	}

};
