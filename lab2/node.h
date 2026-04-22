#ifndef NODE_H_
#define NODE_H_

#include <vector>
#include "counted_ptr.h"

template <typename T>
class Node
{
public:
    //varför finns det två public?
   // Node(int value = 0);
   Node(T value = T{});

    //void insert(Node* node);
    void insert(counted_ptr<Node<T>> node);
    //void remove(Node* node);
    void remove(counted_ptr<Node<T>> node);

    //std::vector<Node*>::const_iterator begin() const;
    typename std::vector<counted_ptr<Node<T>>>::const_iterator begin() const;
    //std::vector<Node*>::const_iterator end() const;
    typename std::vector<counted_ptr<Node<T>>>::const_iterator end() const;

public:

    //int value { };
    T value { };

private:

    std::vector<counted_ptr<Node<T>>> neighbours { };

};

template <typename T>
//std::vector<Node*> get_all_nodes(Node* root);
std::vector<counted_ptr<Node<T>>> get_all_nodes(counted_ptr<Node<T>> root);

#include "node.tcc"

#endif
