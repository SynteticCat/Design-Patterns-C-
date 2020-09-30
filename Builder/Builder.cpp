# include <iostream>
# include <memory>

using namespace std;

class Product
{
public:
	Product() { cout << "Default constructor;" << endl; }
	~Product() { cout << "Destructor;" << endl; }

	void run() { cout << "Method run;" << endl; }
};

class Builder
{
public:
	virtual bool buildPart1() = 0;
	virtual bool buildPart2() = 0;

	shared_ptr<Product> getProduct();

protected:
	virtual shared_ptr<Product> createProduct() = 0;

	shared_ptr<Product> product;
};

class ConBuilder : public Builder
{
public:
	virtual bool buildPart1() override { cout << "Completed part: " << ++part << ";" << endl; return true; }
	virtual bool buildPart2() override { cout << "Completed part: " << ++part << ";" << endl; return true; }

protected:
	virtual shared_ptr<Product> createProduct() override;

private:
	size_t part{ 0 };
};

class Director
{
public:
	shared_ptr<Product> create(shared_ptr<Builder> builder)
	{
		if (builder->buildPart1() && builder->buildPart2()) return builder->getProduct();

		return shared_ptr<Product>();
	}
};

#pragma region Methods
shared_ptr<Product> Builder::getProduct()
{
	if (!product) { product = createProduct(); }

	return product;
}

shared_ptr<Product> ConBuilder::createProduct()
{
	if (part == 2) { product = shared_ptr<Product>(new Product()); }

	return product;
}

#pragma endregion

int main()
{
	shared_ptr<Builder> builder(new ConBuilder());
	shared_ptr<Director> director(new Director());

	shared_ptr<Product> prod = director->create(builder);

	if (prod)
		prod->run();
}
