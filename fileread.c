#include <stdio.h>

void write_weights(void) {
    // write the 5 floats to file
    FILE *f = fopen("weights.bin", "wb");  // wb = write binary
    if (f==NULL) {
        printf("Error: could not open file\n");
        return;
    }
    float data[5] = {0.8f, -0.5f, 0.3f, -0.9f, 0.1f};
    fwrite(data, sizeof(float), 5, f);
    fclose(f);
}

void read_weights(void) {
 // declare a float array of size 5
    float data[5];
    // open the file
    FILE *f = fopen("weights.bin","rb");
    if (f == NULL) {
    printf("Error: could not open file\n");
    return;
    }
    // fread into the array
    fread(data,sizeof(float),5,f);
    // close
    fclose(f);
    // print each value
    for (int i=0;i<5;i++){
        printf("weights[%d]=%f\n",i,data[i]);
    }
}

int main(void) {
    write_weights();
    read_weights();
    return 0;
}