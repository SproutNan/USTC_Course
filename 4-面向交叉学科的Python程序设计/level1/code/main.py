import csv

resource = "../resource/04-01-2022.csv"
top = 10


def modify_path_name(path):
    if len(path) and path.count("."):
        return path[:path.rfind(".")] + "_ranked" + path[path.rfind("."):]
    else:
        return ""


def main(path):
    data = {}
    # key = county name, value 是一个列表 [str: last update time, int: confirmed, int: deaths, int: recovered]
    rank = []
    # 以读方式打开文件
    with open(path, mode="r", encoding="utf-8-sig") as f:
        # 基于打开的文件，创建csv.reader实例
        reader = csv.reader(f)
        # 获取第一行的header（数组）
        header = next(reader)
        # header[3] 是国家名，header[7] 是其确诊人数

        # 逐行获取确诊人数数据，并输出
        for row in reader:
            data[row[3]] = data.get(row[3], ["", 0, 0, 0])
            # 最后更新时间
            data[row[3]][0] = row[4]
            # 计量值
            if len(row[7]):
                data[row[3]][1] += int(row[7])
            if len(row[8]):
                data[row[3]][2] += int(row[8])
            if len(row[9]):
                data[row[3]][3] += int(row[9])

    for pair in data:
        rank.append((pair, data[pair]))

    rank.sort(key=lambda y: y[1][1], reverse=True)

    # 以写方式打开文件。注意添加 newline=""，否则会在两行数据之间都插入一行空白。
    with open(modify_path_name(path), mode="w+", encoding="utf-8-sig", newline="") as w:
        # 创建列表，保存header内容
        header_list = ["Rank", "Country_Region", "Last_Update", "Confirmed", "Deaths", "Recovered"]

        # 创建列表，保存数据
        data_list = [[i+1, rank[i][0]] + rank[i][1] for i in range(top)]
        writer = csv.writer(w)

        # 写入 header。
        # writerow() 一次只能写入一行。
        writer.writerow(header_list)

        # 写入数据。
        # writerows() 一次写入多行。
        writer.writerows(data_list)

        print("Success!")


if __name__ == '__main__':
    main(resource)

