#pragma once

#include <boost/uuid/detail/md5.hpp>
#include <string>

#include "serializable_types.h"

class StrongHash {
public:
    StrongHashValue hash(std::string_view data);

    bool equal(const StrongHashValue &first, const StrongHashValue &second) {
       return (first[0] == second[0] &&
        first[1] == second[1] &&
        first[2] == second[2] &&
        first[3] == second[3]);
    }
};

class WeakHash {
public:
    WeakHashValue hash(std::string_view data);

private:
    uint64_t PRIME = 257;
    uint32_t BASE = 64;
};
