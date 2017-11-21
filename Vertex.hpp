/*
Final Project: SFML graph
Name: Luis Enrique Güitrón A01018616
Name: Juan Pablo Güitrón A01019936
Data Structures
30/11/2016
a01019936@itesm.mx
a01018616@itesm.mx
*/

#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <iostream>
#include "Edge.hpp"
#include <vector>

template <class T, class W>
class Vertex {
    private:
        T data;
        bool visited;
        std::vector<Edge<T, W> *> edges;
    public:
        Vertex();
        Vertex(const T & _data);
        ~Vertex() { clearEdges(); }
        T getData() { return data; }
        void setData(const T & _data) { data = _data; }
        void visit() { visited = true; }
        void unvisit() { visited = false; }
        bool isVisited() { return visited; }
        std::vector<Edge<T, W> *> * getEdges() { return &edges; }
        void addEdge(Edge<T, W> * _edge);
        void clearEdges();
        template <class R, class S>
        friend std::ostream & operator<< (std::ostream & stream, const Vertex<R, S> & vertex);
};

template <class T, class W>
Vertex<T, W>::Vertex()
{
    visited = false;
    edges = nullptr;
}

template <class T, class W>
Vertex<T, W>::Vertex(const T & _data)
{
    data = _data;
    visited = false;
}

template <class T, class W>
void Vertex<T, W>::addEdge(Edge<T, W> * _edge)
{
    edges.push_back(_edge);
}

template <class T, class W>
void Vertex<T, W>::clearEdges()
{
    for(unsigned i=0; i<edges.size();i++)
    {
        delete edges[i];
    }
}

template <class T, class W>
std::ostream & operator<< (std::ostream & stream, const Vertex<T, W> & vertex)
{
    stream << "Data: " << vertex.data << " | " << (vertex.visited ? "" : "not ") << "visited";
    return stream;
}

#endif
