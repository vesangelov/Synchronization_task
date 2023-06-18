#include <iostream>

#include "Storage.h"

constexpr int BUFFERSIZE = 3;
constexpr int MAXDATASIZE = 10;

void addDataToCurrentClass(std::shared_ptr<Storage>& storage) {

    std::cout << std::endl;
    std::cout << "Option menu:" << std::endl;
    std::cout << "1. Write print for printing the data in the buffer." << std::endl;
    std::cout << "2 Enter data to be put in the buffer(data must be less than 10 symbols)." << std::endl;

    std::string data;
    std::cin >> data;

    if (data == "print") {
        storage->printData();
        std::cout << std::endl;
    }
    else {

        if (data.size() > MAXDATASIZE) {
            std::cout << "Data is out of bounds." << std::endl;
        }
        else {
            storage->addData(data);
        }
    }

    std::cout << std::endl;
}

int main()
{
    std::shared_ptr<Storage> storageOne = std::make_shared<Storage>(BUFFERSIZE);
    std::shared_ptr<Storage> storageTwo = std::make_shared<Storage>(BUFFERSIZE);

    storageTwo->setMirrorObject(storageOne);
    storageOne->setMirrorObject(storageTwo);

    int currentClass = 3;

    while (true)
    {
        std::cout << "Select storage to put date(1 storage 1, or 2 for storage 2): " << std::endl;

        std::cin >> currentClass;

        if (currentClass == 1)
        {
            addDataToCurrentClass(storageOne);
        }
        else if (currentClass == 2)
        {
            addDataToCurrentClass(storageTwo);
        }
        else if (currentClass == 0)
        {
            std::cout << "End" << std::endl;
            break;
        }
        else
        {
            std::cout << "Undefined class number." << std::endl;
        }
    }
}