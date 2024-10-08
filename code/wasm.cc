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

#define MAX_ALLOCATOR_SIZE 1024
Allocator<A> myAllocator(MAX_ALLOCATOR_SIZE);


void * A::operator new(std::size_t sz) noexcept {
    return static_cast<void*>(myAllocator.get());
}

void A::operator delete(void* ptr) noexcept {
    myAllocator.rm(static_cast<A*>(ptr));
}

class RandomAllocatorOfA {
public:
    RandomAllocatorOfA() {
        // initialize the random seed on the clock
        srand((unsigned) time(NULL));
    }
    /**
     * @brief perform random allocation/deallocation.
     */
    int performAllocations() {
        int totalAlloc=0;
        int countAllocatedInstances=0;
        int loopCount=100000;
        while (loopCount-->0) {
            A* arr[MAX_ALLOCATOR_SIZE];
            memset(arr,0,sizeof(arr));
            int imax =
                (MAX_ALLOCATOR_SIZE-countAllocatedInstances)>1?
                rand()%(MAX_ALLOCATOR_SIZE-countAllocatedInstances-1)+1:
                0;
            // perform imax allocations
            for (int i=0;i<imax;++i) {
                arr[i]=new A(rand());
                if (arr[i]!=nullptr) {
                    countAllocatedInstances++;
                    totalAlloc++;
                } else {
                    cerr << "---" << countAllocatedInstances << endl;
                }
            }
            int maxDeAlloc = rand()%countAllocatedInstances;
            // free randomly a maximum of maxDeAlloc nodes
            for (int i=0;i<maxDeAlloc;++i) {
                int index = rand()%MAX_ALLOCATOR_SIZE;
                // cerr << index << endl;
                if (arr[index]!=nullptr) {
                    delete arr[index];
                    arr[index]=nullptr;
                    countAllocatedInstances--;
                }
            }
            cout << countAllocatedInstances << endl;
        }
        return totalAlloc;
    }
};

/*** *** *** beging c++/js bindings *** *** ***/
#ifdef JS_BINDINGS
#include <emscripten.h>
#include <emscripten/bind.h>
#include <sstream>

int getRandomAllocatorOfA() {
    return (int)new RandomAllocatorOfA();
}

int performAllocations(int ptr) {
    RandomAllocatorOfA * randomAllocatorOfA = (RandomAllocatorOfA*)ptr;
    return randomAllocatorOfA->performAllocations();
}

int getBufferSize() {
    return myAllocator.size;
}

EMSCRIPTEN_BINDINGS(module) {
    emscripten::function("getRandomAllocatorOfA", &getRandomAllocatorOfA);
    emscripten::function("performAllocations", &performAllocations);
    emscripten::function("getBufferSize", &getBufferSize);
}

/* we expect the following js code to call these c++ functions:
function RandomAllocatorOfA() {
    this.ptr = Module.getRandomAllocatorOfA();
    this.performAllocations = function () {
        return Module.performAllocations(this.ptr);
    }
    this.getBufferSize = function () {
        return Module.getBufferSize();
    }
}
*/
#endif
/*** *** *** end of bindings *** *** ***/

#ifndef JS_BINDINGS
// when not using web assembly: create a "main" function to have a
// regular c++ executable
int main () {
    RandomAllocatorOfA randomAllocatorOfA;
    cout << "performed " << randomAllocatorOfA.performAllocations()
         << " allocations in a buffer of size "
         << MAX_ALLOCATOR_SIZE << "." << endl;
    return 0;
}
#endif
