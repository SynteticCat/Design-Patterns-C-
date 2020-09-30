# include <iostream>
# include <memory>

using namespace std;

// любой компонент
class Component
{
public:
	virtual ~Component() = 0;

	virtual void operation() = 0;
};

Component::~Component() {}

class ConComponent : public Component
{
public:
	virtual void operation() override { cout << "ConComponent; "; }
};

class Decorator : public Component
{
protected:
	// содержит ссылку на компонент
	shared_ptr<Component> component;

public:
	Decorator(shared_ptr<Component> comp) : component(comp) {}
};

class ConDecorator : public Decorator
{
public:
	using Decorator::Decorator;

	// подменяет метод operation
	virtual void operation() override;
};

#pragma region Method
// сам метод содержит вызов operation и дополнительный функционал
void ConDecorator::operation()
{
	if (component)
	{
		component->operation();

		cout << "ConDecorator ";
	}

}

#pragma endregion

int main()
{
	shared_ptr<Component> component(new ConComponent());
	shared_ptr<Component> decorator1(new ConDecorator(component));

	decorator1->operation();
	cout << ";" << endl;

	shared_ptr<Component> decorator2(new ConDecorator(decorator1));

	decorator2->operation();
	cout << ";" << endl;
}
