#ifndef VERTICESARRAYS_H_INCLUDED
#define VERTICESARRAYS_H_INCLUDED

#define P_SIZE 3
#define C_SIZE 1
#define N_SIZE 3

#define BUFFER_OFFSET(a) ((char*)NULL + (a))

struct VA
{
    float* verticesA;
    int* costsA;
    float* normalsA;
    unsigned int* indexA;
};

#endif // VERTICESARRAYS_H_INCLUDED
