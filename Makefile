
CXX=g++
CXXFLAGS=

SOURCES=assembler.cc inst_lower.cc main.cc register_allocator.cc ir/type.cc ir/value.cc
OBJECTS=$(SOURCES:.cc=.o)

all: vm

vm : $(OBJECTS)
	${CXX} ${OBJECTS} -o vm

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS)
