#include <iostream>
#include <memory>
using namespace std;

class Image {};
class Color {};

class BaseGraphics
{
public:	virtual ~BaseGraphics() = 0;
};
BaseGraphics::~BaseGraphics() {}

class BasePen {};
class BaseBrush {};

class QtGraphics : public BaseGraphics
{
public:
	QtGraphics(shared_ptr<Image> im) { cout << "Constructor QtGraphics;" << endl; }
	virtual ~QtGraphics() override { cout << "Destructor QtGraphics;" << endl; }
};

class QtPen : public BasePen {};
class QtBrush : public BaseBrush {};

class AbstractGraphFactory
{
public:
	virtual unique_ptr<BaseGraphics> createGraphics(shared_ptr<Image> im) = 0;
	virtual unique_ptr<BasePen> createPen(shared_ptr<Color> cl) = 0;
	virtual unique_ptr<BaseBrush> createBrush(shared_ptr<Color> cl) = 0;
};

class QtGraphFactory : public AbstractGraphFactory
{
	virtual unique_ptr<BaseGraphics> createGraphics(shared_ptr<Image> im)
	{
		return unique_ptr<BaseGraphics>(new QtGraphics(im));
	}

	virtual unique_ptr<BasePen> createPen(shared_ptr<Color> cl)
	{
		return unique_ptr<BasePen>(new QtPen());
	}

	virtual unique_ptr<BaseBrush> createBrush(shared_ptr<Color> cl)
	{
		return unique_ptr<BaseBrush>(new QtBrush());
	}
};

int main()
{
	shared_ptr<AbstractGraphFactory> grfactory(new QtGraphFactory());

	shared_ptr<BaseGraphics> graphics1 = grfactory->createGraphics(shared_ptr<Image>(new Image()));
	shared_ptr<BaseGraphics> graphics2 = grfactory->createGraphics(shared_ptr<Image>(new Image()));
}
