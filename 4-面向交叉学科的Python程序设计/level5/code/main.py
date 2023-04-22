from time import sleep
import time
import numpy
from numpy import *
import mycsv
import matplotlib.pyplot as plt
import random

def convert(date:str) -> int:
    # 把yy-dd型转化成第多少天
    timeA = time.strptime(date, "%m-%d")
    return timeA.tm_yday
# test = mycsv.collector('161725')
test2 = mycsv.analysier()

# stage = 'get data'
# stage = 'have got data'
stage = 'caculate aver'

# # 获取数据
# if stage == 'get data':
#     for i in range(1, 400):
#         test.get_tk(str(i*5))
#         sleep(random.randint(10,20))
#         print(f"Round {i} success, start next trying...")

# if stage == 'have got data':
#     test2.trans_tk()

if stage == 'caculate aver':
    with open("./log", 'r') as f:
        content = f.read()
        result:dict = eval(content)
        for day in result:
            result[day] = numpy.average(result[day])

        real_dic = {}
        for day in result:
            real_dic[convert(day)] = result[day]

        real_list = []
        for i in range(365):
            if i in real_dic:
                if real_dic[i] ** 2 < 1:
                    real_list.append(real_dic[i])
                else:
                    real_list.append(-1)
            else:
                pass

        npav = array(real_list)
        print(npav)
        aver = mean(npav)
        print(aver)

        real_result = []
        for i in real_list:
            if i > aver:
                real_result.append(1)
            elif i == -1:
                real_result.append(0)
            else:
                real_result.append(-1)


        npa = array(real_result)
        npb = test2.set_jz_simple()
        #plt.plot(npb, 'o')
        #plt.plot(npa, 'o')
        #plt.show()

        testnum = 0
        total = None
        if npa.size > npb.size:
            total = npa.size
        else:
            total = npb.size
        
        for i in range(npa.size+npb.size-total):
            if not (npa[i] * npb[i] == -1):
                testnum += 1

        print('当天的符合度是', testnum/total)

        testnum = 0
        for i in range(npa.size+npb.size-total-1):
            if not (npa[i] * npb[i+1] == -1):
                testnum += 1

        print('负游程检验的符合度是', testnum/total)

        testnum = 0
        for i in range(npa.size+npb.size-total-1):
            if not (npa[i+1] * npb[i] == -1):
                testnum += 1

        print('正游程检验的符合度是', testnum/total)

        # while True:
        #     pass

        # print(real_list)



