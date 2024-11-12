#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include <omp.h>

#define DBL_MAX 9223372036854775807;

struct Data{                                        //holds data for all threads
    unsigned char pointsNum;
    float* dist;
};

struct Thread{                                      //data for only one thread
    float bestRoad;
    float road;
    bool* visited;
    unsigned char visitedCount;
    unsigned char now;
    int* order;
    int* bestOrder;
};

struct Results{                                     //stores end results
    float distance;
    int* order;
};

int base(int i, int a, int j);                      //calculates positon of distance in dist array

float Distance(int x1, int y1, int x2, int y2);     //calculates distance beetwen 2 points

char* FullFileName(char* FileName){                 //creats char* with path to given file

    char* result = (char *) malloc((strlen(FileName) + 9) * sizeof(char*));
    if(result == NULL)
        return NULL;

    strcpy(result, "data/");
    strcat(result, FileName);
    strcat(result, ".txt");

    return result;
}

struct Data* PrepareData(char* FileName){           //reads data from file, calculates roads lengths and creats data*

    char* FileToOpen = FullFileName(FileName);
    if(FileToOpen == NULL)
        return NULL;

    FILE* file = fopen(FileToOpen, "r");

    free(FileToOpen);
    FileToOpen = NULL;

    int* points;
    unsigned char numPoints = 0, distSize;

    if(file == NULL){
        printf("No such file.\n");
        return NULL;
    }
    
    points = (int*) malloc(40 * sizeof(int));
    if(points == NULL)
        return NULL;

    fscanf(file, "%d", &points[numPoints]);

    while( !feof(file)){

        numPoints ++;

        if(numPoints >= 40)
            return NULL;

        fscanf(file, "%d", &points[numPoints]);
    }

    fclose(file);

    numPoints = numPoints / 2 + 1;

    distSize = (numPoints*(numPoints-1))/2;

    struct Data* result = (struct Data*) malloc(sizeof(char) + sizeof(float*));
    if(result == NULL)
        return NULL;
    

    result->pointsNum = numPoints;

    result->dist = (float*) malloc(distSize * sizeof(float));
    if(result->dist == NULL)
        return NULL;
    

    for(int i = 0; i < numPoints - 1; i++)
        for(int j = i+1; j < numPoints; j++)
            result->dist[base(i, numPoints, j)] = Distance(points[i*2], points[i*2+1], points[j*2], points[j*2+1]);

    free(points);
    points = NULL;

    return result;
}

struct Results* CalculateStartDist(struct Data* data){       //uses simple algorithm to calculate some road, this is used in begining as target to beat
    float smallest = DBL_MAX;
    int best, now, HowMany;
    bool beenThere[data->pointsNum];

    struct Results* results = (struct Results*) malloc(sizeof(float) + sizeof(int*));
    if(results == NULL)
        return NULL;

    results->order = (int*) malloc(sizeof(int) * (data->pointsNum + 2));
    if(results->order == NULL)
        return NULL;

    results->order[0] = data->pointsNum;

    for(int i = 1; i < data->pointsNum; i++)
        beenThere[i] = false;

    beenThere[0] = true;
    now = 0;
    HowMany = 1;
    results->order[1] = 0;


    while(HowMany < data->pointsNum){
        for(int i = 0; i < data->pointsNum; i++){
            if(beenThere[i] == 0 && data->dist[base(now, data->pointsNum, i)] < smallest){
                smallest = data->dist[base(now, data->pointsNum, i)];
                best = i;
            }
        }

        results->distance += smallest;
        results->order[HowMany++] = best;
        beenThere[best] = true;
        now = best;
        smallest = DBL_MAX;

    }
    
    results->distance += data->dist[base(0, data->pointsNum, now)];
    results->order[data->pointsNum + 1] = 0;

    return results; 
}

struct Thread** PrepareThreads(float StartDistance, unsigned char NumOfPoints, unsigned char NumOfThreads){    //creats Thread* structure and initializes it

    struct Thread** result = (struct Thread**) malloc(sizeof(struct Thread*) * NumOfThreads);
    if(result == NULL)
        return NULL;

    for(int i = 0; i < NumOfThreads; i++){
        result[i] = (struct Thread*) malloc(sizeof(float) + sizeof(bool*) + sizeof(char)*2);
        if(result[i] == NULL)
            return NULL;

        result[i]->bestRoad = StartDistance;
        result[i]->visitedCount = 1;
        result[i]->now = 0;
        result[i]->visited = (bool*) malloc(sizeof(bool)*NumOfPoints);
        if(result[i]->visited == NULL)
            return NULL;
        
        for(int j = 1; j < NumOfPoints; j++)
            result[i]->visited[j] = false;

        result[i]->visited[0] = true;
    }

    return result;
}

void PrintStartStats(struct Data* data, struct Results* results, double time){  //prints all statictic befour starting the calculations

    printf("Prepare took %fms\nNumber of points: %d\nNumber of all roads: %d\nNot optimized distance: %f\nOrder of points: ", (omp_get_wtime()-time)*1000, data->pointsNum, (data->pointsNum*(data->pointsNum-1))/2, results->distance);
    for(int i = 1; i < results->order[0]; i++)
        printf("%d ", results->order[i]);
    
}

void PrintEndStats(double time, struct Results* result){     //prints all statictic after calculations

    int days = 0, houres = 0, minutes = 0, seconds = 0;

    time = omp_get_wtime() - time;

    while(time >= 86400){
        time -= 86400;
        days++;
    }

    while(time >= 3600){
        time -= 3600;
        houres++;
    }

    while(time >= 60){
        time -= 60;
        minutes++;
    }

    while(time >= 1){
        time -= 1;
        seconds++;
    }

    time *= 1000;   

    printf("\n\nAll took %dd, %dh, %dm, %ds, %fms\nLenghts of best raod is: %f\nOrder of points: ", days, houres, minutes, seconds, time, result->distance);
    for(int i = 1; i < result->order[0]; i++)
        printf("%d ", result->order[i]);
    

}

void CalculateBestRoad(struct Results* results, struct Thread** threads, struct Data* data, unsigned char NumOfThreads){




    return;
}

void Calc(struct Thread** threads, struct Data* data){
    short id = omp_get_thread_num();
    if(threads[id]->road >= threads[id]->bestRoad)
        return;

    if(threads[id]->visitedCount == data->pointsNum){
        threads[id]->bestRoad = threads[id]->road;
        // memcpy threads[id]->bestOrder from threads[id]->order
        return;
    }

    for(int i = 0; i < data->pointsNum; i++){
        if(threads[id]->visited[i] && (i + 2) < data->pointsNum)
            i++;

        float section;
        unsigned char prevNow;
        
        section = data->dist[base(i, data->pointsNum, threads[id]->now)];

        prevNow = threads[id]->now;

        threads[id]->now = i;
        threads[id]->order[threads[id]->visitedCount] = i;
        threads[id]->visitedCount++;
        threads[id]->road += section;
        threads[id]->visited[i] = true;
        
        Calc(threads, data);

        threads[id]->now = prevNow;
        threads[id]->visitedCount--;
        threads[id]->road -= section;
        threads[id]->visited[i] = false;        
        
    }

    return;
}
