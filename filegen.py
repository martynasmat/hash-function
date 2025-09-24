import string
import random

# Test files with one character
for i in range(0, 10):
    with open(f".\\test_files\\test_1char_{i+1}.txt", "x") as f:
        f.write(random.choice(string.ascii_letters + string.digits))

# Test files with 1000 characters
for i in range(0, 10):
    with open(f".\\test_files\\test_1000chars_{i+1}.txt", "x") as f:
        for _ in range(0, 1000):
            f.write(random.choice(string.ascii_letters + string.digits))


# Test files with 1000 characters with 1 character changed
data = []
for _ in range(0, 1000):
    data.append(random.choice(string.ascii_letters + string.digits))

pos = random.randint(0, 999)

for i in range(0, 10):
    with open(f".\\test_files\\test_1000chars_chardiff_{i+1}.txt", "x") as f:
        data[pos] = random.choice(string.ascii_letters + string.digits)
        f.write("".join(data))

# Collision test file
lengths = [10, 100, 500, 1000]
for i in lengths:
    with open(f".\\test_files\\collision{i}.txt", "x") as f:
        for _ in range(0, 100000):
            data_1 = [random.choice(string.ascii_letters + string.digits) for _ in range(0, i)]
            data_2 = [random.choice(string.ascii_letters + string.digits) for _ in range(0, i)]
            f.write("".join(data_1) + ' ' + "".join(data_2) + '\n')

# Avalanche effect test file
with open(f".\\test_files\\avalanche.txt", "x") as f:
    for _ in range(0, 100000):
        data_1 = [random.choice(string.ascii_letters + string.digits) for _ in range(0, 25)]
        data_2 = data_1.copy()
        pos = random.randint(0, len(data_1) - 1)
        while data_2[pos] == data_1[pos]:
            rand_char = random.choice(string.ascii_letters + string.digits)
            data_2[pos] = rand_char
        f.write("".join(data_1) + ' ' + "".join(data_2) + '\n')
