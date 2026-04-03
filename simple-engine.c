# include<stdio.h>
# include<stdint.h>
# include<math.h>

int8_t scalar_quantize(float x, float scale) // Do parameters leta hai as input i.e. x and scale
{
    float q_float = roundf(x/scale); // x ko scale se divide karke round karte hai

    if (q_float > 127.0f)q_float =127.0f; // Agar q_float 127 se bada hai to usko 127 set karte hai
    if (q_float < -128.0f)q_float =-128.0f; // Agar q_float -128 se chota hai to usko -128 set karte hai

    return (int8_t)q_float; // q_float ko int8_t me convert karke return karte hai

}

float scalar_dequantize(int8_t q, float scale) // Do parameters leta hai as input i.e. q and scale
{
    return (float)q*scale; // q ko float me convert karke scale se multiply karke return karte hai
}

int main() // Main function
{
    float scale = 0.05f;

    float test[] = {1.2f,-0.5f,3.1f,2.3f,-1.0f,9.3f};
    int n =6;

    printf("%-10s %-10s %-12s %-10s\n", "original","quantized","dequantized","error");
    printf("-------------------------------------------------------------\n");

    for (int i=0;i<n;i++)
    {
        int8_t q = scalar_quantize(test[i],scale); // test[i] ko scale ke sath quantize karte hai
        float back =scalar_dequantize(q,scale);// q ko scale ke sath dequantize karte hai
        float error = test[i] - back; // original value (test[i]) aur dequantized value (back) ke beech ka error calculate karte hai
        printf("%-10.2f %-10d %-12.2f %-10.2f\n", test[i], q, back, error);
    }
    return 0;
}