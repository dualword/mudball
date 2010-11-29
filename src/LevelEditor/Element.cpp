#include "Element.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

Element::~Element()
{
    node->remove();
}

void Element::animate()
{
}

ElementType Element::getType()
{
    return type;
}

void Element::setPosition(vector3df pos)
{
    node->setPosition(pos);
}

vector3df Element::getPosition()
{
    return node->getPosition();
}

void Element::setRotation(vector3df pos)
{
    node->setRotation(pos);
}

vector3df Element::getRotation()
{
    return node->getRotation();
}

void Element::setParent(ISceneNode* parent)
{
    node->setParent(parent);
}
