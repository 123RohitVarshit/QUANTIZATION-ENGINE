import numpy as np

# Same concept in numpy
print(f"float32 = {np.dtype('float32').itemsize} bytes")
print(f"int8    = {np.dtype('int8').itemsize} bytes")
print(f"int32   = {np.dtype('int32').itemsize} bytes")

# Memory savings
n = 1024
print(f"\n{n} weights as float32: {n * 4} bytes")
print(f"{n} weights as int8   : {n * 1} bytes")