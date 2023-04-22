from asyncore import write
from email.policy import default
import analysis
import csv
import numpy
import url
from datetime import date
import translate
from time import sleep
import os
import random

def convert_date(date_have_gang:str) -> str:
    # 返回一个 mm-dd 类型的字符串，对两类数据归一
    if date_have_gang[0:4] == '2022' or date_have_gang[0:4] == '2021':
        return date_have_gang[date_have_gang.find('-')+1:]
    else:
        return date_have_gang[:date_have_gang.find(' ')]

class collector:
    id:str = None
    value_collector:url.value = None
    talkb_collector:url.talk_bar = None

    def __init__(self, id) -> None:
        try:
            self.id = id
            self.talkb_collector = url.talk_bar(id)
            self.value_collector = url.value(id)
        except:
            print("Error! Check collector __init__.")

    def get_jz(self) -> None:
        try:
            with open(f"./{self.id}_jz_{date.today()}.csv", "w+", newline='') as f:
                headers = ['date', 'rate']
                writer = csv.DictWriter(f, fieldnames=headers)
                writer.writeheader()
                for i in range(1, 200):
                    tmp = self.value_collector.get_rate(str(i))
                    if tmp is not None:
                        writer.writerow({'date': tmp[0], 'rate': tmp[1]})
        except:
            print("Error! Check get_jz.")

    def get_tk(self, page:str='1') -> None:
        try:
            self.talkb_collector.change_page(page) # 先翻到对应的页
            with open(f"./{self.id}_tk_{page}_{date.today()}.csv", "w+", newline='') as f:
                headers = ['date', 'title']
                writer = csv.DictWriter(f, fieldnames=headers)
                writer.writeheader()
                for i in range(1, 81):
                    tmp = self.talkb_collector.get_title(str(i))
                    if tmp is not None:
                        writer.writerow({'date': tmp[0], 'title': tmp[1]})
        except:
            print("Error! Check get_tk.")

class analysier:
    an = analysis.sentiment()
    tr = translate.Translate()

    def get_interested_files(self, id:str, usage:str, path='./') -> list:
        assert usage == 'rate' or usage == 'talk', "invalid parameter: usage!"
        pathlist = os.listdir(path=path)
        ret = []
        for file in pathlist:
            if file.count(id):
                if usage == 'rate' and file.count('jz'):
                    ret.append(file)
                elif usage == 'talk' and file.count('tk'):
                    ret.append(file)
        return ret

    def set_jz(self) -> numpy.ndarray:
        ret = numpy.array([])
        filename_list:list = self.get_interested_files(input('id:'), 'rate')
        for filename in filename_list:
            with open(filename, "r", newline='') as f:
                reader = csv.DictReader(f)
                for row in reader:
                    rate = row['rate']
                    ret = numpy.append(ret, float(rate[:len(rate)-1]))
            break # 只用最新的数据
        return ret

    def set_jz_simple(self) -> numpy.ndarray:
        ret = numpy.array([])
        filename_list:list = self.get_interested_files(input('id:'), 'rate')
        for filename in filename_list:
            with open(filename, "r", newline='') as f:
                reader = csv.DictReader(f)
                for row in reader:
                    rate = row['rate']
                    rate = float(rate[:len(rate)-1])
                    if abs(rate) < 0.2:
                        ret = numpy.append(ret, 0)
                    elif rate > 0:
                        ret = numpy.append(ret, 1)
                    else:
                        ret = numpy.append(ret, -1)
            break # 只用最新的数据
        return ret

    def trans_tk(self) -> None:
        # 先创建所有基金净值出现的天对应的字典
        # {std_date: np.array}，np.array中收录了这一天所有的舆论得分信息，positive为正，negative为负。
        interested_id:str = input("id:")
        filename_list:list = self.get_interested_files(interested_id, 'rate')
        interested_dict = {}
        # interested_dict.get(f'{date.today()}_sa', None)
        try:
            for filename in filename_list:
                with open(filename, 'r', newline='') as f:
                    reader = csv.DictReader(f)
                    for row in reader:
                        _date:str = convert_date(row['date'])
                        interested_dict[_date] = numpy.array([])
                break # 只用最新的数据

            # 打开所有舆论文件
            filename_list:list = self.get_interested_files(interested_id, 'talk')
            for filename in filename_list:
                with open(filename, "r", newline='') as f:
                    reader = csv.DictReader(f)
                    for row in reader:
                        print('------------------------------------------')
                        print(f'now processing {filename} row {row}')
                        std_date = convert_date(row['date'])
                        if std_date not in interested_dict:
                            print(f'{std_date} is not in dict!')
                            continue
                        std_talk = row['title'] # 中文
                        result = self.tr.t(std_talk)
                        print(f'success translated, sleep 0.1s')
                        sleep(0.1)
                        label = self.an.get_label(result)
                        score = self.an.get_score(result)
                        print(f'{label}, {score}, {result}')

                        if label is True:
                            interested_dict[std_date] = numpy.append(interested_dict[std_date], score)
                        else:
                            interested_dict[std_date] = numpy.append(interested_dict[std_date], -1*score)

                        print(std_date, interested_dict[std_date][len(interested_dict[std_date])-5:])
            with open('./log', "w+") as f:
                f.write(interested_dict.__repr__())

        except:
            with open("./log", "w+") as f:
                f.write(interested_dict.__repr__())


    


    