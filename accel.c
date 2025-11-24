double compute_acc_c(double vi, double vf, double t)
{
    return ((vf - vi) / 3.6) / t;
}

double compute_acc_c_full(int Y, double data[][3], int results[])
{
    int i;
    for (i = 0; i < Y; i++) {
        //results[i] = compute_acc_c(data[i][], data[i][1], data[i][2]);
        double vi   = data[i][0];
        double vf   = data[i][1];
        double t    = data[i][2];

        results[i] = round( (vf - vi) / 3.6 / t );
    }
}