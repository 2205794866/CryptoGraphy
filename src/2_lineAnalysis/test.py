

s = [[14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7],[14, 3, 4, 8, 1, 12, 10, 15, 7, 13, 9, 6, 11, 2, 0, 5]]


def S_func(text, mode):
    return s[mode][text & 0xf] | (s[mode][(text >> 4) & 0xf] << 4) | (s[mode][(text >> 8) & 0xf] << 8) | (s[mode][(text >> 12) & 0xf] << 12);

num = 0xffff + 1
str = "ushort s[2][65536]={{"

for i in range(0,num):
    str += "{},".format(hex(S_func(i, 0)))

str += "},{"

for i in range(0,num):
    str += "{},".format(hex(S_func(i, 1)))

str += "}};"

with open("s_table.txt", "w") as f:
    f.write(str)
    