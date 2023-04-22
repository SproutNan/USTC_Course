# Level 1 实验报告

PB20111686 黄瑞轩

### 选题：Level 1.7【统计】时序统计

COVID-19 是由 SARS-CoV-2 导致的传染性疾病，常见症状包括发烧，咳嗽和呼吸急促。现有[全球病情统计结果](https://github.com/CSSEGISandData/COVID-19/tree/master/csse_covid_19_data/csse_covid_19_daily_reports)，以数据 `04-01-2022.csv` 为例，编写一个程序，输出确诊人数（Confirmed​）最多的 10 个国家详细数据（国家 Country/Region，最新更新时间 Last Update，确认病例 Confirmed，死亡病例 Deaths，恢复病例 Recovered）

**思路**：阅读 csv 每一行，提取需要输出的数据栏，列表排序，写入 csv 文件。

**注意**：

- 阅读时同名国家需要合并数据；
- csv写入时，有一个默认的newline参数，如果需要像普通的表格那样，需要设置 `newline=""`。

**初始文件**：

![image-20220507181418388](C:/Users/10258/AppData/Roaming/Typora/typora-user-images/image-20220507181418388.png)

**获得文件**：

<img src="C:/Users/10258/AppData/Roaming/Typora/typora-user-images/image-20220507181438053.png" alt="image-20220507181438053" style="zoom: 67%;" />

**用法**：

```shell
$ python3 main.py
```

> **注**：这里引用的文件名 `"../resource/04-01-2022.csv"` 已经写死在 `main.py` 中。


