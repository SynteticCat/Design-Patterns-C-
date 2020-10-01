# include <iostream>
# include <memory>
# include <map>
# include <random>

using namespace std;

class Subject
{
public:
	virtual ~Subject() = 0;

	// возвращает пару (есть объект или нет + значение по индексу)
	virtual pair<bool, double> request(size_t index) = 0;
	virtual bool changed() { return true; }
};

Subject::~Subject() = default;

// реальный объект вернет ту же пару с bool === true
class RealSubject : public Subject
{
private:
	bool flag{ false };
	size_t counter{ 0 };

public:
	virtual pair<bool, double> request(size_t index) override;
	virtual bool changed() override;
};

class Proxy : public Subject
{
protected:
	shared_ptr<RealSubject> realsubject;

public:
	Proxy(shared_ptr<RealSubject> real) : realsubject(real) {}
};

class ConProxy : public Proxy
{
private:
	// хранит пары (по индексу и числу)
	map<size_t, double> cache;

public:
	using Proxy::Proxy;

	virtual pair<bool, double> request(size_t index) override;
};

#pragma region Methods
bool RealSubject::changed()
{
	if (counter == 0)
	{
		flag = true;
	}
	if (++counter == 7)
	{
		counter = 0;
		flag = false;
	}
	return flag;
}

pair<bool, double> RealSubject::request(size_t index)
{
	random_device rd;
	mt19937 gen(rd());

	return pair<bool, double>(true, generate_canonical<double, 10>(gen));
}

// по индексу ищет среди пар данное число и возвращает его
pair<bool, double> ConProxy::request(size_t index)
{
	pair<bool, double> result;

	// если отсутствует объект, который замещается
	if (!realsubject)
	{
		cache.clear();

		result = pair<bool, double>(false, 0.);
	}
	// отсутсвует, но изменения были, очищается история и происходит запрос, запись в карту
	if (!realsubject->changed())
	{
		cache.clear();

		result = realsubject->request(index);

		cache.insert(map<size_t, double>::value_type(index, result.second));
	}
	// иначе, ищем в карте значение с таким индексом, если нашли, формируем результат
	// или обащаемся к объекту и добавляем в историю
	else
	{
		map<size_t, double>::const_iterator it = cache.find(index);

		if (it != cache.end())
		{
			result = pair<bool, double>(true, it->second);
		}
		else
		{
			result = realsubject->request(index);

			cache.insert(map<size_t, double>::value_type(index, result.second));
		}
	}

	return result;
}

#pragma endregion

int main()
{
	shared_ptr<RealSubject> subject(new RealSubject());
	shared_ptr<Subject> proxy(new ConProxy(subject));

	for (size_t i = 0; i < 21; ++i)
	{
		cout << "( " << i + 1 << ", " << proxy->request(i % 3).second << " )" << endl;

		if ((i + 1) % 3 == 0)
			cout << endl;
	}

}
