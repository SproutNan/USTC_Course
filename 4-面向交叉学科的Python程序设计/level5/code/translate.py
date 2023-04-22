import requests
import sys
import uuid
import hashlib
import time
from imp import reload

reload(sys)

YOUDAO_URL = 'https://openapi.youdao.com/api'
APP_KEY = '35d0e888f39ad756'
APP_SECRET = 'KQ535tyllCbRsmwugFQSGiBIFzq5AMwp'

class Translate:
    def __init__(self) -> None:
        pass

    def encrypt(self, signStr):
        hash_algorithm = hashlib.sha256()
        hash_algorithm.update(signStr.encode('utf-8'))
        return hash_algorithm.hexdigest()


    def truncate(self, q):
        if q is None:
            return None
        size = len(q)
        return q if size <= 20 else q[0:10] + str(size) + q[size - 10:size]


    def do_request(self, data):
        headers = {'Content-Type': 'application/x-www-form-urlencoded'}
        return requests.post(YOUDAO_URL, data=data, headers=headers)


    def connect(self, word):
        data = {}
        data['from'] = 'zh-CHS'
        data['to'] = 'en'
        data['signType'] = 'v3'
        curtime = str(int(time.time()))
        data['curtime'] = curtime
        salt = str(uuid.uuid1())
        signStr = APP_KEY + self.truncate(word) + salt + curtime + APP_SECRET
        sign = self.encrypt(signStr)
        data['appKey'] = APP_KEY
        data['q'] = word
        data['salt'] = salt
        data['sign'] = sign
        # data['vocabId'] = "您的用户词表ID"

        response = self.do_request(data)
        contentType = response.headers['Content-Type']
        # if contentType == "audio/mp3":
        #     millis = int(round(time.time() * 1000))
        #     filePath = "合成的音频存储路径" + str(millis) + ".mp3"
        #     fo = open(filePath, 'wb')
        #     fo.write(response.content)
        #     fo.close()
        # else:
        ans = response.json()
        return ans["translation"][0]

    def t(self, word):
        return self.connect(word)