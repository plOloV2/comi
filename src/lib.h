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
    float Best;
    bool* Visited;
    unsigned char VisitedCount;
    unsigned char Now;
};

int base(int i, int a, int j);                      //calculates positon of distance in dist array

float Distance(int x1, int y1, int x2, int y2);    //calculates distance beetwen 2 points

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

    return result;
}

float CalculateStartDist(struct Data* data){       //uses simple algorithm to calculate some road, this is used in begining as target to beat
    float result = 0, smallest = DBL_MAX;
    int best, now, HowMany;
    bool beenThere[data->pointsNum];

    for(int i = 1; i < data->pointsNum; i++)
        beenThere[i] = false;

    beenThere[0] = true;
    now = 0;
    HowMany = 1;

    while(HowMany < data->pointsNum){
        for(int i = 0; i < data->pointsNum; i++){
            if(beenThere[i] == 0 && data->dist[base(now, data->pointsNum, i)] < smallest){
                smallest = data->dist[base(now, data->pointsNum, i)];
                best = i;
            }
        }

        result += smallest;
        beenThere[best] = true;
        now = best;
        HowMany++;
        smallest = DBL_MAX;

    }
    

    return result + data->dist[base(0, data->pointsNum, now)];
}

struct Thread** PrepareThreads(float StartDistance, unsigned char NumOfPoints){    //creats Thread* structure and initializes it

    struct Thread** result = (struct Thread**) malloc(sizeof(struct Thread*)*8);
    if(result == NULL)
        return NULL;

    for(int i = 0; i < 8; i++){
        result[i] = (struct Thread*) malloc(sizeof(float) + sizeof(bool*) + sizeof(char)*2);
        if(result[i] == NULL)
            return NULL;

        result[i]->Best = StartDistance;
        result[i]->VisitedCount = 1;
        result[i]->Now = 0;
        result[i]->Visited = (bool*) malloc(sizeof(bool)*NumOfPoints);
        if(result[i]->Visited == NULL)
            return NULL;
        
        for(int j = 1; j < NumOfPoints; j++)
            result[i]->Visited[j] = false;

        result[i]->Visited[0] = true;
    }

    return result;
}

void PrintStartStats(struct Data* data, float StartDistance, double time){  //prints all statictic befour starting the calculations

    printf("Prepare took %fms\nNumber of points: %d\nNumber of all roads: %d\nNot optimized distance: %f\n", (omp_get_wtime()-time)*1000, data->pointsNum, (data->pointsNum*(data->pointsNum-1))/2, StartDistance);

}

void PrintEndStats(double time, float result){     //prints all statictic after calculations

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

    printf("\n\nAll took %dd, %dh, %dm, %ds, %fms\nLenghts of best raod is: %f", days, houres, minutes, seconds, time, result);

}

void CalculateBestRoad(){

}
