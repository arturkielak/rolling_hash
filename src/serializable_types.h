#pragma once

#include <array>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>
#include <set>

#include "file.h"

const uint32_t BLOCK_SIZE = 3;
using StrongHashValue = std::array<uint32_t, 4>;
using WeakHashValue = uint64_t;

class Serializable {
public:
    virtual void serialize(File &file) = 0;

    virtual void deserialize(File &file) = 0;
};

class Header : public Serializable {
public:

    Header();

    Header(uint64_t block_size, uint64_t count);

    uint64_t count() const { return count_; }

    uint64_t block_size() const { return block_size_; }

    void count(const uint64_t &count);

    void block_size(const uint64_t &block_size);

    void deserialize(File &file) override;

    void serialize(File &file) override;

private:
    uint64_t block_size_;
    uint64_t count_;
};

class Signature : public Serializable {
public:
    Signature();

    Signature(WeakHashValue weak_hash, StrongHashValue strong_hash, uint64_t offset, bool searched = false);

    WeakHashValue weak_hash() const { return weak_hash_; };

    StrongHashValue strong_hash() const { return strong_hash_; };

    uint64_t offset() const { return offset_; };

    void searched(bool searched) { searched_ = searched; };

    bool searched() const { return searched_; };

    void deserialize(File &file) override;

    void serialize(File &file) override;

private:
    WeakHashValue weak_hash_;
    StrongHashValue strong_hash_;
    uint64_t offset_;
    bool searched_;
};

class Signatures : public Serializable {
public:
    Signatures();

    void add_signature(Signature signature);

    void deserialize(File &file) override;

    void serialize(File &file) override;

    Signature *find(uint64_t weak_hash);

    size_t count() const { return signatures_.size(); }

    std::vector<Signature> signatures() { return signatures_; };

private:
    std::vector<Signature> signatures_;
};

enum class Action {
    INSERT = 1, DELETE = 2, MOVE = 3, SKIP = 4
};

std::ostream &operator<<(std::ostream &os, const Action &object);

//TODO change Delta to keep chunk data
class Delta : public Serializable {
public:
    Delta();

    Delta(uint64_t offset, uint64_t length, Action action);

    uint64_t offset() const { return offset_; }

    uint64_t length() const { return length_; }

    Action action() const { return action_; }

    void deserialize(File &file) override;

    void serialize(File &file) override;

private:
    uint64_t offset_;
    uint64_t length_;
    Action action_;
};

class Deltas : public Serializable {
public:
    Deltas();

    void add_delta(Delta delta);

    void deserialize(File &file) override;

    void serialize(File &file) override;

    bool exist(uint64_t offset) {
        for(int i=0;i<deltas_.size();i++) {
            if(deltas_[i].offset() == offset)
                return true;
        }
        return false;
    }

    std::vector<Delta> deltas() { return deltas_; };

private:
    std::vector<Delta> deltas_;
};