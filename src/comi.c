#include "lib.h"

struct Data* data = NULL;

int main(int argc, char** argv){


    if(argc != 2)
        return 1;

    data = prepare_data(argv[1]);

    if(data == NULL)
        return 2;
    
    printf("%d, %f\n", data->pointsNum, data->dist[base(3, (int)data->pointsNum, 1)]);

    return 0;
}