OBJ = main.o intern.o  func.o parse.o

terminal: $(OBJ)
	gcc -o $@ $^ -lreadline

clean:
	rm -f terminal *.o
