#include "Storage.h"

#include <iostream>

Storage::Storage(size_t size)
    : buffer(size), tempBuffer(size) {}

void Storage::addData(std::string& newData)
{
    /*Function for adding data in the Storage and start the synchronization thread.*/
    std::unique_lock<std::mutex> lock(mutex);

    if (syncThread == nullptr) {
        syncThread = std::unique_ptr<std::thread>(new std::thread(&Storage::synchronization, this));
    }

    tempBuffer.addData(newData);
}

void Storage::printData() 
{
    /*Function for printing the data in the buffer.*/
    for(const auto& item : buffer.getBuffer()){
        std::cout << item << ", ";
    }

    std::cout << std::endl;
}

std::vector<std::string> Storage::copyTempBuffer() 
{
    /*Function return the temporary boffuer with current data.*/
    std::unique_lock<std::mutex> lock(mutex);

    return tempBuffer.consumeBuffer();
}

void Storage::setMirrorObject(const std::shared_ptr<Storage>& storage)
{
    /*Function for synchronization the two objects.*/
    std::unique_lock<std::mutex> lock(mutex);
    mirrorObjPtr = storage;
}

void Storage::syncMirror() 
{
    /*Function for synchronization the temporary buffer with permanent buffer.*/
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

void Storage::synchronization()
{
    /*Function where the synchronization is set.*/

    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        syncMirror();
    }
}

Storage::~Storage() 
{
    syncThread->join();
}
