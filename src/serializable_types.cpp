#include "serializable_types.h"

Header::Header() : block_size_(0), count_(0) {}

Header::Header(uint64_t block_size, uint64_t count) : block_size_(block_size), count_(count) {
}

void Header::count(const uint64_t &count) { count_ = count; }

void Header::block_size(const uint64_t &block_size) { block_size_ = block_size; }

void Header::deserialize(File &file) {
    size_t size = sizeof(*this);
    auto buffer = std::make_unique<uint8_t[]>(size);
    file.read((char *) buffer.get(), size);

    Header *header = (Header *) buffer.get();
    this->block_size_ = header->block_size_;
    this->count_ = header->count_;
}

void Header::serialize(File &file) {
    size_t size = sizeof(*this);
    auto buffer = std::make_unique<uint8_t[]>(size);
    memset(buffer.get(), 0x0, size);
    Header *header = (Header *) buffer.get();
    header->block_size_ = this->block_size_;
    header->count_ = this->count_;

    file.write((char *) buffer.get(), size);
}

Signature::Signature() {}

Signature::Signature(WeakHashValue weak_hash, StrongHashValue strong_hash, uint64_t offset, bool searched) : weak_hash_(weak_hash),
                                                                                      strong_hash_(strong_hash),
                                                                                      offset_(offset),
                                                                                      searched_(searched) {
}

void Signature::deserialize(File &file) {
    //not serialize searched_ value
    size_t size = sizeof(*this) - sizeof(bool);
    auto buffer = std::make_unique<uint8_t[]>(size);
    file.read((char *) buffer.get(), size);

    Signature *signature = (Signature *) buffer.get();
    this->weak_hash_ = signature->weak_hash_;
    this->offset_ = signature->offset_;
    this->strong_hash_ = signature->strong_hash_;
}

void Signature::serialize(File &file) {
    //not serialize searched_ value
    size_t size = sizeof(*this) - sizeof(bool);
    auto buffer = std::make_unique<uint8_t[]>(size);
    memset(buffer.get(), 0x0, size);
    Signature *signature = (Signature *) buffer.get();
    signature->weak_hash_ = this->weak_hash_;
    signature->strong_hash_ = this->strong_hash_;
    signature->offset_ = this->offset_;

    file.write((char *) buffer.get(), size);
}

Signatures::Signatures() {}

void Signatures::add_signature(Signature signature) {
    signatures_.emplace_back(signature);
}

void Signatures::deserialize(File &file) {
    signatures_.clear();

    Header header;
    header.deserialize(file);

    for (size_t i = 0; i < header.count(); i++) {
        Signature signature;
        signature.deserialize(file);
        signatures_.emplace_back(std::move(signature));
    }
}

void Signatures::serialize(File &file) {
    Header(BLOCK_SIZE, signatures_.size()).serialize(file);

    for (auto it = signatures_.begin(); it != signatures_.end(); it++) {
        it->serialize(file);
    }
}

Signature *Signatures::find(uint64_t weak_hash) {
    auto it = find_if(std::begin(signatures_), std::end(signatures_),
                      [&weak_hash](const Signature &signature) { return signature.weak_hash() == weak_hash; });
    if (it != signatures_.end()) {
        auto index = std::distance(signatures_.begin(), it);
        signatures_[index].searched(true);
        return &signatures_[index];
    }
    return nullptr;
}

Delta::Delta() {}

Delta::Delta(uint64_t offset, uint64_t length, Action action) : offset_(offset),
                                                                 length_(length),
                                                                 action_(action) {
}

void Delta::deserialize(File &file) {
    size_t size = sizeof(*this);
    auto buffer = std::make_unique<uint8_t[]>(size);
    file.read((char *) buffer.get(), size);

    Delta *delta = (Delta *) buffer.get();
    this->length_ = delta->length_;
    this->offset_ = delta->offset_;
    this->action_ = delta->action_;
}

void Delta::serialize(File &file) {
    size_t size = sizeof(*this);
    auto buffer = std::make_unique<uint8_t[]>(size);
    memset(buffer.get(), 0x0, size);
    Delta *delta = (Delta *) buffer.get();
    delta->action_ = this->action_;
    delta->length_ = this->length_;
    delta->offset_ = this->offset_;

    file.write((char *) buffer.get(), size);
}

std::ostream& operator << (std::ostream& os, const Action& object) {
    switch(object) {
        case Action::MOVE:
            os << "MOVE";
            return os;
        case Action::INSERT:
            os << "INSERT";
            return os;
        case Action::DELETE:
            os << "DELETE";
            return os;
        case Action::SKIP:
            os << "SKIP";
            return os;
    }
    return os;
}

Deltas::Deltas() {}

void Deltas::add_delta(Delta delta) {
    deltas_.emplace_back(delta);
}

void Deltas::deserialize(File &file) {
    deltas_.clear();

    Header header;
    header.deserialize(file);

    for (size_t i = 0; i < header.count(); i++) {
        Delta delta;
        delta.deserialize(file);
        deltas_.emplace_back(std::move(delta));
    }
}

void Deltas::serialize(File &file) {
    Header(BLOCK_SIZE, deltas_.size()).serialize(file);
    for (auto it = deltas_.begin(); it != deltas_.end(); it++) {
        it->serialize(file);
    }
}