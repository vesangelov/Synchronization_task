#include "Storage.h"

#include <iostream>

Storage::Storage(size_t size)
    : buffer(size), tempBuffer(size) {}

void Storage::addData(std::string& newData)
{
    std::unique_lock<std::mutex> lock(mutex);

    if (syncThread == nullptr) {
        syncThread = std::unique_ptr<std::thread>(new std::thread(&Storage::synchronization, this));
    }

    tempBuffer.addData(newData);
}

void Storage::printData() {

    for(const auto& item : buffer.getBuffer()){
        std::cout << item << ", ";
    }

    std::cout << std::endl;
}

std::vector<std::string> Storage::copyTempBuffer() {
    std::unique_lock<std::mutex> lock(mutex);

    return tempBuffer.consumeBuffer();
}

void Storage::setMirrorObject(const std::shared_ptr<Storage>& storage)
{
    std::unique_lock<std::mutex> lock(mutex);
    mirrorObjPtr = storage;
}

void Storage::syncMirror() {
    auto tempData = copyTempBuffer();

    for(const auto& item : tempData){
        buffer.addData(item);
    }

    if(mirrorObjPtr != nullptr){
        for(const auto& item : tempData){
            mirrorObjPtr->buffer.addData(item);
        }
    }
}

void Storage::synchronization(){

    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        syncMirror();
    }
}

Storage::~Storage() 
{
    syncThread->join();
}
