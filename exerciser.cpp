#include "exerciser.h"

void exercise(connection *C){
    query1(C, 1, 10, 20, 1, 5, 15, 1, 0, 10, 1, 0, 10, 0,0,0,0,0,0);
    query2(C,"Red");
    query3(C,"Duke");
    query4(C, "NC", "DarkBlue");
    query5(C, 5);
}
