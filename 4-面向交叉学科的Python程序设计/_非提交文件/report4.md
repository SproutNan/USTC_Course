# Level 4 实验报告

PB20111686 黄瑞轩

### Transformers

Transformers 提供了数千种预训练模型来执行文本相关任务，如分类、信息提取、问答、摘要、翻译、文本生成等。github 中提供了相应的api，可以在给定文本上快速下载和使用这些预训练模型，在自己的数据集上对它们进行微调。

#### 情感分析

关键代码如下：

```python
from transformers import pipeline
import csv

c = pipeline('sentiment-analysis')
# 股票明天下跌
print(c("Stocks will fall tomorrow."))
# 明天肯定又会出太阳
print(c("It's bound to be sunny again tomorrow."))
# 日复一日做一样的事情想到明天还要重复做肯定很崩溃
print(c("every single day It must be overwelming to think tomorrow I have to do it over again."))
```

测试结果：

<img src="C:/Users/10258/AppData/Roaming/Typora/typora-user-images/image-20220523150221965.png" alt="image-20220523150221965" style="zoom:67%;" />

测试结果基本符合事实。

### 有道翻译API（request 库）

向有道翻译的 API 发起 request 请求，实现汉译英效果，就像在 GUI 页面进行翻译业务那样。

**输入**：

<img src="C:/Users/10258/AppData/Roaming/Typora/typora-user-images/image-20220523152507596.png" alt="image-20220523152507596" style="zoom:67%;" />

**输出**：

<img src="C:/Users/10258/AppData/Roaming/Typora/typora-user-images/image-20220523152523527.png" alt="image-20220523152523527" style="zoom:67%;" />
