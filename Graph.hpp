/*
Final Project: SFML graph
Name: Luis Enrique Güitrón A01018616
Name: Juan Pablo Güitrón A01019936
Data Structures
30/11/2016
a01019936@itesm.mx
a01018616@itesm.mx
*/

#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include "Vertex.hpp"
#include "InfoNode.hpp"
#include <exception>

template <class T, class W>
class Graph {
    private:
        std::vector<Vertex<T, W> *> vertices;
        std::vector<Edge<T, W> *> edges;
        Vertex<T, W> * start;

        // Private methods
        void clearVisits();
        InfoNode<T, W> * initializeDijkstra(std::vector<InfoNode<T, W> *> * dijkstra, Vertex<T, W> * origin);
        void cleardijkstra(std::vector<InfoNode<T, W> *> * dijkstra);
        InfoNode<T, W> * getCheapest(std::vector<InfoNode<T, W> *> * dijkstra);
        std::vector<Vertex<T, W> *> * recoverPath(std::vector<InfoNode<T, W> *> dijkstra, InfoNode<T, W> * current_vertex_info_node);
    public:
        Graph() { start = nullptr; }
        ~Graph();
        void addVertex(const T & _data);
        void addVertex(Vertex<T, W> * _vertex);
        void addEdge(const T & origin_data, const T & destination_data, const W & _weight = 1);
        void addEdge(Vertex<T, W> * _origin, Vertex<T, W> * _destination, const W & _weight = 1);
        void addEdge(Edge<T, W> * _edge);
        Vertex<T, W> * getStart() { return start; }
        void setStart(Vertex<T, W> * _start) { start = _start; }
        void setStart(const T & _data);
        std::vector<Vertex<T, W> *> * findPath(Vertex<T, W> * origin, Vertex<T, W> * destination);
        std::vector<Vertex<T, W> *> * findPath(const T & origin, const T & destination);
        void printPath(std::vector<Vertex<T, W> *> * path);

        //Our Functions
        int getAmountOfVertices(){return vertices.size();}
        int getAmountOfEdges(){return edges.size();}
        Vertex<T,W>* getVertexAtPosition(int position){return vertices[position];}
        Edge<T,W>* getEdgeAtPosition(int position){return edges[position];}

};

template <class T, class W>
Graph<T, W>::~Graph()
{
    for(unsigned i = 0; i<vertices.size();i++)
    {
        delete vertices[i];
    }

    /*for(unsigned i=0;i<edges.size();i++)
    {
        delete edges[i];
    }*/
	
    // Now clear the lists in this class
    vertices.clear();
    edges.clear();
}

template <class T, class W>
void Graph<T, W>::addVertex(const T & _data)
{
    Vertex<T, W> * new_vertex = new Vertex<T, W>(_data);
    addVertex(new_vertex);
}

template <class T, class W>
void Graph<T, W>::addVertex(Vertex<T, W> * _vertex)
{
    vertices.push_back(_vertex);
}

template <class T, class W>
void Graph<T, W>::addEdge(const T & origin_data, const T & destination_data, const W & _weight)
{
    Vertex<T, W> * _origin = nullptr;
    Vertex<T, W> * _destination = nullptr;
    Vertex<T, W> * current_vertex = nullptr;

    for(unsigned i=0; i<vertices.size();i++)
    {
        current_vertex=vertices[i];
        if (current_vertex->getData() == origin_data)
            _origin = current_vertex;
        if (current_vertex->getData() == destination_data)
            _destination = current_vertex;
    }
    // Call the other methods in this class to add edges
    addEdge(_origin, _destination, _weight);
}

template <class T, class W>
void Graph<T, W>::addEdge(Vertex<T, W> * _origin, Vertex<T, W> * _destination, const W & _weight)
{
    if (_origin != nullptr && _destination != nullptr)
    {
        Edge<T, W> * new_edge = new Edge<T, W>(_origin, _destination, _weight);
        addEdge(new_edge);
    }
}

