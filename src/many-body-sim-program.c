#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>



int main()
{
    FILE *data;
    data = fopen("Trajectory.txt", "r");
    float initial_pos[1000][3];
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
        fscanf(data, "%f %f %f", &initial_pos[i][0], &initial_pos[i][1], &initial_pos[i][2]);
        i++;
    }

    printf("Initial Positions of the N Bodies:\n");
    for (i = 0; i < 1000; i++)
    {
        printf("%-8.2f %-8.2f %-8.2f\n", initial_pos[i][0], initial_pos[i][1], initial_pos[i][2]);
    }
    fclose(data);
    return 0;
}