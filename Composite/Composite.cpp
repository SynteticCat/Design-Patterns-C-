# include <iostream>
# include <memory>
# include <vector>
# include <iterator>

using namespace std;

class Component;

using VectorComponent = vector<shared_ptr<Component>>;
using IteratorComponent = VectorComponent::const_iterator;

class Component
{
public:
	virtual ~Component() = 0;

	virtual void operation() = 0;

	virtual bool isComposite() const { return false; }
	virtual bool add(shared_ptr<Component> comp) { return false; }
	virtual bool remove(const IteratorComponent& it) { return false; }
	virtual IteratorComponent begin() const { return IteratorComponent(); }
	virtual IteratorComponent end() const { return IteratorComponent(); }
};

Component::~Component() {}

class Figure : public Component
{
public:
	virtual void operation() override { cout << "Figure method;" << endl; }
};

class Camera : public Component
{
public:
	virtual void operation() override { cout << "Camera method;" << endl; }
};

class Composite : public Component
{
private:
	VectorComponent vec;

public:
	Composite() = default;
	Composite(shared_ptr<Component> first, ...);

	virtual void operation() override;

	virtual bool isComposite() const override { return true; }
	virtual bool add(shared_ptr<Component> comp) { vec.push_back(comp); return false; }
	virtual bool remove(const IteratorComponent& it) { vec.erase(it); return false; }
	virtual IteratorComponent begin() const override { return vec.begin(); }
	virtual IteratorComponent end() const override { return vec.end(); }
};

// заполняем композит компонентами
#pragma region Methods
Composite::Composite(shared_ptr<Component> first, ...)
{
	for (shared_ptr<Component>* ptr = &first; *ptr; ++ptr)
		vec.push_back(*ptr);
}

// для каждого компонента вызывается метод operation (операция применяется к иерархии)
void Composite::operation()
{
	cout << "Composite method:" << endl;
	for (auto elem : vec)
		elem->operation();
}

#pragma endregion

int main()
{
	using Default = shared_ptr<Component>;
	shared_ptr<Component> fig1(new Figure()), fig2(new Figure), cam1(new Camera()), cam2(new Camera());
	shared_ptr<Component> composite1(new Composite(fig1, cam1, fig2, cam2, Default()));

	composite1->operation();
	cout << endl;

	IteratorComponent it = composite1->begin();

	composite1->remove(++it);
	composite1->operation();
	cout << endl;

	// композит является частью своей структуры
	shared_ptr<Component> composite2(new Composite(shared_ptr<Component>(new Figure()), composite1, Default()));

	composite2->operation();
}
