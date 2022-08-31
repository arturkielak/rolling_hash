#pragma once

#include "file.h"
#include "signature_builder.h"

class DeltaBuilder {
public:
    //TODO For large files (size file > 10G) we can use pool of threads to read chunks independent for faster calculation delta
    //Example:
    // file content : ABCDEFHIJKLAMNOPZXYZWTRE
    // block size 4
    // pool size 4 thread depends cpu core's
    // 1 thread calculate ABCD
    // 2 thread calculate EFHI
    // 3 thread calculate JKLA
    // 4 thread calculate MNOP
    // 1 thread calculate ZXYZ
    // 2 thread calculate WTRE
    //TODO read bigger chunks to minimize read from disk
    static Deltas
    build(Signatures &signatures, File &file, uint32_t block_size = BLOCK_SIZE, StrongHash strong = StrongHash(),
          WeakHash weak = WeakHash());

};

