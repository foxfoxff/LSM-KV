#include "kvstore.h"


KVStore::KVStore(const std::string &dir): KVStoreAPI(dir),level(0),index(0),timestamp(0)
{
    workingdir=dir;
    if(!utils::dirExists(dir)){
        utils::mkdir(dir.c_str());
    }
    cur_memtab=new MemTable();
}

KVStore::~KVStore()
{
}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const std::string &s)
{
    cur_memtab->put(key,s);
}
/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key)
{
    string *rel = cur_memtab->get(key);
	return *rel;
}
/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key)
{
    return cur_memtab->remove(key);

}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset()
{
}


void KVStore::toSStable() {
    auto path=getpath();
    if(!utils::dirExists(path))
        mkdir(path.c_str());

    SStable *_sstable = new SStable(workingdir,timestamp,cur_memtab->size(),cur_memtab->getkeys());

    sstable.push_back(_sstable);

}
