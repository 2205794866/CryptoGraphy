import random

str = 'ull s[2][16] = {{'

ls = list()
num_set = set()

for i in range(16):
    while True:
        num = random.randint(0, 15)
        if num not in num_set:
            num_set.add(num)
            ls.append(num)
            break
    
for i in ls:
    str += '{}, '.format(i)

str += '},'

for i in range(16):
    str += '{}, '.format(ls.index(i))

str += '};'
print(str)

