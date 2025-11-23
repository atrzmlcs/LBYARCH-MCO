#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_COL 3

extern void compute_acc(int Y, double *data, int *results);

double compute_acc_c(double vi, double vf, double t)
{
    return ((vf - vi) / 3.6) / t;
}

int main()
{
    
    int Y;
    printf("Enter number of cars (Y): ");
    scanf("%d", &Y);

    double (*data)[MAX_COL] = malloc(Y * sizeof(*data));
    int *results = malloc(Y * sizeof(int));

    printf("Enter the matrix (Y rows of Vi,Vf,T without spaces, e.g., 0.0,62.5,10.1):\n");
    for (int i = 0; i < Y; i++)
    {
        scanf("%lf,%lf,%lf", &data[i][0], &data[i][1], &data[i][2]);
        if (data[i][2] == 0) {
            printf("Can not take 0 as T input, ending program...");
            return 0;
        }
    }

    // debug print
    /*
    printf("Read data:\n");
    for (int i = 0; i < Y; i++)
    {
        printf("%.1f, %.1f, %.1f\n", data[i][0], data[i][1], data[i][2]);
    }
    */

    compute_acc(Y, (double*) data,  results);

    printf("\nAcceleration Values (m/s^2):\n");
    for (int i = 0; i < Y; i++)
    {
        printf("%d\n", results[i]);
    }
    
    //Small Y to avoid crashing
    if (Y <= 10)
    {
        printf("\nCorrectness check:\n");
        for (int i = 0; i < Y; i++)
        {
            double acc_c = compute_acc_c(data[i][0], data[i][1], data[i][2]);
            int acc_int_c = (int)round(acc_c);
            //printf("Car %d: ASM=%d, C=%d (diff=%d)\n", i + 1, results[i], acc_int_c, abs(results[i] - acc_int_c));

            printf("Car %d: Exact=%f, ASM=%d C=%d (diff=%d), \n", 
                i + 1,
                acc_c,
                results[i],
                acc_int_c,
                abs(results[i] - acc_int_c)
            );
        }
    }

    free(data);
    free(results);


    printf("\nGetting Average Time Per Y (30 Trials):\n");
    int test_Ys[] = {10, 100, 1000, 10000};
    int nTests = 4;
    srand(time(NULL));

    for (int t = 0; t < nTests; t++)
    {
        int curY = test_Ys[t];
        double total_time = 0.0;

        double (*testData)[MAX_COL] = malloc(curY * sizeof(*testData));
        int *testResults = malloc(curY * sizeof(int));

        for (int trial = 0; trial < 30; trial++)
        {
            // Generate random data
            for (int i = 0; i < curY; i++)
            {
                testData[i][0] = rand() % 201;                          // Vi: 0-200
                testData[i][1] = testData[i][0] + (rand() % 101);    // Vf: Vi to Vi+100
                testData[i][2] = 1 + (rand() % 20);                     // T: 1-20
            }

            clock_t start = clock();
            compute_acc(curY, (double*) testData, testResults);
            clock_t end = clock();
            total_time += (double)(end - start) / CLOCKS_PER_SEC;
        }

        double avg_time = total_time / 30.0;
        printf("Y=%d:, Total Time: %f, Average time = %f seconds\n", curY, total_time, avg_time);

        free(testData);
        free(testResults);
    }

    
    return 0;
}
