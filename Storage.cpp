#include "Storage.h"

#include <iostream>

Storage::Storage(size_t size)
    : buffer(size), tempBuffer(size) {}

void Storage::addData(std::string& newData)
{
    /*Function for adding data in the Storage and start the synchronization thread.*/
    std::unique_lock<std::mutex> lock(tempBufferMutex);

    if (syncThread == nullptr) {
        syncThread = std::unique_ptr<std::thread>(new std::thread(&Storage::synchronization, this));
    }

    tempBuffer.addData(newData);
}

void Storage::printData() 
{
    /*Function for printing the data in the buffer.*/
    std::unique_lock<std::mutex> lock(bufferMutex);
    for(const auto& item : buffer.getBuffer()){
        std::cout << item << ", ";
    }

    std::cout << std::endl;
}

std::vector<std::string> Storage::copyTempBuffer()
{
    /*Function return the temporary boffuer with current data.*/
    std::unique_lock<std::mutex> lock(tempBufferMutex);

    return tempBuffer.consumeBuffer();
}

void Storage::setMirrorObject(const std::shared_ptr<Storage>& storage)
{
    /*Function for synchronization the two objects.*/
    std::unique_lock<std::mutex> lock(bufferMutex);
    mirrorObjPtr = storage;
}

void Storage::syncMirror() 
{
    /*Function for receiving synchronization on sync thread and copy the data in the other object.*/
    auto tempData = copyTempBuffer();

    {
        std::unique_lock<std::mutex> lock(bufferMutex);
        for(const auto& item : tempData){
            buffer.addData(item);
        }
    }

    if(mirrorObjPtr != nullptr){
        std::unique_lock<std::mutex> lock(mirrorObjPtr->bufferMutex);
        for(const auto& item : tempData){
            mirrorObjPtr->buffer.addData(item);
        }
    }
}

void Storage::synchronization()
{
    /*Function synchronization thread body.*/

    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        syncMirror();
    }
}

Storage::~Storage() 
{
    syncThread->join();
}