template <class T, class W>
void Graph<T, W>::addEdge(Edge<T, W> * _edge)
{
    if (_edge != nullptr)
    {
        // Insert the edge to the list in the graph
        edges.push_back(_edge);
        // Add the edge to the list in the origin vertex
        _edge->getOrigin()->addEdge(_edge);
    }
}

template <class T, class W>
void Graph<T, W>::setStart(const T & _data)
{
    Vertex<T, W> * current_vertex = nullptr;
    // Look for the vertices that contain the data specified
    for(unsigned i=0; i<vertices.size();i++)
    {
        current_vertex = vertices[i];
        if (current_vertex->getData() == _data)
        {
            setStart(current_vertex);
            return;
        }
    }
}


template <class T, class W>
void Graph<T, W>::clearVisits()
{
    // Set all vertices as not visited
    Vertex<T, W> * current_vertex;

    for(unsigned i = 0; i<vertices[i];i++)
    {
        current_vertex = vertices[i];
        current_vertex->unvisit();
    }
}

template <class T, class W>
std::vector<Vertex<T, W> *> * Graph<T, W>::findPath(const T & origin_data, const T & destination_data)
{
    Vertex<T, W> * _origin = nullptr;
    Vertex<T, W> * _destination = nullptr;
    Vertex<T, W> * current_vertex = nullptr;
 
    // Look for the vertices that contain the data specified
    for(unsigned i = 0;i<vertices.size();i++)
    {
        current_vertex = vertices[i];
        if (current_vertex->getData() == origin_data)
            _origin = current_vertex;
        if (current_vertex->getData() == destination_data)
            _destination = current_vertex;
    }

    // Find the path from the origin vertex to the destination vertex
    return findPath(_origin, _destination);
}

template <class T, class W>
std::vector<Vertex<T, W> *> * Graph<T, W>::findPath(Vertex<T, W> * origin, Vertex<T, W> * destination)
{
    std::vector<InfoNode<T, W> *> dijkstra;
    InfoNode<T, W> * info_node = nullptr;
    InfoNode<T, W> * current_vertex_info_node = nullptr;
    Vertex<T, W> * current_vertex = nullptr;
    Vertex<T, W> * neighbour = nullptr;
    std::vector<Edge<T, W> *> * edge_list = nullptr;
    std::vector<Vertex<T, W> *> * path = nullptr;

    // Set the current_vertex vertex to the starting point (origin)
    current_vertex = origin;
    // Initialize the "table" for the Dijkstra algorithm
    current_vertex_info_node = initializeDijkstra(&dijkstra, origin);

    // Loop until the destination is reached
    while (current_vertex_info_node != nullptr and current_vertex != destination)
    {
         // Get the list of edges      
	edge_list = current_vertex->getEdges();
        // Get the node with the first edge

        for(unsigned i =0; i<edge_list->size();i++)
        {
            neighbour = edge_list->at(i)->getDestination();

            for(unsigned j=0 ; j<dijkstra.size(); j++)
            {
                info_node = dijkstra[j];
                if (info_node->getVertex() == neighbour)
                {
                    // Add cost of the current_vertex node and the edge weight to the neighbour
                    W new_cost = current_vertex_info_node->getCost() + edge_list->at(i)->getWeight();
                    // If the new cost is lower, than the previous, or if no cost has been determined yet
                    if (info_node->getCost() == -1 or new_cost < info_node->getCost())
                    {
                        info_node->setCost(new_cost);
                        info_node->setPrevious(current_vertex);
                        break;
                    }
                }
            }
        }
	

        // Continue by checking the next cheapest item in the dijkstra list
        current_vertex_info_node = getCheapest(&dijkstra);
	current_vertex_info_node->visit();
       	current_vertex = current_vertex_info_node->getVertex();
	
    }

    // Recover the path from origin to destination
    path = recoverPath(dijkstra, current_vertex_info_node);

    // Clear the memory used by the Dijkstra list
    cleardijkstra(&dijkstra);

    return path;
}


