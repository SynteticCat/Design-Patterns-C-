#include <iostream>
#include <memory>
using namespace std;

class Product;

// Фабрика для создания продукта
#pragma region Creator
class Creator 
{
public:
    virtual unique_ptr<Product> createProduct() = 0;
};

// к типу продукта мы не привязываемся в фабрике
template <typename Tprod>
class ConCreator : public Creator
{
public:
    virtual unique_ptr<Product> createProduct() override
    {
        return unique_ptr<Product>(new Tprod());
    }
};
#pragma endregion

// Класс продукта
#pragma region Product
class Product 
{
public:
    virtual ~Product() = 0;
    virtual void run() = 0;
};

Product::~Product() {}

class ConProduct1 : public Product
{
public:
    virtual ~ConProduct1() override { cout << "ConProduct1 destructor" << endl; }
    virtual void run() override { cout << "ConProduct1 method run" << endl; }
};

class ConProduct2 : public Product
{
public:
    virtual ~ConProduct2() override { cout << "ConProduct2 destructor" << endl; }
    virtual void run() override { cout << "ConProduct2 method run" << endl; }
};

#pragma endregion

int main()
{
    // ссылка на конкретную фабрику, создающую объекты конкретного типа
    shared_ptr<Creator> cr1(new ConCreator<ConProduct1>());
    shared_ptr<Creator> cr2(new ConCreator<ConProduct2>());

    // ссылка на продукт, который создается через фабрику и конкретный тип продукта
    shared_ptr<Product> ptr1 = cr1->createProduct();
    shared_ptr<Product> ptr2 = cr2->createProduct();

    ptr1->run();
    ptr2->run();
}