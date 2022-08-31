from collections import namedtuple
import itertools
sbox = [0xE, 0x4, 0xD, 0x1,
        0x2, 0xF, 0xB, 0x8,
        0x3, 0xA, 0x6, 0xC,
        0x5, 0x9, 0x0, 0x7]
# sbox = [14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7]
pbox = [i-1 for i in [1, 5, 9, 13,
                      2, 6, 10, 14,
                      3, 7, 11, 15,
                      4, 8, 12, 16]]
# 存放单个S盒的线性表达式
# 其中bias表示该线性表达式的偏差
# in_sum表示该线性表达式的输入[如1010(2)为A(16)]
# out_sum表示该线性表达式的输出
# expression为该线性表达式的字符串表示
WeakExpression = namedtuple(
    'WeakExpression', ['bias', 'in_sum', 'out_sum', 'expression'])


def find_sbox_weak_expression(sbox, min_bias=0.25):
    # 搜索S盒中偏差大的线性表达式
    # sbox为待检测S盒
    # min_bias表示期望的最小的偏差
    s_length = len(sbox)  # S盒的长度
    s_bit = int(s_length**0.5)  # 单个S盒的位数
    result = []
    for in_sum in range(s_length):
        for out_sum in range(s_length):
            if in_sum & (in_sum-1) and out_sum & (out_sum-1):
                # 忽略输入输出都只有一个的情况
                continue
            if in_sum == 0 or out_sum == 0:
                # 忽略输入输出为0的情况
                continue
            bias = (sum(1 - (bin(x & in_sum).count('1') + bin(sbox[x] & out_sum).count('1')) % 2
                        for x in range(s_length)) - s_length/2)/s_length
            if abs(bias) >= min_bias:
                # WeakExpression(bias=0.375, in_sum=1, out_sum=7, expression='X3+Y1+Y2+Y3')
                result.append(WeakExpression(bias, in_sum, out_sum,
                                             '+'.join(['X'+str(s_bit-i-1) for i in range(
                                                 s_bit) if in_sum & (2**(i))][::-1] + ['Y'+str(s_bit-i-1) for i in range(s_bit) if out_sum & (2**i)][::-1])))
    return result


def do_fake_sbox(input, fake_sbox):
    input_bin = [(input >> (i*4)) & 0xf for i in range(4)][::-1]
    output_bin = []
    for i in input_bin:
        if i not in fake_sbox:
            if i == 0:
                # 输入为0则输出也为0
                output_bin.append([WeakExpression(1, 0, 0, "")])
                continue
            else:
                return []
        output_bin.append(fake_sbox[i])
    result = []
    for i in itertools.product(*output_bin):
        result.append([(j.out_sum, j.bias) for j in i])
    # [print(sum(item[k][0]!=0 for k in range(4)), item) for item in result]
    result = [(sum(16**i*item[3-i][0] for i in range(4)), item[0][1]
               * item[1][1]*item[2][1]*item[3][1]*(2**(sum(item[k][0] != 0 for k in range(4))))) for item in result]
    return result


def do_p_box(input):
    input_bin = [(input >> i) & 0x1 for i in range(16)]
    # 将output使用pbox进行映射
    output = [input_bin[i] for i in pbox]
    # 将output转换为16位int
    output = sum(2**i*output[i] for i in range(16))
    return output


weak_expressions = find_sbox_weak_expression(sbox)
fake_sbox = {}  # 通过WeakExpression构造的类似S盒的东西
for weak_expression in weak_expressions:
    if weak_expression.in_sum not in fake_sbox:
        fake_sbox[weak_expression.in_sum] = []
    fake_sbox[weak_expression.in_sum].append(weak_expression)

# fake_sbox demo
# {
# 1: [WeakExpression(bias=0.375, in_sum=1, out_sum=7, expression='X3+Y1+Y2+Y3')],
# 2: [WeakExpression(bias=-0.375, in_sum=2, out_sum=14, expression='X2+Y0+Y1+Y2')],
# 4: [WeakExpression(bias=-0.25, in_sum=4, out_sum=5, expression='X1+Y1+Y3'), WeakExpression(bias=-0.25,  in_sum=4, out_sum=13, expression='X1+Y0+Y1+Y3')],
# 8: [WeakExpression(bias=-0.375, in_sum=8, out_sum=15, expression='X0+Y0+Y1+Y2+Y3')],
# 9: [WeakExpression(bias=-0.25, in_sum=9, out_sum=8, expression='X0+X3+Y0')],
# 10: [WeakExpression(bias=0.25, in_sum=10, out_sum=1, expression='X0+X2+Y3'), WeakExpression(bias=-0.25, in_sum=10, out_sum=4, expression='X0+X2+Y1')],
# 11: [WeakExpression(bias=0.25, in_sum=11, out_sum=1, expression='X0+X2+X3+Y3'), WeakExpression(bias=0.25, in_sum=11, out_sum=4, expression='X0+X2+X3+Y1')],
# 12: [WeakExpression(bias=0.25, in_sum=12, out_sum=2, expression='X0+X1+Y2')],
# 13: [WeakExpression(bias=-0.25, in_sum=13, out_sum=8, expression='X0+X1+X3+Y0')],
# 15: [WeakExpression(bias=-0.25, in_sum=15, out_sum=2, expression='X0+X1+X2+X3+Y2')]
# }

all_result = []
for start_num in range(0xffff):
    # start_num: 第一轮输入
    fake_sbox_res = do_fake_sbox(start_num, fake_sbox)
    p_res = [(do_p_box(res[0]), res[1]) for res in fake_sbox_res]
    for index1, situation_1 in enumerate(p_res):
        fake_sbox_res_2 = do_fake_sbox(p_res[index1][0], fake_sbox)
        p_res_2 = [(do_p_box(res[0]), res[1]) for res in fake_sbox_res_2]
        for index2, situation_2 in enumerate(p_res_2):
            fake_sbox_res_3 = do_fake_sbox(p_res_2[index2][0], fake_sbox)
            p_res_3 = [(do_p_box(res[0]), res[1]) for res in fake_sbox_res_3]
            for index3, situation_3 in enumerate(fake_sbox_res_3):
                total_bias = situation_1[1] * situation_2[1] * situation_3[1]
                all_result.append(
                    (start_num, fake_sbox_res[index1][0], p_res[index1][0], fake_sbox_res_2[index2][0], p_res_2[index2][0], situation_3[0], p_res_3[index3][0], total_bias))
# all_result按照bias从大到小排序
all_result = sorted(all_result, key=lambda x: abs(x[7]), reverse=True)
print(" u1  ->  v1  ->  u2  ->  v2  ->  u3  ->  v3  ->  u4  : total_bias")
for i in all_result[:100]:
    print("%04X" % i[0], "->", "%04X" % i[1], "->",
          "%04X" % i[2], "->", "%04X" % i[3], "->",
          "%04X" % i[4], "->", "%04X" % i[5], "->",
          "%04X" % i[6], ":", i[7]/2)
