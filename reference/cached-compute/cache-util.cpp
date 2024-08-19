#include <map>
#include <assert.h>

#include <iostream>

extern "C" {

int get_from_cache(int key, std::map<int, int> ** cache) {
    if(*cache == nullptr) {
        return 0;
    } else {
        if (auto search = (*cache)->find(key); search != (*cache)->end()) {
            int value = search->second;
            //std::cerr << "Get " << key << ": " << value << "\n";
            return value;
        } else {
            return 0;
        }
    }
}

void update_cache(int key, int value, std::map<int, int> ** cache) {
    if(*cache == nullptr) {
        *cache = new std::map<int, int>();
    }

    //std::cerr << "Cache " << key << ": " << value << "\n";

    (*cache)->emplace(key, value);
}

void clear_cache(std::map<int, int> ** cache) {
    //std::cerr << "Clear cache\n";

    if (*cache == nullptr) {
        std::cerr << "Warning: cache not found\n";
        return;
    }

    (*cache)->clear();
}

}
