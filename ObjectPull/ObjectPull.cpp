# include <iostream>
# include <memory>
# include <iterator>
# include <vector>

using namespace std;

class Product
{
private:
	static size_t count;
public:
	Product() { cout << "Constructor(" << ++count << ");" << endl; }
	~Product() { cout << "Destructor(" << count-- << ");" << endl; }

	void clear() { cout << "Method clear: 0x" << this << endl; }
};

size_t Product::count = 0;

// реализован на базе одиночки
template <typename Type>
class ObjectPool
{
public:
	static shared_ptr<ObjectPool<Type>> instance();

	shared_ptr<Type> getObject();
	bool releaseObject(shared_ptr<Type>& obj);
	size_t count() const { return pool.size(); }

	// удобно использовать итератор
	iterator<output_iterator_tag, const pair<bool, shared_ptr<Type>>> begin() const;
	iterator<output_iterator_tag, const pair<bool, shared_ptr<Type>>> end() const;

	ObjectPool(const ObjectPool<Type>&) = delete;
	ObjectPool<Type>& operator=(const ObjectPool<Type>&) = delete;

private:
	vector<pair<bool, shared_ptr<Type>>> pool;

	ObjectPool() {}

	pair<bool, shared_ptr<Type>> create();

	template <typename Type>
	friend ostream& operator << (ostream& os, const ObjectPool<Type>& pl);
};

#pragma region ObjectPool class Methods 
template <typename Type>
shared_ptr<ObjectPool<Type>> ObjectPool<Type>::instance()
{
	static shared_ptr<ObjectPool<Type>> myInstance(new ObjectPool<Type>());

	return myInstance;
}

template <typename Type>
shared_ptr<Type> ObjectPool<Type>::getObject()
{
	size_t i;
	for (i = 0; i < pool.size() && pool[i].first; ++i);

	if (i < pool.size())
	{
		pool[i].first = true;
	}
	else
	{
		pool.push_back(create());
	}

	return pool[i].second;
}

template <typename Type>
bool ObjectPool<Type>::releaseObject(shared_ptr<Type>& obj)
{
	size_t i;
	for (i = 0; i < pool.size() && pool[i].second != obj; ++i);

	if (i == pool.size()) return false;

	obj.reset();
	pool[i].first = false;
	pool[i].second->clear();

	return true;
}

template <typename Type>
pair<bool, shared_ptr<Type>> ObjectPool<Type>::create()
{
	return pair<bool, shared_ptr<Type>>(true, shared_ptr<Type>(new Type()));
}

#pragma endregion

template <typename Type>
ostream& operator << (ostream& os, const ObjectPool<Type>& pl)
{
	for (auto elem : pl.pool)
		os << "{" << elem.first << ", 0x" << elem.second << "} ";

	return os;
}

int main()
{
	shared_ptr<ObjectPool<Product>> pool = ObjectPool<Product>::instance();

	vector<shared_ptr<Product>> vec(4);

	for (auto& elem : vec)
		elem = pool->getObject();

	pool->releaseObject(vec[1]);

	cout << *pool << endl;

	shared_ptr<Product> ptr = pool->getObject();
	vec[1] = pool->getObject();

	cout << *pool << endl;
}
