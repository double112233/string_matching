import random
import string

def generate_random_string(length):
    letters = string.ascii_letters
    return ''.join(random.choice(letters) for _ in range(length))

# 讀取使用者輸入的兩個數字 N 和 M
N, M = input().split()

# 生成長度為 N 和 M 的隨機字串
random_string_N = generate_random_string(int(N))
random_string_M = generate_random_string(int(M))

# 輸出隨機字串
print(random_string_N)
print(random_string_M)
