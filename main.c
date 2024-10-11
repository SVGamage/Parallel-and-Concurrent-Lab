#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>

#include "linked_list.h"

void calculate_statistics(double *times, int num_runs, double *mean, double *std_dev)
{
    double sum = 0.0;
    double sum_of_squares = 0.0;

    for (int i = 0; i < num_runs; i++)
    {
        sum += times[i];
    }
    *mean = sum / num_runs;

    for (int i = 0; i < num_runs; i++)
    {
        sum_of_squares += pow(times[i] - *mean, 2);
    }
    *std_dev = sqrt(sum_of_squares / num_runs);
}

int calculate_sample_size(double mean, double std_dev, double accuracy, double z)
{
    return (int)ceil(pow((100 * z * std_dev) / (accuracy * mean), 2));
}

int runTestCases(const char *method, double mMember, double mInsert, double mDelete, int numberOfThreads, double *mean, double *std_dev)
{
    const int initial_runs = 100;
    double times[initial_runs];
    int sample_size;

    for (int i = 0; i < initial_runs; i++)
    {
        if (strcmp(method, "serial") == 0)
        {
            times[i] = performSerialOperationOnLinkedList(mMember, mInsert, mDelete);
        }
        else if (strcmp(method, "mutex") == 0)
        {
            times[i] = performMutexOperationOnLinkedList(mMember, mInsert, mDelete, numberOfThreads);
        }
        else if (strcmp(method, "rwlock") == 0)
        {
            times[i] = performLockOperationOnLinkedList(mMember, mInsert, mDelete, numberOfThreads);
        }
        else
        {
            fprintf(stderr, "Error: Invalid method type specified.\n");
            return -1;
        }
    }

    calculate_statistics(times, initial_runs, mean, std_dev);

    double accuracy = 5.0;
    double z = 1.96;
    sample_size = calculate_sample_size(*mean, *std_dev, accuracy, z);

    double *new_times = (double *)malloc(sample_size * sizeof(double));

    for (int i = 0; i < sample_size; i++)
    {
        if (strcmp(method, "serial") == 0)
        {
            new_times[i] = performSerialOperationOnLinkedList(mMember, mInsert, mDelete);
        }
        else if (strcmp(method, "mutex") == 0)
        {
            new_times[i] = performMutexOperationOnLinkedList(mMember, mInsert, mDelete, numberOfThreads);
        }
        else if (strcmp(method, "rwlock") == 0)
        {
            new_times[i] = performLockOperationOnLinkedList(mMember, mInsert, mDelete, numberOfThreads);
        }
    }

    calculate_statistics(new_times, sample_size, mean, std_dev);
    free(new_times);

    return sample_size;
}

void writeExperimentResults(const char *method, int numberOfThreads, double mMember, double mInsert, double mDelete, int sample_size, double mean, double std_dev)
{
    printf("method:%s Threads:%d mMember:%f mInsert:%f mDelete:%f sampleSize:%d mean:%f std_dev:%f\n", method, numberOfThreads, mMember, mInsert, mDelete, sample_size, mean, std_dev);
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("Usage: ./main [mutex|rwlock|serial] [mMember] [mInsert] [mDelete]\n");
        return 1;
    }

    char *mode = argv[1];
    double mMember = atof(argv[2]);
    double mInsert = atof(argv[3]);
    double mDelete = atof(argv[4]);

    for (int i = 0; i < 4; i++)
    {
        int numberOfThreads = (int)pow(2, i);
        double mean, std_dev;
        int sample_size;
        if (strcmp(mode, "serial") == 0 && numberOfThreads == 1)
        {
            sample_size = runTestCases(mode, mMember, mInsert, mDelete, numberOfThreads, &mean, &std_dev);
            writeExperimentResults(mode, numberOfThreads, mMember, mInsert, mDelete, sample_size, mean, std_dev);
        }

        if (strcmp(mode, "mutex") == 0)
        {
            mean = 0.0;
            std_dev = 0.0;
            sample_size = runTestCases(mode, mMember, mInsert, mDelete, numberOfThreads, &mean, &std_dev);
            writeExperimentResults(mode, numberOfThreads, mMember, mInsert, mDelete, sample_size, mean, std_dev);
        }

        if (strcmp(mode, "rwlock") == 0)
        {
            mean = 0.0;
            std_dev = 0.0;
            sample_size = runTestCases(mode, mMember, mInsert, mDelete, numberOfThreads, &mean, &std_dev);
            writeExperimentResults(mode, numberOfThreads, mMember, mInsert, mDelete, sample_size, mean, std_dev);
        }
    }

    return 0;
}
