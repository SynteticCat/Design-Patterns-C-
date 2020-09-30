# include <iostream>
# include <memory>

using namespace std;

class BaseObject
{
public:
	virtual ~BaseObject() = default;

	virtual unique_ptr<BaseObject> clone() = 0;
};

class Object1 : public BaseObject
{
public:
	Object1() { cout << "Default constructor;" << endl; }
	Object1(const Object1& obj) { cout << "Copy constructor;" << endl; }
	~Object1() { cout << "Destructor;" << endl; }

	virtual unique_ptr<BaseObject> clone() override
	{
		return unique_ptr<BaseObject>(new Object1(*this));
	}
};

int main()
{
	unique_ptr<BaseObject> ptr1(new Object1());

	auto ptr2 = ptr1->clone();
}
