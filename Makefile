CLIBS=`sdl2-config --libs`

all: iotograph

test: all sineTest
	./sineTest | ./iotograph 1

sineTest: sineTest.cpp
	$(CXX) -o $@ $^ $(CLIBS)
iotograph: obj/main.o obj/ldc/Window.o
	$(CXX) -o $@ $^ $(CLIBS)
	
obj/%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) -c -o $@ $< $(CLIBS)

clear:
	$(RM) -r obj/* sineTest iotograph