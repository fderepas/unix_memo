/**
 * @file a.cc
 * @brief A simple example to test memory allocation in web assembly.
 *
 * To compile into a regular c++ excutable an execute:
 *     g++ -Wall -O2 -std=c++20 wasm.cc && ./a.out
 * 
 * To compile into web assembly:
 *     em++ -DJS_BINDINGS -o wasm.js -Wall -O2 -std=c++20 wasm.cc --bind
 * Then look at a.html though a web server. For instance to launch a
 * server in the directory where this file is type on the command line:
 *     python -m http.server 8080
 * then using a browser go to http://127.0.0.1:8080/wasm.html
 */

#include <map>
#include <iostream>
#include <cstdlib>  // rand srand
#include <string.h> // memset

using namespace std;

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

Allocator<A> * myAllocator = nullptr;

void * A::operator new(std::size_t sz) noexcept {
    return static_cast<void*>(myAllocator->get());
}

void A::operator delete(void* ptr) noexcept {
    myAllocator->rm(static_cast<A*>(ptr));
}

class RandomAllocatorOfA {
public:
    RandomAllocatorOfA(int size): allocator(size) {
        // initialize the random seed on the clock
        srand((unsigned) time(NULL));
        myAllocator=&allocator;
    }
    /**
     * @brief perform random allocation/deallocation.
     */
    int performAllocations() {
        int totalAlloc=0;
        int countAllocatedInstances=0;
        int loopCount=10000;
        int sz=allocator.size;
        // keep track of allocations performed
        A** arr = new A*[sz];
        memset(arr,0,sizeof(A*)*sz);
        while (loopCount-->0) {
            int imax =
                (sz-countAllocatedInstances)>1?
                rand()%(sz-countAllocatedInstances-1)+1:
                0;
            // perform imax allocations
            int index=0;
            for (int i=0;index<sz && i<imax;++i) {
                while (arr[index] && index<sz) ++index;
                if (index<sz) {
                    arr[index]=new A(rand());
                    if (arr[index]!=nullptr) {
                        countAllocatedInstances++;
                        totalAlloc++;
                    } else {
                        cerr << "---" << countAllocatedInstances << endl;
                    }
                }
            }
            int maxDeAlloc = rand()%countAllocatedInstances;
            // free randomly a maximum of maxDeAlloc instances
            for (int i=0;i<maxDeAlloc;++i) {
                // get a random index
                int index = rand()%sz ;
                // delete if it's an allocated node
                if (arr[index]!=nullptr) {
                    delete arr[index];
                    arr[index]=nullptr; // forget the node
                    countAllocatedInstances--;
                }
            }
        }
        delete [] arr;
        return totalAlloc;
    }
    Allocator<A> allocator;
};

/*** *** *** beging c++/js bindings *** *** ***/
#ifdef JS_BINDINGS
#include <emscripten.h>
#include <emscripten/bind.h>
#include <sstream>

int getRandomAllocatorOfA(int size) {
    return (int)new RandomAllocatorOfA(size);
}

int performAllocations(int ptr) {
    RandomAllocatorOfA * randomAllocatorOfA = (RandomAllocatorOfA*)ptr;
    return randomAllocatorOfA->performAllocations();
}

int getBufferSize(int ptr) {
    RandomAllocatorOfA * randomAllocatorOfA = (RandomAllocatorOfA*)ptr;
    return randomAllocatorOfA->allocator.size;
}

EMSCRIPTEN_BINDINGS(module) {
    emscripten::function("getRandomAllocatorOfA", &getRandomAllocatorOfA);
    emscripten::function("performAllocations", &performAllocations);
    emscripten::function("getBufferSize", &getBufferSize);
}

/* we expect the following js code to call these c++ functions:
function RandomAllocatorOfA(size) {
    this.ptr = Module.getRandomAllocatorOfA(size);
    this.performAllocations = function () {
        return Module.performAllocations(this.ptr);
    }
    this.getBufferSize = function () {
        return Module.getBufferSize(this.ptr);
    }
}
*/
#endif
/*** *** *** end of bindings *** *** ***/

#ifndef JS_BINDINGS
// when not using web assembly: create a "main" function to have a
// regular c++ executable
int main () {
    RandomAllocatorOfA randomAllocatorOfA(1024);
    std::cout.imbue(std::locale(""));
    cout << "performed " << randomAllocatorOfA.performAllocations()
         << " allocations in a buffer of size "
         << myAllocator->size << "." << endl;
    return 0;
}
#endif
