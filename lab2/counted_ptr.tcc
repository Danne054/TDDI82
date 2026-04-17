#ifndef COUNTED_PTR_TCC
#define COUNTED_PTR_TCC

#include "counted_ptr.h"

//  Konstruktor av en rå pekare 
template <typename T>
counted_ptr<T>::counted_ptr(T* ptr)
    : data{ptr}, count{new int{1}}
{}

//  Standardkonstruktor (blir nullpter typ)
template <typename T>
counted_ptr<T>::counted_ptr()
    : data{nullptr}, count{nullptr}
{}

// Konstruktor från nullptr 
template <typename T>
counted_ptr<T>::counted_ptr(std::nullptr_t)
    : data{nullptr}, count{nullptr}
{}

// Kopieringskonstruktor 
template <typename T>
counted_ptr<T>::counted_ptr(counted_ptr<T> const& other)
    : data{other.data}, count{other.count}
{
    if (count != nullptr)
        ++(*count);
}

//  Kopieringstilldelning 
template <typename T>
counted_ptr<T>& counted_ptr<T>::operator=(counted_ptr<T> const& other)
{
    if (this != &other)
    {
        release();
        data  = other.data;
        count = other.count;
        if (count != nullptr)
            ++(*count);
    }
    return *this;
}

// Flyttkonstruktor 
template <typename T>
counted_ptr<T>::counted_ptr(counted_ptr<T>&& other) noexcept
    : data{other.data}, count{other.count}
{
    other.data  = nullptr;
    other.count = nullptr;
}

// Flyttilldelning 
template <typename T>
counted_ptr<T>& counted_ptr<T>::operator=(counted_ptr<T>&& other) noexcept
{
    if (this != &other)
    {
        release();
        data  = other.data;
        count = other.count;
        other.data  = nullptr;
        other.count = nullptr;
    }
    return *this;
}

//Destruktor 
template <typename T>
counted_ptr<T>::~counted_ptr()
{
   release();
}


//avrefererar pekare
template <typename T>
int counted_ptr<T>::use_count() const
{
    if (count == nullptr)
        return 0;
    return *count;
}

template <typename T>
T& counted_ptr<T>::operator*() 
{
    return *data;
}
template <typename T>
T counted_ptr<T>::operator*() const
{
    return *data;
}

template <typename T>
T* counted_ptr<T>::operator->() noexcept
{
    return data;
}

template <typename T>
bool counted_ptr<T>::operator==(counted_ptr<T> const& other) const
{
    return data == other.data;
}

template <typename T>
bool counted_ptr<T>::operator!=(counted_ptr<T> const& other) const
{
    return data != other.data;
}

template <typename T>
bool counted_ptr<T>::operator==(T const* ptr) const
{
    return data == ptr;
}

template <typename T>
bool counted_ptr<T>::operator==(T const* ptr) const
{
    return data == ptr;
}

template <typename T>
bool counted_ptr<T>::operator!=(T const* ptr) const
{
    return data != ptr;
}

template <typename T>
bool  counted_ptr<T>::operator==(std::nullptr_t) const
{
    return  data == nullptr;
}

template <typename T>
bool  counted_ptr<T>::operator!=(std::nullptr_t) const
{
    return data != nullptr;
}

template <typename T>
void counted_ptr<T>::release()
{
    if (count == nullptr)
    {
        return;
    }
    --(*count);
    if (*count == 0)
    {
        delete data;
        delete count;
    }
    data = nullptr;
    count = nullptr;
}


#endif