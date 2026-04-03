# include <stdio.h>

int main()
{
    float x = 3.14f;
    float scale=0.05f;
    int q=0;

    printf("Orginal value: %f\n",x);
    printf("Scale: %f\n",scale);
    printf("Quantized value: %d\n",q);

    return 0;
}