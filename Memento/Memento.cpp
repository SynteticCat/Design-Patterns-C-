# include <iostream>
# include <memory>
# include <list>

using namespace std;

class Memento;

class Caretaker
{
public:
	// получить снимок (идея стека)
	unique_ptr<Memento> getMemento();
	// добавить снимок (идея стека)
	void setMemento(unique_ptr<Memento> memento);

private:
	// список снимков
	list<unique_ptr<Memento>> mementos;
};

class Originator
{
public:
	Originator(int s) : state(s) {}

	const int getState() const { return state; }
	void setState(int s) { state = s; }

	// сделать снимок
	std::unique_ptr<Memento> createMemento() { return make_unique<Memento>(*this); }
	void restoreMemento(std::unique_ptr<Memento> memento);

private:
	int state;
};

class Memento
{
	friend class Originator;

public:
	Memento(Originator o) : originator(o) {}

private:
	// инициализирует
	void setOriginator(Originator o) { originator = o; }
	// выбирает
	Originator getOriginator() { return originator; }

private:
	// хранит объект, снимок объекта
	Originator originator;
};

#pragma region Methods Caretaker
void Caretaker::setMemento(unique_ptr<Memento> memento)
{
	// добавляем в хвост
	mementos.push_back(move(memento));
}

unique_ptr<Memento> Caretaker::getMemento() {

	unique_ptr<Memento> last = move(mementos.back());

	mementos.pop_back();

	// из хвоста берем
	return last;
}

#pragma endregion

#pragma region Method Originator
void Originator::restoreMemento(std::unique_ptr<Memento> memento)
{
	// заменяем на тот, что был до этого
	*this = memento->getOriginator();
}

#pragma endregion

int main()
{
	auto originator = make_unique<Originator>(1);
	auto caretaker = make_unique<Caretaker>();

	cout << "State = " << originator->getState() << endl;
	// добавляем снимок с объекта
	caretaker->setMemento(originator->createMemento());

	// меняем состояние
	originator->setState(2);
	cout << "State = " << originator->getState() << endl;
	// новый снимок добавляем
	caretaker->setMemento(originator->createMemento());
	originator->setState(3);
	cout << "State = " << originator->getState() << endl;
	caretaker->setMemento(originator->createMemento());

	// откат 1
	originator->restoreMemento(caretaker->getMemento());
	cout << "State = " << originator->getState() << endl;

	// откат 2
	originator->restoreMemento(caretaker->getMemento());
	cout << "State = " << originator->getState() << std::endl;

	// откат 3
	originator->restoreMemento(caretaker->getMemento());
	cout << "State = " << originator->getState() << std::endl;
}

