CXX=g++
CXXFLAGS += -I src/ -std=c++11 -Wextra -Wundef -Wall -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code

.PHONY: all clean

all: parse_tester ngrep2dot

clean:
	-rm src/*.o src/parser/*.o src/parse_tester/*.o src/ngrep2dot/*.o parse_tester ngrep2dot

parse_tester: src/parse_tester/parse_tester.o src/parser/ngrep_log_parser.o
	$(CXX) $(CXXFLAGS) src/parse_tester/parse_tester.o src/parser/ngrep_log_parser.o -o parse_tester

ngrep2dot: src/ngrep2dot/main.o src/parser/ngrep_log_parser.o
	$(CXX) $(CXXFLAGS) src/ngrep2dot/main.o src/parser/ngrep_log_parser.o -o ngrep2dot
