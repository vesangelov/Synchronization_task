#include "Buffer.h"

Buffer::Buffer(size_t bufferSize)
        : data(bufferSize), begin(0), size(0) {}

void Buffer::addData(const std::string& newData){

    if (size == data.size())
    {
        data[begin] = newData;

        if (++begin >= size)
        {
            begin = 0;
        }
    }
    else
    {
        data[size] = newData;

        if (size < data.size())
        {
            ++size;
        }
    }
}

std::vector<std::string> Buffer::getBuffer() const{
    std::vector<std::string> result(size);

    if (begin == 0)
    {
        for (size_t i = 0; i < size; i++)
        {
            result[i] = data[i];
        }
    }
    else
    {
        for (size_t i = begin; i < data.size(); i++)
        {
            result[i - begin] = data[i];
        }

        for (size_t i = 0; i < begin; i++)
        {
            result[i + size] = data[i];
        }
    }

    return result;
}

std::vector<std::string> Buffer::consumeBuffer(){
    std::vector<std::string> result = getBuffer();
    begin = 0;
    size = 0;

    return result;
}
