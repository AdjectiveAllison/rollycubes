CXX=g++
CXXFLAGS=-std=c++20 -Wall -IuWebSockets/src -IuWebSockets/uSockets/src -Iincludes
LDFLAGS=-lz -lpthread -lssl -lcrypto
TARGET=GameServer

SRC=$(wildcard src/*.cpp)
OBJ=$(SRC:.cpp=.o)
DEP=$(OBJ:.o=.d)

$(TARGET): $(OBJ) $(UWS)
	cd uWebSockets/uSockets && WITH_SSL=0 make
	$(CXX) -flto $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS) uWebSockets/uSockets/*.o;

%.d: %.cpp
	@$(CXX) $(CXXFLAGS) $< -MM -MT $(@:.d=.o) >$@

src/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

-include $(DEP)

.PHONY: clean
clean:
	rm -f **/*.o
	rm -f GameServer
	rm -f **/*.d
	cd uWebSockets/uSockets && make clean

debug: clean
debug: CXXFLAGS += -g
debug: $(TARGET)

release: clean
release: CXXFLAGS += -O2
release: $(TARGET)

all: $(TARGET)
