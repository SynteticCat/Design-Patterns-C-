# include <iostream>
# include <memory>

using namespace std;

class Adapter
{
public:
	virtual ~Adapter() = 0;

	virtual void request() = 0;
};

Adapter::~Adapter() = default;

class BaseAdaptee
{
public:
	virtual ~BaseAdaptee() = 0;

	virtual void specificRequest() = 0;
};

BaseAdaptee::~BaseAdaptee() = default;

class ConAdapter : public Adapter
{
private:
	shared_ptr<BaseAdaptee>  adaptee;

public:
	ConAdapter(shared_ptr<BaseAdaptee> ad) : adaptee(ad) {}

	virtual void request() override;
};

class ConAdaptee : public BaseAdaptee
{
public:
	virtual void specificRequest() override { cout << "Method ConAdaptee;" << endl; }
};

#pragma region Methods
void ConAdapter::request()
{
	cout << "Adapter: ";

	if (adaptee)
	{
		adaptee->specificRequest();
	}
	else
	{
		cout << "Empty!" << endl;
	}
}

#pragma endregion


int main()
{
	shared_ptr<BaseAdaptee> adaptee(new ConAdaptee());
	shared_ptr<Adapter> adapter(new ConAdapter(adaptee));

	adapter->request();
}
