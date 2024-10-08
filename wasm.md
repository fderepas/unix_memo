Web assembly (wasm) enables to used C,C++ or Rust instead of Javascript.

Here is an example to manage memory when using wasm and C++.

First let's define an allocator. It will be used lated on to override ```new``` and ```delete``` operator:

```c++
/**
 * @class Allocator
 * @brief Class implementing a allocator using an array of type \par T.
 */
template <class T>
class Allocator {
public:
    Allocator(int size) : size(size) {
        data = (T*) malloc(sizeof(T) * size);
        for (int i=0;i<size;++i) {
            available.insert({&(data[i]),false});
        }
    }
    ~Allocator() {
        free(data);
    }
    T* get() {
        if (available.size()==0) {
            cerr << "No more memory." << endl;
            return nullptr;
        }
        T*answer = available.begin()->first;
        available.erase(answer);
        used.insert({answer,false});
        return answer;
    }
    void rm(T*t) {
        if (used.erase(t)) {
            available.insert({t,false});
        }
    }
    map<T*,bool> available;
    map<T*,bool> used;
    T * data;
    int size;
};
```

Now let's define a dummy class:
```c++
/**
 * @class A
 * @brief A simple class on which new and delete operators are going to be
 * called.
 */
class A {
public:
    A(int v=0): value(v) {}
    ~A() { value=0;}
    int value;

    void * operator new(std::size_t sz) noexcept;
    void operator delete(void* ptr) noexcept;
};
```
The allocator can be instanciated using:
```c++
Allocator<A> myAllocator(1024);

```
And ```new``` and ```delete``` operators can be overriden by:
```c++
void * A::operator new(std::size_t sz) noexcept {
    return static_cast<void*>(myAllocator.get());
}

void A::operator delete(void* ptr) noexcept {
    myAllocator.rm(static_cast<A*>(ptr));
}
``
Complete example is available using a [C++ file](https://github.com/fderepas/unix_memo/blob/main/code/wasm.cc) and an [html file](https://github.com/fderepas/unix_memo/blob/main/code/wasm.html).