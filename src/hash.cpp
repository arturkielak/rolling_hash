#include "hash.h"

StrongHashValue StrongHash::hash(std::string_view data) {
    using boost::uuids::detail::md5;
    md5 md5_hash;
    md5::digest_type digest;
    md5_hash.process_bytes(data.data(), data.size());
    md5_hash.get_digest(digest);

    return {digest[0], digest[1], digest[2], digest[3]};
}

WeakHashValue WeakHash::hash(std::string_view data) {
    uint64_t hash_ = 0;
    for (uint64_t i = 0; i < data.length(); i++) {
        int ascii_code = (islower(data[i])) ? data[i] - 'A' + 27 : data[i] - 'a' + 1;
        hash_ = (hash_ * BASE + ascii_code) % PRIME;
    }
    return hash_;
}