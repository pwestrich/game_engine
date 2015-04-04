
#ifndef UTILITIES_H
#define UTILITIES_H

#include <cstdlib>
#include <string>
#include <sstream>

using namespace std;

void parseFloats(string str, float *values);
void parseInts(string str, int *values);

char *itoa(int value);
char *ftoi(float value);
char *dtoa(double value);

#endif
