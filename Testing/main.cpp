#include <cstdio>
#include "arm_math_types.h"
extern int testmain(const char *);

extern "C" const char *patternData;


int main()
{
    int r;

    r=testmain(patternData);

    exit(r);
}
