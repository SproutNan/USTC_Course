from transformers import pipeline
import csv

c = pipeline('sentiment-analysis')
print(c("Stocks will fall tomorrow."))
print(c("It's bound to be sunny again tomorrow."))
print(c("every single day It must be overwelming to think tomorrow I have to do it over again."))


import json
import requests


def translate(word):
    def get_response(word):
        # 有道词典 api
        url = 'http://fanyi.youdao.com/translate?smartresult=dict&smartresult=rule&smartresult=ugc&sessionFrom=null'
        # 传输的参数，其中 i 为需要翻译的内容
        key = {
            'type': "AUTO",
            'i': word,
            "doctype": "json",
            "version": "2.1",
            "keyfrom": "fanyi.web",
            "ue": "UTF-8",
            "action": "FY_BY_CLICKBUTTON",
            "typoResult": "true"
        }
        # key 这个字典为发送给有道词典服务器的内容
        response = requests.post(url, data=key)
        # 判断服务器是否相应成功
        if response.status_code == 200:
            # 然后相应的结果
            return response.text
        else:
            print("有道词典调用失败")
            # 相应失败就返回空
            return None

    response = get_response(word)
    if response:
        result = json.loads(response)
        return result['translateResult'][0][0]['src'], result['translateResult'][0][0]['tgt']
    else:
        return word, None


if __name__ == '__main__':
    src, tgt = translate('股票明天将会下跌')
    print(src) # 输入
    print(tgt) # 结果


