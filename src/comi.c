#include "lib.h"

struct Data* data = NULL;
struct Thread** threads = NULL;

int main(int argc, char** argv){

    if(argc != 2)
        return 1;

    data = PrepareData(argv[1]);
    if(data == NULL)
        return 2;
    
    double StartDistance = CalculateStartDist(data);
    if(StartDistance == 0)
        return 3;

    threads = PrepareThreads(StartDistance, data->pointsNum);
    if(threads == NULL)
        return 4;

    PrintStartStats(data, StartDistance);

    return 0;
}