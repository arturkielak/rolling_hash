#include <iostream>
#include "delta_builder.h"

Deltas
DeltaBuilder::build(Signatures &signatures, File &file, uint32_t block_size, StrongHash strong,
                    WeakHash weak) {
    Deltas deltas;
    auto buffer = std::make_unique<char[]>(block_size);
    uint32_t bytes = 0;
    uint32_t offset = 0;

    while ((bytes = file.read(buffer.get(), block_size)) > 0) {
        const std::string_view data = {buffer.get(), bytes};
        uint64_t weak_hash = weak.hash(data);
        Action action;
        if (Signature *signature = signatures.find(weak_hash); signature != nullptr) {
            if ((strong.equal(strong.hash(data), signature->strong_hash()))) {
                if (signature->offset() != offset) {
                    action = Action::MOVE;
                } else {
                    action = Action::SKIP;
                }
            } else {
                //insert new block when is false positive on strong hash
                action = Action::INSERT;
            }
        } else {
            action = Action::INSERT;
        }

        if (action != Action::SKIP) {
            deltas.add_delta(Delta{offset, bytes, action});
        }
        offset += bytes;
    }

    std::vector<Signature> signatures_ = signatures.signatures();
    std::vector<Delta> deltas_ = deltas.deltas();
    for (int i = 0; i < signatures_.size(); i++) {
        if (!signatures_[i].searched() && !deltas.exist(signatures_[i].offset())) {
            deltas.add_delta(Delta{signatures_[i].offset(), bytes, Action::DELETE});
        }
    }

    return deltas;
}