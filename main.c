#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Assembly function prototype
extern void compute_acc(double *data, int Y, int *results);

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

    // Allocate memory
    double *data = malloc(Y * 3 * sizeof(double));
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
        scanf("%lf,%lf,%lf", &data[i * 3], &data[i * 3 + 1], &data[i * 3 + 2]);
    }
    // Debug: Print read data to verify
    printf("Read data:\n");
    for (int i = 0; i < Y; i++)
    {
        printf("%.1f, %.1f, %.1f\n", data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
    }

    // Compute
    compute_acc(data, Y, results);

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
            double acc_c = compute_acc_c(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
            int acc_int_c = (int)acc_c;
            printf("Car %d: ASM=%d, C=%d (diff=%d)\n", i + 1, results[i], acc_int_c, abs(results[i] - acc_int_c));
        }
    }

    // Timing runs (with random data)
    int test_Ys[] = {10, 100, 1000, 10000};
    int num_tests = sizeof(test_Ys) / sizeof(test_Ys[0]);
    srand(time(NULL));

    for (int t = 0; t < num_tests; t++)
    {
        int curr_Y = test_Ys[t];
        double total_time = 0.0;

        for (int trial = 0; trial < 30; trial++)
        {
            // Generate random data
            for (int i = 0; i < curr_Y; i++)
            {
                data[i * 3] = rand() % 201;                     // Vi: 0-200
                data[i * 3 + 1] = data[i * 3] + (rand() % 101); // Vf: Vi to Vi+100
                data[i * 3 + 2] = 1 + (rand() % 20);            // T: 1-20
            }

            clock_t start = clock();
            compute_acc(data, curr_Y, results);
            clock_t end = clock();
            total_time += (double)(end - start) / CLOCKS_PER_SEC;
        }

        double avg_time = total_time / 30.0;
        printf("Y=%d: Average time = %.6f seconds\n", curr_Y, avg_time);
    }

    free(data);
    free(results);
    return 0;
}
