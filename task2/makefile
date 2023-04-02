OBJECTWAY = obj/
MAINWAY = src/
BINWAY = bin/

app: $(BINWAY)app

$(BINWAY)app: $(OBJECTWAY)main.o $(OBJECTWAY)command.o $(OBJECTWAY)coder.o
	gcc -Wall -o $@ $^

$(OBJECTWAY)main.o: $(MAINWAY)main.c
	gcc -c -Wall -o $@ $^

$(OBJECTWAY)command.o: $(MAINWAY)command.c
	gcc -c -Wall -o $@ $^

$(OBJECTWAY)coder.o: $(MAINWAY)coder.c
	gcc -c -Wall -o $@ $^

test:$(MAINWAY)test_bin.c
	gcc -Wall $^ -o $(BINWAY)test_bin

clean: $(OBJECTWAY)*.o
	rm $^