#pragma once

#include <string>
#include <fstream>

class File {
public:
    File &operator=(File &&file) {
        file_ = std::move(file.file_);
        path_ = file.path_;
        return *this;
    }

    File(File &&file) : file_(std::move(file.file_)), path_(std::move(file.path_)) {

    }

    File(const std::string &path, std::ios_base::openmode mode) : path_(path) {
        file_.open(path_, mode);
        if (!file_.is_open())
            throw std::runtime_error("Can't open file " + path);
    }

    template<typename T>
    void write(T t, size_t length) {
        file_.write(t, length);
        file_.flush();
    }

    template<typename T>
    unsigned long read(T t, size_t block_size) {
        return file_.read(t, block_size).gcount();
    }

    void seek(std::ios_base::seekdir position) {
        if (!file_.is_open())
            throw std::runtime_error("File is not open!");
        file_.seekg(0, position);
    }

    void close() {
        file_.flush();
        file_.close();
    }

    ~File() {
        close();
    }

private:
    std::fstream file_;
    std::string path_;
};