#include "node.h"
#include <iostream>
#include <string>

counted_ptr<Node<std::string>> create_example()
{
    counted_ptr<Node<std::string>> n0 { new Node<std::string> { "noll" } };
    counted_ptr<Node<std::string>> n1 { new Node<std::string> { "ett" } };
    counted_ptr<Node<std::string>> n2 { new Node<std::string> { "två" } };
    counted_ptr<Node<std::string>> n3 { new Node<std::string> { "tre" } };
    counted_ptr<Node<std::string>> n4 { new Node<std::string> { "fyra" } };
    counted_ptr<Node<std::string>> n5 { new Node<std::string> { "fem" } };

    n0->insert(n1);
    n0->insert(n2);
    n1->insert(n5);
    n2->insert(n3);
    n2->insert(n4);
    n3->insert(n4);
    n3->insert(n5);

    return n0;
}

void print_nodes(std::vector<counted_ptr<Node<std::string>>> nodes)
{
    for (counted_ptr<Node<std::string>> node : nodes)
    {
        std::cout << node->value << " ";
    }
    std::cout << std::endl;
}

int main()
{
    counted_ptr<Node<std::string>> root { create_example() };
    std::vector<counted_ptr<Node<std::string>>> nodes { get_all_nodes(root) };

    std::cout << "After insertions: " << std::endl;
    print_nodes(nodes);

    root->remove(nodes[2]);
    nodes = get_all_nodes(root);

    std::cout << "After removal: " << std::endl;
    print_nodes(nodes);
}
