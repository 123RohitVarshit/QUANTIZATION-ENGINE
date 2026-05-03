#include<stdio.h>
#include<math.h>

float absmax(float *arr,int n){
    float max=0.0f;
    for (int i=0;i<n;i++){
        if (fabsf(arr[i])>max)
            max=fabsf(arr[i]);
    }
    return max;
}

float mean(float *arr,int n){
    float sum=0.0f;
    for (int i=0;i<n;i++){
        sum+=arr[i];
    }
    return sum/n;
}

int main()
{
   float weights[5] = {0.8f, -0.5f, 0.3f, -0.9f, 0.1f};
   float max_weight = absmax(weights, 5);
   float avg_weight = mean(weights, 5);
   printf("Maximum weight: %f\n", max_weight);
   printf("Average weight: %f\n", avg_weight);
    return 0;
}
