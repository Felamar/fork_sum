pairs = [x * 2 for x in range(0, 100)]
odds  = [x * 2 + 1 for x in range(0, 100)]
sums  = [x + y for x, y in zip(pairs, odds)]

for i, sum in enumerate(sums):
    print(f'suma[{i}] = {sum}')