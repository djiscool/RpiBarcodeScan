# referenced from http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
CC=g++
CFLAGS= `mysql_config --libs` -std=c++11
DEPS = getBarcode.h piScanSQLFunctions.h newPiBarScan.h
OBJ = newPiBarScan.o piScanSQLFunctions.o
LIBS = `mysql_config --cflags`
%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

piBarcodeProj.x: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


clean:
	rm -f *.o *~ core *~