// Create the table with the information needed to determine the path using dijkstra
// Receives the list, the head of the list of vertices and the origin vertex
// Returns the info node for the origin vertex
template <class T, class W>
InfoNode<T, W> * Graph<T, W>::initializeDijkstra(std::vector<InfoNode<T, W> *> * dijkstra, Vertex<T, W> * origin)
{
    InfoNode<T, W> * info_node = nullptr;
    InfoNode<T, W> * origin_info_node = nullptr;
    Vertex<T, W> * current_vertex = nullptr;

    // Fill the dijkstra list with info nodes
    for(unsigned i =0;i<vertices.size();i++)
    {
        current_vertex = vertices[i];
        info_node = new InfoNode<T, W> (current_vertex);
        if (current_vertex == origin)
        {
            origin_info_node = info_node;
            //Mark origin as visited to avoid visiting it twice
            origin_info_node->visit();
            info_node->setCost(0);
        }
        dijkstra->push_back(info_node);
    }

    return origin_info_node;
}

// Free all the data nodes created for the Dijkstra table
template <class T, class W>
void Graph<T, W>::cleardijkstra(std::vector<InfoNode<T, W> *> * dijkstra)
{
    InfoNode<T, W> * info_node = nullptr;
    for(unsigned i=0;i<dijkstra->size();i++)
    {
        info_node = dijkstra->at(i);
        delete info_node;
    }
}

template <class T, class W>
InfoNode<T, W> * Graph<T, W>::getCheapest(std::vector<InfoNode<T, W> *> * dijkstra)
{
    InfoNode<T, W> * info_node = nullptr;
    InfoNode<T, W> * lowest_vertex = nullptr;
    // TODO: Find a better way to initialize the cost
    W lowest_cost = 99999999;

    for(unsigned i =0; i<dijkstra->size();i++)
    {
        info_node = dijkstra->at(i);
        if (info_node->getCost() != -1 and info_node->getCost() < lowest_cost and !info_node->isVisited())
        {
            lowest_cost = info_node->getCost();
            lowest_vertex = info_node;
        }
    }
    return lowest_vertex;
}

// Recover the path from origin to destination
template <class T, class W>
std::vector<Vertex<T, W> *> * Graph<T, W>::recoverPath(std::vector<InfoNode<T, W> *> dijkstra, InfoNode<T, W> * current_vertex_info_node)
{
    std::vector<Vertex<T, W> *> * path = new std::vector<Vertex<T, W> *>;
    InfoNode<T, W> * info_node = nullptr;

    // Search the previous vertices in the Dijkstra list
    while(current_vertex_info_node->getPrevious() != nullptr)
    {
        // Insert the vertex in to the path list
        path->insert(path->begin(),current_vertex_info_node->getVertex());
        // Look in the list of info nodes for the previous vertex

        for(unsigned i = 0; i<dijkstra.size();i++)
        {
            info_node = dijkstra[i];
            // Get the vertex that is marked as previous in this info node
            if (info_node->getVertex() == current_vertex_info_node->getPrevious())
            {
                current_vertex_info_node = info_node;
                break;
            }
        }
    }
    // Insert the origin vertex in the list
    path->insert(path->begin(),current_vertex_info_node->getVertex());
    return path;
}

// Method to print the contents of a list of vertices as a path
template <class T, class W>
void Graph<T, W>::printPath(std::vector<Vertex<T, W> *> * path)
{
    Vertex<T, W> * vertex;

    for(unsigned i=0;i<path->size();i++)
    {
        vertex = path->at(i);
        std::cout << (*vertex).getData();
        if (i < path->size()-1)
            std::cout << " -> ";
        else
            std::cout << std::endl;
    }
}
#endif
