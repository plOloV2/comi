#include "lib.h"

struct Data* data = NULL;
struct Thread** threads = NULL;

int main(int argc, char** argv){

    double time = omp_get_wtime();

    if(argc != 2)
        return 1;

    data = PrepareData(argv[1]);
    if(data == NULL)
        return 2;
    
    float Distance = CalculateStartDist(data);
    if(Distance == 0)
        return 3;

    threads = PrepareThreads(Distance, data->pointsNum);
    if(threads == NULL)
        return 4;

    PrintStartStats(data, Distance, time);

    time = omp_get_wtime();



    PrintEndStats(time, Distance);

    return 0;
}