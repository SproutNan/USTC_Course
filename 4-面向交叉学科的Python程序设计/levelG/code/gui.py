import sys
import numpy as np
import jieba
import imgkit
import matplotlib.pyplot as plt

from PyQt5 import QtWidgets, QtGui
from url import all_fund
from mycsv import collector, analysier
from mpl_canvas import MplCanvas
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
from collections import Counter
from pyecharts.charts import WordCloud
from analysis import forecast

class Window(QtWidgets.QTabWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("基金情绪分析与预测")
        self.resize(500, 600)
        self.query_page = QtWidgets.QWidget()
        self.trend_page = QtWidgets.QWidget()
        self.post_page = QtWidgets.QWidget()
        self.setup_data()
        self.setup_query_page()
        self.setup_trend_page()
        self.setup_post_page()
        self.setup_tab_widget()

    def setup_data(self):
        self.all_fund = all_fund()

    # 查询基金对应的代码
    def setup_query_page(self):
        label = QtWidgets.QLabel(self.query_page)
        label.setText("名称：")
        label.move(20, 20)
        fund_name = QtWidgets.QLineEdit(self.query_page)
        fund_name.move(100, 20)
        btn = QtWidgets.QPushButton("查询", self.query_page)
        btn.move(250, 20)
        result_show = QtWidgets.QPlainTextEdit(self.query_page)
        result_show.resize(500, 520)
        result_show.move(0, 80)

        def search_id():
            name = fund_name.text()
            search_result = self.all_fund.query(name)
            result_text = '\n'.join(
                [f'{result[0]}     {result[1]}' for result in search_result])
            result_show.setPlainText(result_text)

        btn.clicked.connect(search_id)

    # 基金趋势绘图
    def setup_trend_page(self):
        label = QtWidgets.QLabel(self.trend_page)
        label.setText("编号：")
        label.move(20, 20)
        fund_code = QtWidgets.QLineEdit(self.trend_page)
        fund_code.move(100, 20)
        btn = QtWidgets.QPushButton("获取", self.trend_page)
        btn.move(250, 20)

        # 绘图
        sc = MplCanvas(self.trend_page, width=5, height=4, dpi=100)
        toolbar = NavigationToolbar(sc, self.trend_page)
        layout = QtWidgets.QVBoxLayout()
        layout.addWidget(toolbar)
        layout.addWidget(sc)
        widget = QtWidgets.QWidget(self.trend_page)
        widget.setLayout(layout)
        widget.move(0, 100)

        # 更新趋势为当前指定的 id
        def get_trend():
            code = fund_code.text()
            message_collector = collector(code)
            # 获取最新文件
            message_collector.get_jz()
            # 绘制到界面上
            data_analyser = analysier(code)
            y = data_analyser.set_data()
            time = len(y)
            x = np.arange(time)
            sc.axes.cla()
            sc.axes.plot(x, y, label='raw')
            predict = np.array([])
            for _ in range(30):
                predict_value = forecast.forecast(y[-1], y[-30:], True)
                y = np.append(y, predict_value)
                predict = np.append(predict, y[-1])
            sc.axes.plot(np.arange(time, time + 30), y[-30:], label='predict')
            sc.draw()

        btn.clicked.connect(get_trend)

    # 爬取帖子
    def setup_post_page(self):
        label = QtWidgets.QLabel(self.post_page)
        label.setText("编号：")
        label.move(20, 20)
        fund_code = QtWidgets.QLineEdit(self.post_page)
        fund_code.move(100, 20)

        label1 = QtWidgets.QLabel(self.post_page)
        label1.setText("面数：")
        label1.move(20, 70)
        page_max = QtWidgets.QLineEdit(self.post_page)
        page_max.move(100, 70)

        btn = QtWidgets.QPushButton("查找", self.post_page)
        btn.move(250, 70)

        # 走势图
        sc = MplCanvas(self.post_page, width=5, height=4, dpi=100)
        toolbar = NavigationToolbar(sc, self.post_page)
        layout = QtWidgets.QVBoxLayout()
        layout.addWidget(toolbar)
        layout.addWidget(sc)
        widget = QtWidgets.QWidget(self.post_page)
        widget.setLayout(layout)
        widget.move(350, 20)

        # 词云
        with open('cloud.html') as f:
            imgkit.from_file(f, 'cloud.jpg')
        cloud = QtWidgets.QLabel(self.post_page)
        cloud.move(350, 450)
        cloud.resize(600, 300)
        cloud.setPixmap(QtGui.QPixmap("cloud.jpg"))
        # 设置内容缩放
        cloud.setScaledContents(True)

        # 饼图
        pie = QtWidgets.QLabel(self.post_page)
        pie.move(0, 450)
        pie.resize(400, 300)
        pie.setPixmap(QtGui.QPixmap("pie.jpg"))
        # 设置内容缩放
        pie.setScaledContents(True)

        # 评论
        result_show = QtWidgets.QPlainTextEdit(self.post_page)
        result_show.resize(300, 300)
        result_show.move(50, 150)

        def get_post():
            code = fund_code.text()
            page = page_max.text()
            post_collecter = collector(code)
            # 循环获取指定页数的信息
            for i in range(1, int(page) + 1):
                post_collecter.get_tk(str(i * 5))
            # 进行翻译
            data_analyser = analysier(code)
            # 日期 映射到 [(标题，情感), ...]
            posts = data_analyser.trans_tk()
            # 需要转换为：走势图，评论滚动，饼图，词云
            # 评论显示
            def color(score): return "🟥" if score > 0 else "🟩"
            text = '\n'.join(['\n'.join(
                [f"{date} {color(post[1])} {post[0]}" for post in posts]) for date, posts in posts.items() if date])
            result_show.setPlainText(text)

            # 走势图
            avers = list(reversed(
                [sum([post[1] for post in day]) / len(day) for day in posts.values() if day]))
            x = np.arange(len(avers))
            sc.axes.cla()
            sc.axes.plot(x, avers)
            sc.draw()

            # 词云
            post_all: str = ' '.join(
                [' '.join([post[0] for post in day]) for day in posts.values()])
            # 中文分词
            seg_list = jieba.lcut(post_all)
            # 提取停用词
            stop_word = []
            f = open('stop_words.txt', "r", encoding='utf-8')
            stop_word += f.read().split()
            f.close()
            # 去除单个字和停用词
            seg_list = [tok for tok in seg_list if len(
                tok.lower()) > 1 and (tok.lower() not in stop_word)]
            # 统计词频
            seg = list(Counter(seg_list).items())
            # 绘制图表
            wd = WordCloud()
            wd.add('cloud', seg)
            # 生成图表
            wd.render('cloud.html')
            with open('cloud.html') as f:
                imgkit.from_file(f, 'cloud.jpg')
            cloud.setPixmap(QtGui.QPixmap("cloud.jpg"))

            # 饼图
            positive = sum([sum([1 for post in day if post[1] > 0])
                           for day in posts.values() if day])
            negative = sum([sum([1 for post in day if post[1] < 0])
                           for day in posts.values() if day])
            y = np.array([positive, negative])
            plt.pie(y,
                    labels=['Positive', 'Negative'],  # 设置饼图标签
                    colors=["#d5695d", "#5d8ca8"],  # 设置饼图颜色
                    )
            plt.title("sentiment")  # 设置标题
            plt.savefig('pie.jpg')
            pie.setPixmap(QtGui.QPixmap("pie.jpg"))

        btn.clicked.connect(get_post)

    def setup_tab_widget(self):
        self.addTab(self.query_page, "代码查询")
        self.addTab(self.trend_page, "基金走势")
        self.addTab(self.post_page, "讨论帖")
        self.setTabsClosable(False)

        # 标签页
        self.setUsesScrollButtons(False)


if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    window = Window()
    window.show()
    sys.exit(app.exec_())
