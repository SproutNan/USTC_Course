from lxml import etree
import requests

headers =  {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.198 Safari/537.36'
}

def get_header(num:str):
    inner_headers =  {
        'ErrCode': '0', 
        'ErrMsg': 'null', 
        'Expansion': 'null', 
        'PageIndex': num, 
        'PageSize': '20', 
        'TotalCount': '1709', 
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.198 Safari/537.36'
    }
    return inner_headers


# request获得html对象
def get_html(_url):
    return requests.get(url=_url, headers=headers)

# 获得html源文字
def get_html_text(_url):
    return requests.get(url=_url, headers=headers).text

# 获得html源文字
def get_html_text_post(_url, num:str):
    return requests.post(url=_url, headers=get_header(num)).text


# 创建xpath可读取的对象
def get_xpath_etree(_html):
    if _html[0:3] == '\r\n\r\n':
        _html = _html[4:]
    return etree.HTML(_html)


# 根据xpath获取内容
def get_content_by_xpath(_etree, _xpath):
    return _etree.xpath(_xpath)

class talk_bar:
    inited = False
    ofid = ''
    page_now = ''
    url = ''
    max_page = 9999
    lazy = None # 缓存，记录当前url的html内容

    def __init__(self, id:str):
        self.ofid = id
        self.url = f'http://guba.eastmoney.com/list,of{id}.html'
        try:
            self.lazy = get_xpath_etree(get_html_text(self.url))
            # print(get_content_by_xpath(self.lazy, '//*[@id="articlelistnew"]/div[85]/span/span/span/span/text()'))
            # self.max_page = int(get_content_by_xpath(self.lazy, '//*[@id="articlelistnew"]/div[85]/span/span/span/span/text()')[0])
            self.inited = True
        except:
            print("Error! Check id.")

    def change_page(self, page:str):
        assert self.inited, "This object has not been inited yet, or something wrong happened before."
        assert int(page) < self.max_page, "You have tried changing to unexisting pages."
        if page == '1':
            try:
                self.page_now = page
                self.url = f'http://guba.eastmoney.com/list,of{self.ofid}.html'
                self.lazy = get_xpath_etree(get_html_text(self.url))
            except:
                print("Error! Check net or something else.")
                self.inited = False
        else:
            try:
                self.page_now = page
                self.url = f'http://guba.eastmoney.com/list,of{self.ofid}_{self.page_now}.html'
                self.lazy = get_xpath_etree(get_html_text(self.url))
            except:
                print("Error! Check net or something else.")
                self.inited = False
    
    def get_title(self, num:str):
        assert 1 <= int(num) and int(num) <= 82, "Invalid item number."
        try:
            time = get_content_by_xpath(self.lazy, f'//*[@id="articlelistnew"]/div[{num}]/span[5]/text()')[0]
            topic = get_content_by_xpath(self.lazy, f'//*[@id="articlelistnew"]/div[{num}]/span[3]/a/@title')[0]
            if len(topic) == 0:
                return None
            return [time, topic]
        except:
            return None

class value:
    inited = False
    ofid = ''
    url = ''
    lazy = None # 缓存，记录当前url的html内容

    def __init__(self, id:str):
        self.ofid = id
        self.url = f'https://www.dayfund.cn/fundvalue/{self.ofid}_y.html'
        try:
            self.lazy = get_xpath_etree(get_html_text(self.url))
            self.inited = True
        except:
            print("Error! Check jjjz id.")

    def get_rate(self, num:str):
        assert self.inited, "This object has not been inited yet, or something wrong happened before."
        assert 1 <= int(num) and int(num) <= 200, "Only recent 60 days' data is avalible."
        try:
            time = get_content_by_xpath(self.lazy, f'/html/body/div[3]/div[1]/table/tbody/tr[{int(num)+1}]/td[1]/text()')[0]
            rate = get_content_by_xpath(self.lazy, f'/html/body/div[3]/div[1]/table/tbody/tr[{int(num)+1}]/td[9]/text()')[0]
            if time == '' or rate == '':
                return None
            return [time, rate]
        except:
            return None





