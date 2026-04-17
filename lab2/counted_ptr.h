#ifndef COUNTED_PTR_H
#define COUNTED_PTR_H

#include <cstddef>  // std::nullptr_t

template <typename T>
class counted_ptr
{
public:
    // Konstruktor från rå pekare
    explicit counted_ptr(T* ptr);

    // Standardkonstruktor – motsvarar nullptr
    counted_ptr();

    // Konstruktor från nullptr
    counted_ptr(std::nullptr_t);

    // Kopieringskonstruktor
    counted_ptr(counted_ptr<T> const& other);

    // Kopieringstilldelning
    counted_ptr<T>& operator=(counted_ptr<T> const& other);


    // Flyttkonstruktor
    counted_ptr(counted_ptr<T>&& other) noexcept;

    // Flyttilldelning
    counted_ptr<T>& operator=(counted_ptr<T>&& other) noexcept;

    // Destruktor
    ~counted_ptr();

    // Antal delade pekare
    int use_count() const;

    //*operator
    //counted_ptr<T>&
    T& operator*();
    T operator*() const;

    T* operator->();
    // T& operator->() const;

    //jämför med samma obj
    bool operator==(counted_ptr<T> const& other) const;
    bool operator!=(counted_ptr<T> const& other) const;

    //jämför med pekare
    bool operator==(T const* ptr) const;
    bool operator!=(T const* ptr) const;

    //jämför med null ptr
    bool operator==(std::nullptr_t) const;
    bool operator!=(std::nullptr_t) const;

    T* get() const;

private:
    T*   data;   // Pekare till objektet
    int* count;  // Delad räknare 

    void release();
};

#include "counted_ptr.tcc"

#endif
