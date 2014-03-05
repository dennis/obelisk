CXXFLAGS+=-Wall -W -Wcast-align -Wshadow -Wundef
CXXFLAGS+=-ggdb

all: obelisk

obelisk: main.o obelisk.o parser.o line.o channel_line.o parsed_file.o clause.o
	$(CXX) $? -o $@ $(LDFLAGS)

clean:
	rm *.o obelisk

.phony: all clean
