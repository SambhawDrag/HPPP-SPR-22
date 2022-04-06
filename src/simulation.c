#include <omp.h>
#include <stdio.h>
#include <math.h>
#include "vec3.h"
#include <stdlib.h>

void readInit(struct vec3 *output)
{
    FILE *data;
    data = fopen("Trajectory.txt", "r");
    // struct vec3 * initial_pos = (struct vec3*)malloc(1000*sizeof(struct vec3));
    int i, count = 0;
    if (data == NULL)
    {
        printf("Failed to open file: \"Trajectory.txt\"\nExiting..");
        exit(1);
    }

    printf("Reading the Initial Positions of the N bodies...\n");
    fseek(data, 249, SEEK_SET);

    i = 0;
    while (i < 1000)
    {
        fscanf(data, "%f %f %f", &output[i].x, &output[i].y, &output[i].z);
        i++;
    }
    fclose(data);
}

int main()
{
    int numParticles = 1000;
    float delT = 0.01;
    int numIters = 200; // 720000;
    float M = 1;
    float xSize = 50, ySize = 100, zSize = 300;

    struct vec3 *posN, *posN1, *velN, *velNhalf, **outLog;
    posN = (struct vec3 *)malloc(numParticles * sizeof(struct vec3));
    posN1 = (struct vec3 *)malloc(numParticles * sizeof(struct vec3));
    velN = (struct vec3 *)malloc(numParticles * sizeof(struct vec3));
    velNhalf = (struct vec3 *)malloc(numParticles * sizeof(struct vec3));
    outLog = (struct vec3 **)malloc((numIters / 100) * sizeof(struct vec3 *));
    for (int i = 0; i < numIters / 100; i++)
    {
        outLog[i] = (struct vec3 *)malloc(numParticles * sizeof(struct vec3));
    }

    readInit(posN);

    omp_set_num_threads(8);

    for(int i=0; i<10; i++)printf("%f %f %f\n", posN[i].x, posN[i].y, posN[i].z);

    double fulldt, fullt, firdt, firt, secdt, sect;

    for (int i = 0; i < numIters; i++)
    {
        if(i%10==0)printf("jjj%d\n", i);
        // fullt = omp_get_wtime();
        // firt = omp_get_wtime();
        // sect = omp_get_wtime();
        #pragma omp parallel for
        for (int j = 0; j < numParticles; j++)
        {
            struct vec3 fN;
            fN.x = 0, fN.y = 0, fN.z = 0;

            for (int k = 0; k < numParticles; k++)
            {
                if (k == j)
                    continue;

                float distSq = lenSquared(diff(posN[k], posN[j]));
                struct vec3 dir = normalize(diff(posN[k], posN[j]));
                struct vec3 force = constMul(dir, (M * M / distSq));

                fN = sum(fN, force);
            }

            struct vec3 vHalf = sum(velN[j], constMul(fN, delT / (2 * M)));
            // if(i%20 && j==50)printf("%f yolo \n", constMul(vHalf, delT).x);
            posN1[j] = sum(posN[j], constMul(vHalf, delT));
            // if(i%100==0 && j==50)printf("%f %f yolo \n", posN1[j].x, posN[j].x);
            velNhalf[j] = vHalf;
            velNhalf[j] = checkBounds(posN1[j], velNhalf[j], xSize, ySize, zSize);
        }

        // firdt = omp_get_wtime() - firt;
        // sect = omp_get_wtime();
        if(i%10==0)printf("djj%d\n", i);
        #pragma omp parallel for
        for (int j = 0; j < numParticles; j++)
        {
            struct vec3 fN;
            fN.x = 0, fN.y = 0, fN.z = 0;

            for (int k = 0; k < numParticles; k++)
            {
                if (k == j)
                    continue;

                float distSq = lenSquared(diff(posN1[k], posN1[j]));
                struct vec3 dir = normalize(diff(posN1[k], posN1[j]));
                struct vec3 force = constMul(dir, (M * M / distSq));

                fN = sum(fN, force);
            }
            struct vec3 vFull = sum(velNhalf[j], constMul(fN, delT / (2 * M)));
            velN[j] = vFull;
            posN[j] = posN1[j];

            if (i % 100 == 0)
            {
                // printf("%d %d\n", i, j);
                outLog[i / 100, j] = &posN[j];
            }
        }
        if(i%10==0)printf("djd%d\n", i);

        // secdt = omp_get_wtime() - sect;
        // fulldt = omp_get_wtime() - fullt;

        // if(i%10 == 0)
        // {
        //     printf("%lf %lf %lf\n", firdt, secdt, fulldt);
        // }
    }

    printf("\n yolo \n");
    for(int i=0; i<10; i++)printf("%f %f %f\n", outLog[1][i].x, outLog[1][i].y, outLog[1][i].z);
    FILE *trajec;
    trajec = fopen("New_Trajectory.txt", "w");

    if (trajec == NULL)
    {
        printf("Failed to open file: \"New_Trajectory.txt\"\nExiting..");
        exit(1);
    }

    for (int i = 0; i < numIters / 100; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
            fprintf(trajec, "%-8.2f", outLog[i][j].x);
        }
        fprintf(trajec, "\n");
        for (int j = 0; j < 1000; j++)
        {
            fprintf(trajec, "%-8.2f", outLog[i][j].y);
        }
        fprintf(trajec, "\n");
        for (int j = 0; j < 1000; j++)
        {
            fprintf(trajec, "%-8.2f", outLog[i][j].z);
        }
        fprintf(trajec,"\n\n");
    }
}