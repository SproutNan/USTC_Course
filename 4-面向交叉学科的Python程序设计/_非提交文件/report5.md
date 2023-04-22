# Level 5 实验报告

PB20111686 黄瑞轩

### 选题：基金吧舆论情感走势与基金净值走势的相关检验

**背景**：随着支付宝、东方财富网购买基金功能的完善，越来越多的大学生加入了炒基金的行列。和传统炒股、炒基金模式不同的是，现在购买基金的 APP 往往提供“评论区”功能，用户可以发帖、回帖、分享自己的见解。Level 5 中，我将综合利用爬虫、情感分析和其他图形库技术，对东方财富网基金吧内舆论进行情感走势与基金净值走势的相关检验。

#### Step1：分析基金吧网页结构

基金吧 url：`http://guba.eastmoney.com/list,of{ID}{_{Page}}.html`，其中`{ID}`表示基金代码，如果不是第一页，则需要加上`{_{Page}}`，`{Page}`表示当前页号。

最大页号 xpath：`//*[@id="articlelistnew"]/div[82]/span/span/span/span`

标题 xpath 构成：

- 第一个帖子的 xpath：`//*[@id="articlelistnew"]/div[2]/span[3]/a/text()`

- 最后一个帖子的 xpath：`//*[@id="articlelistnew"]/div[81]/span[3]/a/text()`

很容易看出遍历帖子所需要的变量。

发帖时间 xpath：`//*[@id="articlelistnew"]/div[2]/span[5]`，这里的 2 处为帖子位置。

#### Step2：分析基金净值网页结构

爬基金净值的网页 url：`https://www.dayfund.cn/fundvalue/{ID}.html`，其中`{ID}`表示基金代码。

日期 xpath：`//*[@id="his_nav_table"]/tbody/tr[1]/td[1]`

增长率 xpath：`//*[@id="his_nav_table"]/tbody/tr[1]/td[4]`

#### Step3：封装基金吧网页、基金净值网页对象

先简单将翻译功能封装成`translate`模块，这里不赘述。

基金吧网页支持如下方法：

```python
class talk_bar:
    url = ''
    max_page = 0
    
    def __init__(self, id):
        # 按基金编码来初始化url，初始化翻到第一页
        # 获取最大页数
        
    def change_page(self, page):
        # 翻到基金吧第page页
        
    def get_title(self, num):
        # 获取第num个标题内容和时间
        # 返回值是一个二元素列表，第0个元素是时间，第1个元素是内容
```

基金净值网页支持如下方法：

```python
class value:
    url = ''
    
    def __init__(self, id):
        # 按基金编码来初始化url
        
    def change_page(self, page):
        # 翻到第page页
    
    def get_rate(self, num):
        # 获取表格第num行的内容
        # 返回值是一个二元素列表，第0个元素是时间，第1个元素是当日净值增长率
```

> 这里列出的也是**伪代码**。实际情况中因为对每一页的访问量大，并且 API 翻译需要时间，因此可以对每一页的 html 页面进行缓存，以减少`request`次数，这里没有列出具体方法，实现详情见代码。

这样，只需要调用

```python
import url

test = url.talk_bar('161725')
test2 = url.value('161725')

for i in range(1, 61):
    pr2 = test.get_title(str(i))
    pr = test2.get_rate(str(i))
    if pr2 is not None:
        print(pr2)
    # if pr is not None:
    #     print(pr)
```

就可以相当方便的给出我们需要的数据。

<img src="../report.assets/image-20220606113207519.png" alt="image-20220606113207519" style="zoom:50%;" />

<img src="../report.assets/image-20220606113223962.png" alt="image-20220606113223962" style="zoom:50%;" />

#### Step4：各种库模块封装以及数据流传送设计

数据流传送图：

![image-20220606153255862](pic/image-20220606153255862.png)

可见写入和验证程序的读取是比较固定的模式，把这个操作封装成库（与 numpy 及 matplotlib 等库结合）。

数据命名规范：

- 基金净值：`{ID}_jz_{create_time}.csv`，其中 ID 为基金代号，create_time 是一个 `yyyy-mm-dd` 形状的导出时间。
- 基金吧数据：`{ID}_tk_{page}_{create_time}`，其中 ID 为基金代号，page 是导出时此页为第几页，create_time 是一个 `yyyy-mm-dd` 形状的导出时间。

这样，用户就可以无需看到复杂的内部结构，直接初始化（这里以*招商中证白酒指数分级基金：161725*为例）

```python
from time import sleep
import mycsv

test = mycsv.collector('161725')

test.get_jz()
sleep(5)
test.get_tk('1')
```

就可以获得下面的结果：

<img src="../report.assets/image-20220606164223864.png" alt="image-20220606164223864" style="zoom:50%;" />

拿到数据，就可以来做数据检验了。

> 这里不把翻译后的数据放进来，是因为翻译的结果要消耗大量请求，极易发生错误，本人认为保存原始数据的操作越安全越好。

#### Step5：数据检验设计

> 依然以*161725.of*为例，封装一个读取数据的 csv 行为对象以及 matplotlib 画图对象，这里不展开。
>
> 由于基金的涨跌牵涉到的方面较为复杂，单从**舆论情感**一方面量化可能不完善，因此本项目先对“**涨**”、“**跌**”和“**平**”（定义为当天涨跌幅在0.2%以内）做三态的分析，以`simple`为接口标记。最后会实现一个量化的接口，但并不保证其准确性（超出了Level 5的范围）。

首先，收集感兴趣对象的基金净值数据以及基金吧舆论数据：

```python
test = mycsv.collector('161725')

stage = 'get data'

# 获取数据
if stage == 'get data':
    for i in range(1, 400):
        test.get_tk(str(i*5))
        sleep(random.randint(10,20))
        print(f"Round {i} success, start next trying...")
        
    test.get_jz()
```

然后，根据舆论数据按日期降序排列的特点，将所有的内容翻译，交给 Transformers 判断情感，结果存在 `./log` 文件中。

```python
test2 = mycsv.analysier()

stage = 'have got data'

if stage == 'have got data':
    test2.trans_tk()
```

<img src="../report.assets/image-20220607115907404.png" alt="image-20220607115907404" style="zoom:50%;" />

之后，根据返回的 dict 字典，计算各情感值得分的平均。对结果进行正负及当天游程检验，得结果

<img src="../report.assets/image-20220607162637203.png" alt="image-20220607162637203" style="zoom: 67%;" />

**结论**：基金吧舆论对基金涨跌有影响，但是这种影响是正向的，即舆论倾向受净值的影响比净值受舆论的影响更显著。

> **level 5 项目文件（code 文件夹中）**
>
> - analysis.py （情感分析封装）
>
> - log （分析结果）
>
> - main.py （主函数）
>
> - mycsv.py （读写 csv 操作封装）
>
> - translate.py （翻译功能封装）
>
> - url.py （爬虫模块封装）
>
> **注**：按照所述步骤，将 `161725` 换成其他的基金代码就可以检验其他基金数据。所获得的
>
> *Total : 5 files, 358 codes, 38 comments, 84 blanks, all 480 lines*

