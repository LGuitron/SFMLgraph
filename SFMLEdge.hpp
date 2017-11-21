/*
Final Project: SFML graph
Name: Luis Enrique Güitrón A01018616
Name: Juan Pablo Güitrón A01019936
Data Structures
30/11/2016
a01019936@itesm.mx
a01018616@itesm.mx
*/

#ifndef SFMLEDGE_HPP
#define SFMLEDGE_HPP

class SFMLEdge
{
    //Attributes
    private:
        float x0;
        float y0;
        float x1;
        float y1;
        std::string weight;

    //methods
    public:
        SFMLEdge(){}
        SFMLEdge(float x0, float y0, float x1, float y1,std::string weight);
        float getX0(){return x0;}
        float getY0(){return y0;}
        float getX1(){return x1;}
        float getY1(){return y1;}
        std::string getWeight(){return weight;}

};
#endif

SFMLEdge::SFMLEdge(float x0, float y0, float x1, float y1,std::string weight)
{
  this->x0=x0;
  this->y0=y0;
  this->x1=x1;
  this->y1=y1;
  this->weight=weight;
}
