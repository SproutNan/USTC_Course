from typing import Callable
from transformers import pipeline

class sentiment:
    analyser:Callable = None
    
    def __init__(self) -> None:
        self.analyser = pipeline('sentiment-analysis')

    def get_label(self, word) -> bool:
        assert self.analyser is not None, "You have not inited the analyser!"
        tmp:dict = self.analyser(word)[0]
        if tmp['label'] == 'NEGATIVE':
            return False
        return True

    def get_score(self, word) -> float:
        assert self.analyser is not None, "You have not inited the analyser!"
        tmp:dict = self.analyser(word)[0]
        return tmp['score']

    def get_dict(self, word) -> dict:
        assert self.analyser is not None, "You have not inited the analyser!"
        return self.analyser(word)[0]