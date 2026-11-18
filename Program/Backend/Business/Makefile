CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDLIBS   = -lpqxx -lpq

app: main.cpp asignaciones.hpp repositorio_postgres.hpp
	$(CXX) $(CXXFLAGS) main.cpp -o app $(LDLIBS)

clean:
	rm -f app

.PHONY: clean
