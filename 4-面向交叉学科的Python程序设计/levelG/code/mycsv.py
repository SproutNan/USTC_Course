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
    def __init__(self, id: str) -> None:
        try:
            self.id = id
            self.talkb_collector = url.talk_bar(id)
            self.value_collector = url.value(id)
        except:
            print("Error! Check collector __init__.")

    def get_jz(self) -> None:
        try:
            with open(f"./{self.id}_jz_{date.today()}.csv", "w+", newline='') as f:
                headers = ['date', 'total', 'rate']
                writer = csv.DictWriter(f, fieldnames=headers)
                writer.writeheader()
                for i in range(1, 200):
                    data = self.value_collector.get_rate(str(i))
                    if data is not None:
                        time, total, rate = data
                        writer.writerow({'date': time, 'total': total, 'rate': rate})
        except:
            print("Error! Check get_jz.")

    def get_tk(self, page: str='1') -> None:
        try:
            # 先翻到对应的页
            self.talkb_collector.change_page(page)
            # 存储到对应文件
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

    def __init__(self, id: str):
        self.id = id

    def get_interested_files(self, usage: str, path='./') -> list[str]:
        assert usage == 'rate' or usage == 'talk', "invalid parameter: usage!"
        keyword = {'rate': 'jz', 'talk': 'tk'}[usage]
        pathlist = [file for file in os.listdir(path=path) if file.count(self.id)]
        return [file for file in pathlist if file.count(keyword)]

    def set_data(self) -> numpy.ndarray:
        ret = numpy.array([])
        # 取第一个文件，即最新的文件
        file = self.get_interested_files('rate')[0]
        with open(file, 'r', newline='') as f:
            reader = csv.DictReader(f)
            for row in reader:
                total = row['total']
                ret = numpy.append(ret, float(total))
        return ret

    def set_jz(self) -> numpy.ndarray:
        ret = numpy.array([])
        # 取第一个文件，即最新的文件
        file = self.get_interested_files('rate')[0]
        with open(file, "r", newline='') as f:
            reader = csv.DictReader(f)
            for row in reader:
                rate = row['rate']
                ret = numpy.append(ret, float(rate[:len(rate)-1]))
        return ret

    def set_jz_simple(self) -> numpy.ndarray:
        ret = numpy.array([])
        filename_list:list = self.get_interested_files('rate')
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

    # 日期 映射到 [(标题，情感), ...]
    def trans_tk(self) -> dict[str, list[tuple[str, float]]]:
        # 先创建所有基金净值出现的天对应的字典
        # {std_date: np.array}，np.array中收录了这一天所有的舆论得分信息，positive为正，negative为负。
        filename_list: list = self.get_interested_files('rate')
        interested_dict: dict[str, list[tuple[str, float]]] = {}
        # interested_dict.get(f'{date.today()}_sa', None)
        try:
            for filename in filename_list:
                with open(filename, 'r', newline='') as f:
                    reader = csv.DictReader(f)
                    for row in reader:
                        _date: str = convert_date(row['date'])
                        interested_dict[_date] = []
                break # 只用最新的数据

            # 打开所有舆论文件
            filename_list: list = self.get_interested_files('talk')
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
                        # result = self.tr.t(std_talk)
                        print(f'success translated, sleep 0.1s')
                        # sleep(0.1)
                        # label = self.an.get_label(result)
                        # score = self.an.get_score(result)
                        # print(f'{label}, {score}, {result}')

                        interested_dict[std_date].append(std_talk)
                        # if label is True:
                            # interested_dict[std_date].append((std_talk, score))
                        # else:
                            # interested_dict[std_date].append((std_talk, -1 * score))

            with open('./log', "w+") as f:
                f.write(interested_dict.__repr__())
            return interested_dict

        except:
            with open("./log", "w+") as f:
                f.write(interested_dict.__repr__())
            return interested_dict


    


    