######################################################################
# need to install libunwind-dev and binutils:
# sudo  apt install libunwind-dev binutils
######################################################################

CXX:=g++
CXXFLAGS:=-g -std=c++20 -rdynamic

default: stacktrace
	./stacktrace

stacktrace : stacktrace.cc
	$(CXX) $(CXXFLAGS) -o $@ $^

clean :
	rm -rf *~ stacktrace
