#ifndef SYNCHRONIZATION_TASK_BUFFER_H
#define SYNCHRONIZATION_TASK_BUFFER_H

#include <string>
#include <vector>

class Buffer {
public:
    explicit Buffer(size_t bufferSize);
    void addData(const std::string& newData);
    std::vector<std::string> getBuffer() const;
    std::vector<std::string> consumeBuffer();
private:
    std::vector<std::string> data;
    size_t begin;
    size_t size;
};


#endif //SYNCHRONIZATION_TASK_BUFFER_H
