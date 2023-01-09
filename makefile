CXX = g++

CPPFLAGS = -I -lm 

LIBS = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

OBJ = Main.o Calculator.o UI_TextBox.o UI_Button.o UI_ScrollBar.o MainRC.o

DEPS = Calculator.hpp UI_TextBox.hpp UI_Button.hpp UI_ScrollBar.hpp

Calculator.exe: $(OBJ)
	$(CXX) -o $@ $^ $(CPPFLAGS) $(LIBS)

UI_TextBox.o: UI_TextBox.cpp UI_TextBox.hpp
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(LIBS)

UI_Button.o: UI_Button.cpp UI_Button.hpp
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(LIBS)

UI_ScrollBar.o: UI_ScrollBar.cpp UI_ScrollBar.hpp UI_Button.hpp
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(LIBS)

Calculator.o: Calculator.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(LIBS)

Main.o: Main.cpp Main.hpp
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(LIBS)

.PHONY: clean

clean:
	del *.o *.exe