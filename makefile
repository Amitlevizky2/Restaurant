# All Targets
all: hello

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
hello: bin/HelloWorld.o bin/Run.o
	@echo 'Building target: hello'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/hello bin/HelloWorld.o bin/Run.o
	@echo 'Finished building target: hello'
	@echo ' '

# Depends on the source and header files
bin/HelloWorld.o: src/HelloWorld.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/HelloWorld.o src/HelloWorld.cpp

# Depends on the source and header files
bin/Run.o: src/Run.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Run.o src/Run.cpp

#Clean the build directory
clean:
	rm -f bin/*