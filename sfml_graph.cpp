/*
Final Project: SFML graph
Name: Luis Enrique Güitrón A01018616
Name: Juan Pablo Güitrón A01019936
Data Structures
30/11/2016
a01019936@itesm.mx
a01018616@itesm.mx
*/

#include <iostream>
#include <exception>
#include "Graph.hpp"
#include "SFMLEdge.hpp"
#include "SFMLNode.hpp"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>

//Constants
#define PI 3.14159265259
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TEXT_SIZE 20
#define RADIUS 30
#define VERTEX_OUTLINE 4
#define ARROW_SIZE 10
#define ARROW_OFFSET (RADIUS+2*VERTEX_OUTLINE+0.5*ARROW_SIZE)



//Struct for thread parameters//
struct ThreadParameters
{
    Graph<std::string, float>* graph;
    std::vector<SFMLEdge>* sfmlEdgeList;
    std::vector<SFMLNode>* sfmlNodeList;
    bool* windowStatus;
    std::vector<Vertex<std::string, float> *> * path;
};

//Functions//
void userInput(ThreadParameters);
SFMLNode findSFMLNode(std::string,std::vector<SFMLNode>);
sf::Vertex* drawLine(unsigned,unsigned,unsigned,float,float,float,float);
sf::CircleShape drawArrow(unsigned,unsigned,unsigned,float,float,float,float);
sf::CircleShape drawVertex(unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,float,float);
std::string getNum();

int main()
{
    //Variables//
    Graph<std::string, float> graph;
    std::vector<SFMLEdge> sfmlEdgeList;
    std::vector<SFMLNode> sfmlNodeList;
    std::vector<Vertex<std::string, float> *> path;
    bool windowStatus=true;

    ThreadParameters parameters = {&graph,&sfmlEdgeList,&sfmlNodeList,&windowStatus,&path};

    // Create a thread for userInput function and runs it//
    sf::Thread thread(&userInput,parameters);
    thread.launch();

    //Main thread//
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Visual Graph");

    //Load Arial font//
    sf::Font font;
    font.loadFromFile("arial.ttf");

    //Dynamic view//
    sf::View view1;
    view1.reset(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

    while (window.isOpen() && windowStatus)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
               {
                   windowStatus=false;
                   window.close();
               }
        }
	
	//Controls for the View//
	if (event.type == sf::Event::KeyPressed)
	{	
    		if (event.key.code == sf::Keyboard::Up )
			view1.move(0,-5);
		
		if (event.key.code == sf::Keyboard::Down )
			view1.move(0,5);

		if (event.key.code == sf::Keyboard::Left )
			view1.move(-5,0);

		if (event.key.code == sf::Keyboard::Right )
			view1.move(5,0);
	}

        window.clear(sf::Color::White);

        //Variables used in loops for easier reading//
        Edge<std::string,float>* current_edge=nullptr;
        SFMLNode current_sfmlNode;
        SFMLEdge current_sfmlEdge;


        ////////////////////////////////////////////////////DRAW EDGES////////////////////////////////////////////
        for(int i=0;i<graph.getAmountOfEdges();i++)
        {
              current_sfmlEdge=sfmlEdgeList[i];
              current_edge=graph.getEdgeAtPosition(i);

              //DRAW BLACK ARROWS FOR ALL EDGES//
              sf::Vertex* line = drawLine(0,0,0,current_sfmlEdge.getX0(),current_sfmlEdge.getY0(),current_sfmlEdge.getX1(),current_sfmlEdge.getY1());
              sf::CircleShape arrow = drawArrow(0,0,0,current_sfmlEdge.getX0(),current_sfmlEdge.getY0(),current_sfmlEdge.getX1(),current_sfmlEdge.getY1());
              window.draw(line , 2, sf::Lines);
              window.draw(arrow);

              //DRAW EDGES WEIGHT IN BLACK FOR ALL EDGES//
              sf::Text Weight(current_sfmlEdge.getWeight(),font,TEXT_SIZE);
              Weight.setColor(sf::Color::Black);
              Weight.setPosition((current_sfmlEdge.getX0()+current_sfmlEdge.getX1())/2,(current_sfmlEdge.getY0()+current_sfmlEdge.getY1())/2);
              window.draw(Weight);


              //DRAW PATH (EDGES)//
              if(path.size()>0)
              {
                  for(unsigned j=0;j<path.size()-1;j++)
                  {
                      if(path[j]->getData()==current_edge->getOrigin()->getData() && path[j+1]->getData()==current_edge->getDestination()->getData())
                      {
                          //DRAW RED ARROWS FOR EDGES IN PATH//
                          sf::Vertex* line = drawLine(255,0,0,current_sfmlEdge.getX0(),current_sfmlEdge.getY0(),current_sfmlEdge.getX1(),current_sfmlEdge.getY1());
                          sf::CircleShape arrow = drawArrow(255,0,0,current_sfmlEdge.getX0(),current_sfmlEdge.getY0(),current_sfmlEdge.getX1(),current_sfmlEdge.getY1());
                          window.draw(line , 2, sf::Lines);
                          window.draw(arrow);

                          //DRAW EDGES WEIGHT IN RED FOR EDGES IN PATH//
                          sf::Text Weight(current_sfmlEdge.getWeight(),font,TEXT_SIZE);
                          Weight.setColor(sf::Color::Red);
                          Weight.setPosition((current_sfmlEdge.getX0()+current_sfmlEdge.getX1())/2,(current_sfmlEdge.getY0()+current_sfmlEdge.getY1())/2);
                          window.draw(Weight);
                      }
                  }
              }

        }

        /////////////////////////////////////////////////////DRAW VERTICES//////////////////////////////////////////////////
        for(int i=0;i<graph.getAmountOfVertices();i++)
        {
            current_sfmlNode=sfmlNodeList[i];

            //DRAW ALL VERTICES IN GREEN WITH BLACK OUTLINE//
            sf::CircleShape vertex = drawVertex(0,175,0,0,0,0,current_sfmlNode.getX(),current_sfmlNode.getY());
            window.draw(vertex);

            //DRAW DATA FOR ALL VERTICES IN BLACK//
            sf::Text NodeInfo(current_sfmlNode.getData(),font,TEXT_SIZE);
            NodeInfo.setColor(sf::Color(0,0,0));
            NodeInfo.setPosition(current_sfmlNode.getX()+RADIUS-0.3*TEXT_SIZE,current_sfmlNode.getY()+RADIUS-0.7*TEXT_SIZE);
            window.draw(NodeInfo);

            //DRAW PATH (VERTICES)//
            if(path.size()>0 )
            {
                for(unsigned j=0;j<path.size();j++)
                {
                    if(path[j]->getData()==current_sfmlNode.getData())
                    {
                        //DRAW PATH VERTICES IN ORANGE WITH BROWN OUTLINE//
                        sf::CircleShape vertex = drawVertex(255,165,0,139,69,19,current_sfmlNode.getX(),current_sfmlNode.getY());
                        window.draw(vertex);

                        //DRAW DATA FOR PATH VERTICES IN BLUE//
                        sf::Text NodeInfo(current_sfmlNode.getData(),font,TEXT_SIZE);
                        NodeInfo.setColor(sf::Color(0,0,128));
                        NodeInfo.setPosition(current_sfmlNode.getX()+RADIUS-0.3*TEXT_SIZE,current_sfmlNode.getY()+RADIUS-0.7*TEXT_SIZE);
                        window.draw(NodeInfo);
                    }
                }
            }
        }

	window.setView(view1);
        window.display();
    }

    return 0;
}

