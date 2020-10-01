# include <iostream>
# include <memory>

using namespace std;

class AbstractHandler
{
protected:
	shared_ptr<AbstractHandler> next;

	virtual bool run() = 0;

public:
	using Default = shared_ptr<AbstractHandler>;

	virtual ~AbstractHandler() = default;

	virtual bool handle() = 0;

	// базовый класс может добавлять в список обработчики
	void add(shared_ptr<AbstractHandler> node);
	void add(shared_ptr<AbstractHandler> node1, shared_ptr<AbstractHandler> node2, ...);
};

class ConHandler : public AbstractHandler
{
private:
	bool condition{ false };

protected:
	virtual bool run() override { cout << "Method run;" << endl; return true; }

public:
	ConHandler() = default;
	ConHandler(bool c) : condition(c) { cout << "Constructor;" << endl; }
	virtual ~ConHandler() override { cout << "Destructor;" << endl; }

	// конкретный обработчик содержит метод обработки
	// То есть идем до тех пор, пока не обработаем запрос
	virtual bool handle() override
	{
		// состояние false и следующий элемент цепочки задан, обработать запрос 
		if (!condition) return next ? next->handle() : false;

		// если состояние true, обработаем запрос
		return run();
	}

};

#pragma region Methods
void AbstractHandler::add(shared_ptr<AbstractHandler> node)
{
	if (next)
		next->add(node);
	else
		next = node;
}

void AbstractHandler::add(shared_ptr<AbstractHandler> node1, shared_ptr<AbstractHandler> node2, ...)
{
	add(node1);
	for (Default* ptr = &node2; *ptr; ++ptr)
		add(*ptr);
}

#pragma endregion

int main()
{
	shared_ptr<AbstractHandler> chain(new ConHandler());

	// формируется цепочка
	chain->add(
		shared_ptr<AbstractHandler>(new ConHandler(false)),
		shared_ptr<AbstractHandler>(new ConHandler(true)),
		shared_ptr<AbstractHandler>(new ConHandler(true)),
		AbstractHandler::Default()
	);

	// делаем запрос на обработку
	cout << "Result = " << chain->handle() << ";" << endl;;
}

