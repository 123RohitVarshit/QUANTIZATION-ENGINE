import numpy as np

def quantize_scalar(x,scale):
    q_float =round(x/scale)

    if q_float >127.0:
        q_float =127.0
    if q_float <-128.0:
        q_float =-128.0
    return np.int8(q_float)

def dequantize_scalar(q,scale):
    return float(q)*scale
if __name__ == "__main__":
    scale = 0.05
    test = np.array([-1.20, -0.50, 3.10, 2.30, -1.00, 9.30], dtype=np.float32)
    print(f"{'Original':<10} {'Quantized':<10} {'Dequantized':<12} {'Error':<10}")
    for x in test:
        q = quantize_scalar(x,scale)
        back = dequantize_scalar(q,scale)
        error = x - back
        print(f"{x:<10.2f} {q:<10d} {back:<12.2f} {error:<10.2f}")