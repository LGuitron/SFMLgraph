/*
Final Project: SFML graph
Name: Luis Enrique Güitrón A01018616
Name: Juan Pablo Güitrón A01019936
Data Structures
30/11/2016
a01019936@itesm.mx
a01018616@itesm.mx
*/

#ifndef SFMLNODE_HPP
#define SFMLNODE_HPP
#include <string>

class SFMLNode
{
    //Attributes
    private:
        float x;
        float y;
        float radius;
        std::string data;

    //methods
    public:
        SFMLNode(){}
        SFMLNode(float x, float y, float radius, std::string data);
        float getX(){return x;}
        float getY(){return y;}
        float getRadius(){return radius;}
        std::string getData(){return data;}
};
#endif

SFMLNode::SFMLNode(float x, float y, float radius,std::string data)
{
  this->x=x;
  this->y=y;
  this->radius=radius;
  this->data=data;
}
