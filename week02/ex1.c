#include <stdio.h>
#include <limits.h>
#include <float.h>

int main() {
    int integerVariable = INT_MAX;
    unsigned short int unsignedShortVariable = USHRT_MAX;
    signed long int signedLongVariable = LONG_MAX;
    float floatVariable = FLT_MAX;
    double doubleVariable = DBL_MAX;
    
    printf("Size of Integer Variable: %zu bytes\n", sizeof(integerVariable));
    printf("Size of Unsigned Short Integer Variable: %zu bytes\n", sizeof(unsignedShortVariable));
    printf("Size of Signed Long Integer Variable: %zu bytes\n", sizeof(signedLongVariable));
    printf("Size of Float Variable: %zu bytes\n", sizeof(floatVariable));
    printf("Size of Double Variable: %zu bytes\n\n", sizeof(doubleVariable));
    
    printf("Integer Variable: %d\n", integerVariable);
    printf("Unsigned Short Integer Variable: %hu\n", unsignedShortVariable);
    printf("Signed Long Integer Variable: %ld\n", signedLongVariable);
    printf("Float Variable: %f\n", floatVariable);
    printf("Double Variable: %lf\n", doubleVariable);
    
    return 0;
}
