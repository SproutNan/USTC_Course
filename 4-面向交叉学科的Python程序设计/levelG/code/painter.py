import random
from typing import Callable
import jieba
import csv
import os
from PIL import Image, ImageDraw, ImageFont

seg_dict = {}
seg_match = {} # 记录这个词来源于哪句话，不管性能问题了，替换策略：越长的句子优先级越高

ascii_set = ['\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\t', '\n', '\x0b', '\x0c', '\r', '\x0e', '\x0f', '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17', '\x18', '\x19', '\x1a', '\x1b', '\x1c', '\x1d', '\x1e', '\x1f', ' ', '!', '"', '#', '$', '%', '&', "'", '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?', '@', '$', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', '\x7f', '\x80', '\x81', '\x82', '\x83', '\x84', '\x85', '\x86', '\x87', '\x88', '\x89', '\x8a', '\x8b', '\x8c', '\x8d', '\x8e', '\x8f', '\x90', '\x91', '\x92', '\x93', '\x94', '\x95', '\x96', '\x97', '\x98', '\x99', '\x9a', '\x9b', '\x9c', '\x9d', '\x9e', '\x9f', '\xa0', '¡', '¢', '£', '¤', '¥', '¦', '§', '¨', '©', 'ª', '«', '¬', '\xad', '®', '¯', '°', '±', '²', '³', '´', 'µ', '¶', '·', '¸', '¹', 'º', '»', '¼', '½', '¾', '¿', 'À', 'Á', 'Â', 'Ã', 'Ä', 'Å', 'Æ', 'Ç', 'È', 'É', 'Ê', 'Ë', 'Ì', 'Í', 'Î', 'Ï', 'Ð', 'Ñ', 'Ò', 'Ó', 'Ô', 'Õ', 'Ö', '×', 'Ø', 'Ù', 'Ú', 'Û', 'Ü', 'Ý', 'Þ', 'ß', 'à', 'á', 'â', 'ã', 'ä', 'å', 'æ', 'ç', 'è', 'é', 'ê', 'ë', 'ì', 'í', 'î', 'ï', 'ð', 'ñ', 'ò', 'ó', 'ô', 'õ', 'ö', '÷', 'ø', 'ù', 'ú', 'û', 'ü', 'ý', 'þ']

def init():
    return open('stop_words.txt', "r", encoding='utf-8').read().split('\n')


def analyse_by_date(stop_list: list, dir: str, stratgy: Callable, *args):
    interested_files = os.listdir(dir)
    for file in interested_files:
    # target_date 默认是此 file 的第一个有效 date
        with open(dir+file, "r", newline='', encoding="utf-8") as f:
            reader = csv.DictReader(f)
            # is_first = True
            target_date = None
            for row in reader:
                # if is_first:
                #     target_date = row['date'][:5]
                #     is_first = False
                # if row['date'].count(target_date):
                if stratgy(*args):
                    seg_list = jieba.cut(row['title'], cut_all=True)
                    for i in seg_list:
                        if i in stop_list or len(i) < 2:
                            continue
                        if i in seg_dict:
                            seg_dict[i] += 1
                            if random.randint(0,1):
                                tag = False
                                for char in seg_match[i]:
                                    if char in ascii_set:
                                        tag = True
                                        break
                                if tag:
                                    continue
                                seg_match[i] = row['title']
                        else:
                            seg_dict[i] = 1
                            seg_match[i] = row['title']

def always_true():
    return True

def print_ret(dir: str, stratgy: Callable, *args):
    stop_list = init()
    analyse_by_date(stop_list, './test/', always_true)
    ret = sorted(seg_dict.items(), key=lambda x: x[1], reverse=True)
    rank = ret[:10]

    path = r'./bg.jpg'
    image = Image.open(path)
    draw = ImageDraw.Draw(image)
    font40 = ImageFont.truetype('./font.TTF',40)
    font30 = ImageFont.truetype('./font.TTF',30)
    font25 = ImageFont.truetype('./font.TTF',25)
    font15 = ImageFont.truetype('./font.TTF',15)

    draw.text((29,29),'这段时间，大家都在聊什么？',fill=(0,0,0),font=font40)
    draw.line([(30,90),(710,90)],fill=(215, 215, 215),width=2)
    draw.text((29,97),'看看这些热聊的关键词',fill=(50,50,50),font=font25)
    draw.line([(30,137),(710,137)],fill=(215, 215, 215),width=2)
    draw.line([(30,167),(610,167)],fill=(215, 215, 215),width=2)
    #draw.line([(30,137),(710,137)],fill=(215, 215, 215),width=2)
    for i in range(11):
        draw.line([(30+i*(610-30)//10,167),(30+i*(610-30)//10,162)],fill=(215, 215, 215),width=2)
    up = 175
    for i in range(10):
        draw.text((29,up+5),rank[i][0],fill=(0,0,0),font=font30)
        for j in range(48):
            if j < int(seg_dict[rank[i][0]]/seg_dict[rank[0][0]]*48):
                draw.rectangle([29+j*12,up+45,38+j*12,up+54],fill=250-15*i)

        tmp = int(seg_dict[rank[i][0]]/seg_dict[rank[0][0]]*100)
        draw.text((29+tmp*5.9,up+35),f'{tmp}',fill=250-15*i,font=font25)

        draw.text((29,up+65),seg_match[rank[i][0]],fill=(0,0,0),font=font15)
        for j in range(117):
            draw.line([(30+j*5,up+90),(32+j*5,up+90)],fill=(180, 180, 180),width=2)
        up += 95

    draw.text((29,up+15),'USTC@Python大作业 PB20111686 黄瑞轩 & PB20000180 刘良宇',fill=(150)*3,font=font15)
    image.show()

print_ret('./test/', always_true)