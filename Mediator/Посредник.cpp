# include <iostream>
# include <memory>
# include <list>
# include <vector>

using namespace std;

class Message {};         // Request

class Mediator;

class Colleague
{
private:
	// объект содержит указатель на медиатор
	weak_ptr<Mediator> mediator;

public:
	virtual ~Colleague() = default;

	// есть метод установки медиатора
	void setMediator(shared_ptr<Mediator> mdr) { mediator = mdr; }

	// 2 метода для общения с медиатором
	virtual bool send(shared_ptr<Message> msg);
	virtual void receive(shared_ptr<Message> msg) = 0;
};

class ColleagueLeft : public Colleague
{
public:
	virtual void receive(shared_ptr<Message> msg) override { cout << "Right - > Left;" << endl; }
};

class ColleagueRight : public Colleague
{
public:
	virtual void receive(shared_ptr<Message> msg) override { cout << "Left - > Right;" << endl; }
};

class Mediator
{
protected:
	list<shared_ptr<Colleague>> colleagues;

public:
	virtual ~Mediator() = default;

	// послать сообщение (кому передать, что передать)
	virtual bool send(const Colleague* coleague, shared_ptr<Message> msg) = 0;

	// добавить медиатор
	static bool add(shared_ptr<Mediator> mediator, shared_ptr<Colleague> colleague, ...);
};

class ConMediator : public Mediator
{
public:
	virtual bool send(const Colleague* coleague, shared_ptr<Message> msg) override;
};

#pragma region Methods Colleague
bool Colleague::send(shared_ptr<Message> msg)
{
	shared_ptr<Mediator> mdr = mediator.lock();

	// если есть медиатор, то посылаем сообщение
	return mdr ? mdr->send(this, msg) : false;
}

#pragma endregion

#pragma region Methods Mediator
bool Mediator::add(shared_ptr<Mediator> mediator, shared_ptr<Colleague> colleague, ...)
{
	if (!mediator || !colleague) return false;

	for (shared_ptr<Colleague>* ptr = &colleague; *ptr; ++ptr)
	{
		mediator->colleagues.push_back(*ptr);
		(*ptr)->setMediator(mediator);
	}

	return true;
}

bool ConMediator::send(const Colleague* colleague, shared_ptr<Message> msg)
{
	bool flag = false;
	for (auto& elem : colleagues)
	{
		// если полученный элемент левый, а элемент из списка правый - кидаю правому сообщение
		if (dynamic_cast<const ColleagueLeft*>(colleague) && dynamic_cast<ColleagueRight*>(elem.get()))
		{
			elem->receive(msg);
			flag = true;
		}
		// если полученный элемент правый, а элемент из списка левый - кидаю левому сообщение
		else if (dynamic_cast<const ColleagueRight*>(colleague) && dynamic_cast<ColleagueLeft*>(elem.get()))
		{
			elem->receive(msg);
			flag = true;
		}
	}

	return flag;
}

#pragma endregion

int main()
{
	shared_ptr<Mediator> mediator(new ConMediator());

	shared_ptr<Colleague> col1(new ColleagueLeft());
	shared_ptr<Colleague> col2(new ColleagueRight());
	shared_ptr<Colleague> col3(new ColleagueLeft());
	shared_ptr<Colleague> col4(new ColleagueLeft());

	Mediator::add(mediator, col1, col2, col3, col4, shared_ptr<Colleague>());

	shared_ptr<Message> msg(new Message());

	col1->send(msg);
	col2->send(msg);
}
