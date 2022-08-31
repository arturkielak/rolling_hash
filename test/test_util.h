#pragma once

#include "file.h"
#include "serializable_types.h"
#include "hash.h"
#include "delta_builder.h"

File get_file(const std::string &name, const std::string &content) {
    File file(name, std::ios_base::out | std::ios_base::in | std::ios_base::trunc);
    file.write(content.data(), content.length());
    return file;
}

Signatures get_signatures(const std::string &buffer) {
    Signatures signatures;
    uint64_t offset = 0;
    for (int i = 0; i < buffer.length(); i += BLOCK_SIZE) {
        std::string_view data = buffer.substr(i, BLOCK_SIZE);
        signatures.add_signature(Signature{WeakHash().hash(data), StrongHash().hash(data), offset});
        offset += BLOCK_SIZE;
    }
    return signatures;
}

Deltas get_deltas(Signatures &signatures, const std::string &buffer) {
    File file = get_file("./new_file", buffer);
    file.seek(std::ios_base::beg);
    return DeltaBuilder::build(signatures, file);
}