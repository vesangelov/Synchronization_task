#include "Storage.h"

#include <iostream>

Storage::Storage()
    : buffer(BUFFERSIZE), begin(0), size(0), canSync(false), tempBuffer(BUFFERSIZE) {}

void Storage::addData(std::string& newData)
{
    if (syncThread == nullptr) {
        syncThread = std::unique_ptr<std::thread>();
    }

    if (size == tempBuffer.size())
    {
        tempBuffer[begin] = newData;

        if (++begin >= size)
        {
            begin = 0;
        }
    }
    else
    {
        tempBuffer[size] = newData;

        if (size < tempBuffer.size())
        {
            ++size;
        }
    }

    syncMirror();
}

void Storage::printData() {

    std::unique_lock<std::mutex> lock(mutex);

    if (begin == 0)
    {
        for (size_t i = 0; i < size; i++)
        {
            std::cout << buffer[i] << ", ";
        }
    }
    else
    {
        for (size_t i = begin; i < buffer.size(); i++)
        {
            std::cout << buffer[i] << ", ";
        }

        for (size_t i = 0; i < begin; i++)
        {
            std::cout << buffer[i] << ", ";
        }
    }
}

void Storage::syncData(const std::string& newData)
{
    std::unique_lock<std::mutex> lock(mutex);

    if (size == buffer.size())
    {
        buffer[begin] = newData;

        if (++begin >= size)
        {
            begin = 0;
        }
    }
    else
    {
        buffer[size] = newData;

        if (size < buffer.size())
        {
            ++size;
        }
    }
}

void Storage::setMirrorObject(const std::shared_ptr<Storage>& storage)
{
    std::unique_lock<std::mutex> lock(mutex);
    storagePtr = std::make_shared<Storage>();
    storagePtr->buffer = storage->buffer;
    storagePtr->begin = storage->begin;
    storagePtr->size = storage->size;
}

void Storage::syncMirror() {

    std::unique_lock<std::mutex> lock(mutex);

    if (storagePtr == nullptr) {
        tempBuffer.clear();
    }
    else {
        for (const auto& data : tempBuffer)
        {
            storagePtr->syncData(data);
        }
    }
}

// This should always be called wild hoding a lock to the mutex
void Storage::appendData(std::string& newData) {

}

Storage::~Storage() 
{
    syncThread->join();
}
