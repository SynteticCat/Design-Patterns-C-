# include <iostream>
# include <memory>
# include <vector>

using namespace std;

class Subscriber;

using Reseiver = Subscriber;

class Publisher
{
private:
	using Action = void(Reseiver::*)();
	// пара содержит указатель на подписчика и какой метод подписчика мы вызываем
	using Pair = pair<shared_ptr<Reseiver>, Action>;

	// издатель содержить вектор пар
	vector<Pair> callback;

public:
	void subscribe(shared_ptr<Reseiver> r, Action a);

	void run();
};

class Subscriber
{
public:
	virtual ~Subscriber() = default;

	virtual void method() = 0;
};

class ConSubscriber : public Subscriber
{
public:
	virtual void method() override { cout << "method;" << endl; }
};

#pragma region Methods Publisher
void Publisher::subscribe(shared_ptr<Reseiver> r, Action a)
{
	// создается пара
	Pair pr(r, a);

	// добавляется в вектор
	callback.push_back(pr);
}

void Publisher::run()
{
	cout << "Run:" << endl;
	// проходимся по парам и вызываем методы
	for (auto elem : callback)
		((*elem.first).*(elem.second))();
}

#pragma endregion

int main()
{
	shared_ptr<Subscriber> subscriber(new ConSubscriber());
	shared_ptr<Publisher> publisher(new Publisher());

	// подписчик подпсывается на издателя
	publisher->subscribe(subscriber, &Subscriber::method);

	publisher->run();
}
