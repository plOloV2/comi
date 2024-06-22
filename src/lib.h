#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <omp.h>

struct Data{
    unsigned char pointsNum;
    double* dist;
};

struct Results{
    double best[8];

};

int base(int i, int a, int j);                      //calculates positon of distance in dist array

double Distance(int x1, int y1, int x2, int y2);    //calculates distance beetwen two points

char* FullFileName(char* FileName){

    char* result = (char *) malloc((strlen(FileName) + 9) * sizeof(char*));

    strcpy(result, "dane/");
    strcat(result, FileName);
    strcat(result, ".txt");

    return result;
}

struct Data *prepare_data(char* FileName){

    char* FileToOpen = FullFileName(FileName);

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

        if(numPoints >= 40){
            printf("Too many points");
            return NULL;
        }

        fscanf(file, "%d", &points[numPoints]);
    }

    fclose(file);

    numPoints = numPoints / 2 + 1;

    distSize = (numPoints*(numPoints-1))/2;

    struct Data* result = (struct Data*) malloc(sizeof(char) + sizeof(double*));

    result->pointsNum = numPoints;

    result->dist = (double*) malloc(distSize * sizeof(double));

    for(int i = 0; i < numPoints - 1; i++)
        for(int j = i+1; j < numPoints; j++)
            result->dist[base(i, numPoints, j)] = Distance(points[i*2], points[i*2+1], points[j*2], points[j*2+1]);

    return result;
}