// compile with :
//     g++ -g -std=c++20 -rdynamic -o stacktrace stacktrace.cc

#include <execinfo.h>   // For backtrace and backtrace_symbols
#include <signal.h>     // For signal handling
#include <stdlib.h>     // For exit
#include <unistd.h>     // For STDERR_FILENO
#include <cxxabi.h>     // to demangle
#include <iostream>
#include <sstream>
#include <cstdlib>

char *progName = nullptr;

void printStackTrace() {
    const int max_frames = 64;
    void* buffer[max_frames];
    
    // Capture up to max_frames stack frames
    int num_frames = backtrace(buffer, max_frames);

    // Translate addresses into an array of strings with function names
    char** symbols = backtrace_symbols(buffer, num_frames);
    if (symbols == nullptr) {
        std::cerr << "Error in backtrace_symbols.\n";
        exit(EXIT_FAILURE);
    }

    std::cerr << "Stack trace:\n";
    for (int i = 0; i < num_frames; ++i) {
        // only keep what is in between '(' and '+'
        char * b = symbols[i];
        while (*b!='(' && *b!=0) ++b; // go to '('
        if (*b!=0) ++b; // one character after '('
        char * c = b;
        while (*c!='+' && *c!=')' && *c!=0 && ((c-b)<100)) ++c;
        *c=0; // end the string at either '+' or ')'
        if (((int)*b) != 0) {
            // Extract the address from symbols[i]
            std::stringstream cmd;
            cmd << "addr2line -e " << progName << " " << b;
            // Run addr2line command and output the file and line number
            FILE* fp = popen(cmd.str().c_str(), "r");
            if (fp) {
                char result[128];
                int st;
                char* cxx_sname = abi::__cxa_demangle(b,nullptr,0,&st);
                while (fgets(result, sizeof(result), fp) != nullptr) {
                    std::cerr << "in " << cxx_sname << "\n    at " << result;
                }
                pclose(fp);
            }
        }
    }
    free(symbols);
}

void f1() { printStackTrace();}
void f2() { f1();} 
void f3() { f2();} 
void f4() { f3();} 
void f5() { f4();} 

int main(int argc, char ** argv) {
    progName = argv[0];
    f5();
    return 0;
}
