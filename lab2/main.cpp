#include <iostream>
#include <cassert>
#include "counted_ptr.h"

int main()
{
    //  Konstruktor från rå pekare 
    {
        counted_ptr<int> p{new int{42}};
        assert(p.use_count() == 1);
        assert(*p == 42);
        std::cout << "Konstruktor från rå pekare OK\n";
    }

    //  Standardkonstruktor 
    {
        counted_ptr<int> p{};
        assert(p.use_count() == 0);
        assert(p == nullptr);
        std::cout << "Standardkonstruktor OK\n";
    }

    // Konstruktor från nullptr 
    {
        counted_ptr<int> p{nullptr};
        assert(p.use_count() == 0);
        assert(p == nullptr);
        std::cout << "Konstruktor från nullptr OK\n";
    }

    // Kopkon
    {
        counted_ptr<int> p1{new int{10}};
        counted_ptr<int> p2{p1};
        assert(p1.use_count() == 2);
        assert(p2.use_count() == 2);
        assert(p1 == p2);
        assert(*p2 == 10);
        std::cout << "Kopieringskonstruktor OK\n";
    }

    // Koptill
    {
        counted_ptr<int> p1{new int{7}};
        counted_ptr<int> p2{new int{99}};
        p2 = p1;
        assert(p1.use_count() == 2);
        assert(p2.use_count() == 2);
        assert(*p2 == 7);
        std::cout << "Kopieringstilldelning OK\n";
    }


    //  Flyttkon
    {
        counted_ptr<int> p1{new int{3}};
        counted_ptr<int> p2{std::move(p1)};
        assert(p1 == nullptr);
        assert(p1.use_count() == 0);
        assert(*p2 == 3);
        assert(p2.use_count() == 1);
        std::cout << "Flyttkonstruktor OK\n";
    }

    // Flyttill
    {
        counted_ptr<int> p1{new int{8}};
        counted_ptr<int> p2{new int{55}};
        p2 = std::move(p1);
        assert(p1 == nullptr);
        assert(p1.use_count() == 0);
        assert(*p2 == 8);
        assert(p2.use_count() == 1);
        std::cout << "Flyttilldelning OK\n";
    }
    // ── get() ─────────────────────────────────────────────────────────────────────
    {
        int* raw = new int{55};
        counted_ptr<int> p{raw};
        assert(p.get() == raw);
        assert(p.use_count() == 1);

        std::cout << "get() OK\n";
    }

    std::cout << "\nAlla konstruktortester godkända!\n";
    return 0;
}