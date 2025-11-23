#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_COL 3

// Assembly function prototype
extern void compute_acc(int Y, double *data, int *results);

// Reference C function for correctness check
double compute_acc_c(double vi, double vf, double t)
{
    return ((vf - vi) / 3.6) / t;
}

int main()
{
    
    int Y;
    printf("Enter number of cars (Y): ");
    scanf("%d", &Y);

    // Allocate memory (for initial manual Y)
    double (*data)[MAX_COL] = malloc(Y * sizeof(*data));
    int *results = malloc(Y * sizeof(int));
    if (!data || !results)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Read matrix (for sample/manual input)
    printf("Enter the matrix (Y rows of Vi,Vf,T without spaces, e.g., 0.0,62.5,10.1):\n");
    for (int i = 0; i < Y; i++)
    {
        scanf("%lf,%lf,%lf", &data[i][0], &data[i][1], &data[i][2]);
    }
    // Debug: Print read data to verify
    printf("Read data:\n");
    for (int i = 0; i < Y; i++)
    {
        printf("%.1f, %.1f, %.1f\n", data[i][0], data[i][1], data[i][2]);
    }

    // Compute
    compute_acc(Y, (double*) data,  results);

    // Print results
    printf("Accelerations (m/s^2):\n");
    for (int i = 0; i < Y; i++)
    {
        printf("%d\n", results[i]);
    }

    // Correctness check (for small Y)
    if (Y <= 10)
    {
        printf("\nCorrectness check:\n");
        for (int i = 0; i < Y; i++)
        {
            double acc_c = compute_acc_c(data[i][0], data[i][1], data[i][2]);
            int acc_int_c = (int)round(acc_c);
            //printf("Car %d: ASM=%d, C=%d (diff=%d)\n", i + 1, results[i], acc_int_c, abs(results[i] - acc_int_c));

            printf("Car %d: Exact=%.6f, ASM=%d C=%d (diff=%d), \n", 
                i + 1,
                acc_c,
                results[i],
                acc_int_c,
                abs(results[i] - acc_int_c)
            );
        }
    }

    // Free initial allocations (we'll reallocate for timing)
    free(data);
    free(results);

    // Timing runs (with random data)
    int test_Ys[] = {10, 100, 1000, 10000};
    int num_tests = sizeof(test_Ys) / sizeof(test_Ys[0]);
    srand(time(NULL));

    for (int t = 0; t < num_tests; t++)
    {
        int curr_Y = test_Ys[t];
        printf("Starting allocation and trials for Y=%d\n", curr_Y);
        fflush(stdout);

        // Allocate memory for current Y (to avoid buffer overflow)
        double (*data)[MAX_COL] = malloc(curr_Y * sizeof(*data));
        int *results = malloc(curr_Y * sizeof(int));
        if (!data || !results)
        {
            printf("Memory allocation failed for Y=%d\n", curr_Y);
            fflush(stdout);
            return 1;
        }
        printf("Allocation successful for Y=%d\n", curr_Y);
        fflush(stdout);

        double total_time = 0.0;

     
        for (int trial = 0; trial < 30; trial++) 
        {
            // Generate random data
            for (int i = 0; i < curr_Y; i++)
            {
                data[i][0] = rand() % 201;                          // Vi: 0-200
                data[i][1] = data[i][0] + (rand() % 101);    // Vf: Vi to Vi+100
                data[i][2] = 1 + (rand() % 20);                     // T: 1-20
            }

            clock_t start = clock();
            compute_acc(curr_Y, (double*) data, results);
            clock_t end = clock();
            total_time += (double)(end - start) / CLOCKS_PER_SEC;
        }

        // printf("Trials completed for Y=%d\n", curr_Y);
        // fflush(stdout);

      
        double avg_time = total_time / 30.0;
        printf("Y=%d: Average time = %.6f seconds\n", curr_Y, avg_time);
        fflush(stdout);

        // Free for this Y
        free(data);
        free(results);
    }

    return 0;
}