void userInput(ThreadParameters parameters)
{

    //Variables for user inputs//
    std::string selectedOption;
    std::string xInput,yInput,radius,nodeInfo;
    std::string origin,destination,weight;

    while(*parameters.windowStatus)
    {
        std::cout<<std::endl<<"____________Visual Graph_____________"<<std::endl;
        std::cout<<"1. Add node with parameters"<<std::endl;
        std::cout<<"2. Add edge"<<std::endl;
        std::cout<<"3. Find path"<<std::endl;
        std::cout<<"4. Quit"<<std::endl;
        std::cout<<"Your selection: ";
        std::cin>>selectedOption;

        if(selectedOption=="1")
        {
            std::cout<<"Enter x coordinate: ";
            xInput= getNum();
            std::cout<<"Enter y coordinate: ";
            yInput= getNum();
            std::cout<<"Enter node data: ";
            std::cin>>nodeInfo;

            SFMLNode info = findSFMLNode(nodeInfo,*parameters.sfmlNodeList);
            if(info.getRadius()== -1 && xInput!="false" && yInput!="false" )
            {
            	//Insert sfml node into the list//
            	SFMLNode new_node(std::stof(xInput),std::stof(yInput),RADIUS,nodeInfo);
            	parameters.sfmlNodeList->push_back(new_node);

            	//Insert vertex to the graph//
            	parameters.graph->addVertex(nodeInfo);
            }
            else
              std::cout<<"Please enter valid information for the vertex"<<std::endl;


        }

        else if(selectedOption=="2")
        {

            std::cout<<"Enter origin vertex: ";
            std::cin>>origin;
            std::cout<<"Enter destination vertex: ";
            std::cin>>destination;
            std::cout<<"Enter weight: ";
            weight=getNum();

            SFMLNode originNode = findSFMLNode(origin,*parameters.sfmlNodeList);
            SFMLNode destinationNode = findSFMLNode(destination,*parameters.sfmlNodeList);

            if(originNode.getRadius()!= -1 && destinationNode.getRadius()!=-1 && weight!="false")
            {

              float x0 = originNode.getX()+RADIUS;
              float y0 = originNode.getY()+RADIUS;
              float x1 = destinationNode.getX()+RADIUS;
              float y1 = destinationNode.getY()+RADIUS;

              //Insert sfml edge into the list//
              SFMLEdge new_edge(x0,y0,x1,y1,weight);
              parameters.sfmlEdgeList->push_back(new_edge);

              //Insert edge into the graph//
              parameters.graph->addEdge(origin,destination,std::stof(weight));
            }
            else
                std::cout<<"Error while adding the edge, please insert valid information and verify that the vertex exist"<<std::endl;
        }

        else if(selectedOption=="3")
        {

            std::cout<<"Enter origin node data: ";
            std::cin>>origin;
            std::cout<<"Enter destination node data: ";
            std::cin>>destination;  
            SFMLNode originNode = findSFMLNode(origin,*parameters.sfmlNodeList);
            SFMLNode destinationNode = findSFMLNode(destination,*parameters.sfmlNodeList);
  
            if(originNode.getRadius()!= -1 && destinationNode.getRadius()!=-1)
            {
              *parameters.path = *parameters.graph->findPath(origin,destination);
		
            }
            else
              std::cout<<"The path could not be made because one of the vertices does not exist"<<std::endl;

        }

        else if(selectedOption=="4")
            *parameters.windowStatus=false;

        else
            std::cout<<"Invalid option"<<std::endl;
    }
}

