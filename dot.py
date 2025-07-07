vector1 = [0.316812, 0.65018, 0.0117172, 0.158066, 0.900709, 0.558675, 0.401449, 0.511811, 0.35205, 0.241928, 0.49172, 0.747217, 0.908112, 0.172635, 0.672775]
vector2 = [3.06445, 3.03948, 0.717217, 1.05669, 5.45635, 5.09312, 2.699, 3.09328, 2.38319, 1.64815, 2.7129, 3.17429, 4.28687, 1.05098, 3.61603]

# Check if vectors have the same length
if len(vector1) != len(vector2):
    print("Error: Vectors must have the same length")
    exit(1)

# Compute dot product
dot_product = sum(a * b for a, b in zip(vector1, vector2))

print(f"Vector 1: {vector1}")
print(f"Vector 2: {vector2}")
print(f"Dot product: {dot_product}")