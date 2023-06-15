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

constexpr int BUFFERSIZE = 3;

class Storage {
public:
    explicit Storage();
    void addData(std::string& newData);
    void printData();
    void syncData(const std::string& newData);
    void setMirrorObject(const std::shared_ptr<Storage>& storage);
    ~Storage();

private:
    void syncMirror();
    //Check if I have mirror object

    void appendData(std::string& newData);
    std::vector<std::string> buffer;
    size_t begin;
    size_t size;
    std::mutex mutex;
    bool canSync;
    std::shared_ptr<Storage> storagePtr;
    std::unique_ptr<std::thread> syncThread;
    std::vector<std::string> tempBuffer;
};


#endif //TASK_STORAGE_H

/*syncMirror()
tempBuffer saves the data and sync it with the other class buffer.
Where I will have a mutex?
If few users are traying to add data, tempBuffer will add all and afther that will make the synchronization.*/