SFMLNode findSFMLNode(std::string data,std::vector<SFMLNode> sfmlNodeList)
{
    for(unsigned i=0;i<sfmlNodeList.size();i++)
    {
        if(sfmlNodeList[i].getData()==data)
        {
            return sfmlNodeList[i];
        }
    }
    //Radius of -1 will help identify an empty node
    SFMLNode empty_node(0,0,-1,"empty");
    return empty_node;
}

sf::Vertex* drawLine(unsigned r, unsigned g, unsigned b, float x0, float y0, float x1, float y1)
{
    sf::Vertex* line = new sf::Vertex[2];
    line[0]=sf::Vertex(sf::Vector2f(x0, y0), sf::Color(r,g,b));
    line[1]=sf::Vertex(sf::Vector2f(x1, y1), sf::Color(r,g,b));
    return line;
}


sf::CircleShape drawArrow(unsigned r, unsigned g, unsigned b, float x0, float y0, float x1, float y1)
{
    //Draw arrow in the edge//
    double deltaX = x1-x0;
    double deltaY = y1-y0;
    double rotationAngle = atan(deltaX/deltaY)*180/PI;
    sf::CircleShape triangle(ARROW_SIZE,3);
    triangle.setFillColor(sf::Color(r,g,b));
    triangle.setOrigin(sf::Vector2f(ARROW_SIZE,ARROW_SIZE));
    triangle.move(sf::Vector2f(x1,y1));

    //SET ARROW ROTATION AND POSITION//
    //Quadrants 1 and 2
    if(deltaY<0)
    {
        triangle.move(-ARROW_OFFSET*sin((-rotationAngle)*(PI/180)),ARROW_OFFSET*cos((-rotationAngle)*(PI/180)));
        triangle.rotate(-1*rotationAngle);
    }

    //Quadrants 3 and 4
    else
    {
      triangle.move(-ARROW_OFFSET*sin((-1*rotationAngle+180)*(PI/180)),ARROW_OFFSET*cos((-1*rotationAngle+180)*(PI/180)));
      triangle.rotate(-1*rotationAngle+180);
    }
    return triangle;
}

sf::CircleShape drawVertex(unsigned rf,unsigned gf,unsigned bf,unsigned ro,unsigned go,unsigned bo,float x,float y)
{
    sf::CircleShape circle;
    circle.setPosition(x,y);
    circle.setRadius(RADIUS);
    circle.setFillColor(sf::Color(rf,gf,bf));
    circle.setOutlineColor(sf::Color(ro,go,bo));
    circle.setOutlineThickness(VERTEX_OUTLINE);
    return circle;
}

std::string getNum()
{
	std::string a;
	std::cin>>a;
	try
	{
		std::stof(a);
		return a;
	}
	catch(std::exception& e)
	{
		return "false";
	}
	
}
