#ifndef TASK_STORAGE_H
#define TASK_STORAGE_H

#include <string>
#include <vector>
#include <mutex>
#include <memory>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <cassert>

#include "Buffer.h"

class Storage {
public:
    explicit Storage(size_t size);
    void addData(std::string& newData);
    void printData();
    void setMirrorObject(const std::shared_ptr<Storage>& storage);
    ~Storage();

private:
    void synchronization();
    void syncMirror();
    std::vector<std::string> copyTempBuffer();
    //Check if I have mirror object
    Buffer tempBuffer;
    Buffer buffer;
    std::mutex mutex;
    std::shared_ptr<Storage> mirrorObjPtr;
    std::unique_ptr<std::thread> syncThread;
};


#endif //TASK_STORAGE_H

/*syncMirror()
tempBuffer saves the data and sync it with the other class buffer.
Where I will have a mutex?
If few users are trying to add data, tempBuffer will add all and after that will make the synchronization.*/