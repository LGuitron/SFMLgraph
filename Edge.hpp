/*
Final Project: SFML graph
Name: Luis Enrique Güitrón A01018616
Name: Juan Pablo Güitrón A01019936
Data Structures
30/11/2016
a01019936@itesm.mx
a01018616@itesm.mx
*/

#ifndef EDGE_HPP
#define EDGE_HPP

template <class T, class W>
class Vertex;

template <class T, class W>
class Edge {
    private:
        Vertex<T, W> * origin;
        Vertex<T, W> * destination;
        W weight;
    public:
        Edge(Vertex<T, W> * _origin=nullptr, Vertex<T, W> * _destination=nullptr, const W & _weight=1) :
            origin(_origin), destination(_destination), weight(_weight) {}
        void setWeight(const W & _weight) { weight = _weight; }
        W getWeight() { return weight; }
        Vertex<T, W> * getOrigin() { return origin; }
        Vertex<T, W> * getDestination() { return destination; }
};

#endif
