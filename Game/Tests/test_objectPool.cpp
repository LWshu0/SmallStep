#include "Core/ObjectPool.h"
#include <iostream>
#include <cassert>

class example_Dog {
public:
    example_Dog() {}
    void bark()
    {
        std::cout << "bark" << std::endl;
    }
    ~example_Dog() {}
};

class DogFactory :public ObjectFactory<example_Dog> {
public:
    virtual std::shared_ptr<example_Dog> create()
    {
        return std::make_shared<example_Dog>();
    }
};

int main()
{
    std::shared_ptr<DogFactory> dog_factory = std::make_shared<DogFactory>();
    ObjectPool<example_Dog> pool(dog_factory);
    // SafeObjectPool<example_Dog> pool(dog_factory);

    auto dog1 = pool.acquire();
    auto dog2 = pool.acquire();

    pool.release(dog1);
    assert(1 == pool.size());
    assert(nullptr == dog1);

    dog1 = pool.acquire();
    assert(0 == pool.size());

    pool.release(dog1);
    pool.release(dog2);
    assert(2 == pool.size());

    std::cout << "corret" << std::endl;
}