#ifndef OPERATION_SUBSCRIPTION_H
#define OPERATION_SUBSCRIPTION_H

#include "constants.h"

struct operation
{
    char date[MAX_STRING_SIZE];
    char time[MAX_STRING_SIZE];
    char type[MAX_STRING_SIZE];
    char account[MAX_STRING_SIZE];
    double sum;
    char description[MAX_STRING_SIZE];
};

#endif