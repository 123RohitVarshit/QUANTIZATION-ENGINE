#include <stdio.h>
#include <stdint.h>

int main(){
    printf("float=%zu bytes\n",sizeof(float));
    printf("double=%zu bytes\n",sizeof(double));
    printf("int8_t=%zu bytes\n",sizeof(int8_t));
    printf("unint8_t=%zu bytes\n",sizeof(uint8_t));
    printf("int32_t=%zu bytes\n",sizeof(int32_t));

    int8_t max_value=127;
    int8_t min_value=-128;

    printf("int8_t range: %d to %d\n",min_value,max_value);

    int n =1024;
    printf("\n%d weights as float32 requires %zu bytes\n",n,n*sizeof(float));
    printf("\n%d weights as int8 requires %zu bytes\n",n,n*sizeof(int8_t));
    return 0;
}