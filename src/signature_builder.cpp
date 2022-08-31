#include "signature_builder.h"

Signatures
SignatureBuilder::build(File &file, uint32_t block_size, StrongHash strong, WeakHash weak) {
    Signatures signatures;
    uint32_t offset = 0;
    auto buffer = std::make_unique<char[]>(block_size);
    size_t bytes = 0;
    while ((bytes = file.read(buffer.get(), block_size)) > 0) {
        const std::string_view data = {buffer.get(), bytes};
        signatures.add_signature(Signature{weak.hash(data), strong.hash(data), offset});
        offset += bytes;
    }
    return signatures;
}