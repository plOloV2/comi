#include "lib.h"

struct Data* data = NULL;
struct Thread** threads = NULL;
struct Results* results = NULL;

int main(int argc, char** argv){

    double time = omp_get_wtime();
    unsigned char NumOfThreads = omp_get_num_threads();

    if(argc != 2)
        return 1;

    data = PrepareData(argv[1]);
    if(data == NULL)
        return 2;
    
    results = CalculateStartDist(data);
    if(results == NULL)
        return 3;

    if(NumOfThreads > data->pointsNum)
        NumOfThreads = data->pointsNum;

    threads = PrepareThreads(results->distance, data->pointsNum, NumOfThreads);
    if(threads == NULL)
        return 4;

    PrintStartStats(data, results, time);

    time = omp_get_wtime();

    CalculateBestRoad(results, threads, data,  NumOfThreads);
    if(results == NULL)
        return 5;

    PrintEndStats(time, results);

    return 0;
}
