# %% [markdown]
# # 处理数据
# 
# 参考了 https://blog.csdn.net/raelum/article/details/127568409 的代码

# %%
import os
import torch

from torchtext.data import get_tokenizer
from torchtext.vocab import build_vocab_from_iterator
from torchtext import transforms as T
from torch.utils.data import TensorDataset

class DataReader:
    def __init__(self, path='./aclImdb', is_train=True):
        """
        从给定的路径中读取训练集或测试集，并返回分词后的影评及其对应的标签
        返回的 reviews 是一个二维列表，即 reviews[0] 代表一段（分词了的）影评，reviews[0][0] 代表这段影评中的一个词元
        返回的 labels 是一个一维列表，其中的每个元素非 0 即 1【0 代表是消极影评（负样本），1 代表是积极影评（正样本）】
        """
        self.reviews, self.labels = [], []
        tokenizer = get_tokenizer('basic_english')
        for label in ['pos', 'neg']:
            folder_name = os.path.join(path, 'train' if is_train else 'test', label)
            for filename in os.listdir(folder_name):
                with open(os.path.join(folder_name, filename), mode='r', encoding='utf-8') as f:
                    self.reviews.append(tokenizer(f.read()))
                    self.labels.append(1 if label == 'pos' else 0)
        
    def build_dataset(self, vocab, max_len=512):
        text_transform = T.Sequential(
            T.VocabTransform(vocab=vocab),
            T.Truncate(max_seq_len=max_len),
            T.ToTensor(padding_value=vocab['<pad>']),
            T.PadTransform(max_length=max_len, pad_value=vocab['<pad>']),
        )
        self.dataset = TensorDataset(text_transform(self.reviews), torch.tensor(self.labels))

    
train_reader = DataReader(is_train=True)
test_reader = DataReader(is_train=False)

vocab = build_vocab_from_iterator(train_reader.reviews, min_freq=3, specials=['<pad>', '<unk>', '<cls>', '<sep>'])
vocab.set_default_index(vocab['<unk>'])

train_reader.build_dataset(vocab)
test_reader.build_dataset(vocab)

train_data = train_reader.dataset
test_data = test_reader.dataset

# 将train_data拆分成训练集和验证集
train_size = int(0.8 * len(train_data))
val_size = len(train_data) - train_size
train_data, val_data = torch.utils.data.random_split(train_data, [train_size, val_size])

batch_size = 64
train_iter = torch.utils.data.DataLoader(train_data, batch_size=batch_size, shuffle=True)
val_iter = torch.utils.data.DataLoader(val_data, batch_size=batch_size, shuffle=True)
test_iter = torch.utils.data.DataLoader(test_data, batch_size=batch_size, shuffle=True)

# %% [markdown]
# # RNN 训练

# %%
import torch.nn as nn
import matplotlib.pyplot as plt

class SentimentClassifierRNN(nn.Module):
    def __init__(self, vocab_size, embed_dim, num_class, device):
        super().__init__()
        self.embedding = nn.Embedding(vocab_size, embed_dim)
        self.encoder = nn.LSTM(embed_dim, embed_dim, batch_first=True)
        self.decoder = nn.Linear(embed_dim, num_class)
        self.device = device
    
    def forward(self, x):
        x = self.embedding(x)
        x, _ = self.encoder(x)
        x = x[:, -1, :]
        x = self.decoder(x)
        return x
    
    def train_proc(self, num_epochs, train_iter, loss_fn, optimizer):
        train_loss, val_loss, val_acc = [], [], []
        for epoch in range(num_epochs):
            self.train()
            for x, y in train_iter:
                x, y = x.to(self.device), y.to(self.device)
                y_pred = self(x)
                loss = loss_fn(y_pred, y)
                optimizer.zero_grad()
                loss.backward()
                optimizer.step()
            # print(f'epoch {epoch + 1}, loss {loss.item():.4f}')
            train_loss.append(loss.item())
            val_loss_item, val_acc_item = self.evaluate_proc(val_iter, loss_fn, prompt="val")
            val_loss.append(val_loss_item)
            val_acc.append(val_acc_item)
        self.plot_loss_acc(num_epochs, train_loss, val_loss, val_acc)
        
    def evaluate_proc(self, test_iter, loss_fn, prompt="test"):
        self.eval()
        corrects, total_loss = 0, 0
        for x, y in test_iter:
            x, y = x.to(self.device), y.to(self.device)
            y_pred = self(x)
            loss = loss_fn(y_pred, y)
            total_loss += loss.item()
            corrects += (y_pred.argmax(1) == y).sum().item()
        test_loss, test_acc = total_loss / len(test_iter), corrects / len(test_iter.dataset)
        # print(f'{prompt} loss {test_loss:.4f}, {prompt} acc {test_acc:.4f}')
        return test_loss, test_acc
    
    def plot_loss_acc(self, num_epochs, train_loss, val_loss, val_acc):
        # 使用matplotlib画出训练集和验证集的loss曲线
        plt.plot(range(1, num_epochs + 1), train_loss, label='train')
        plt.plot(range(1, num_epochs + 1), val_loss, label='val')
        plt.xlabel('epoch')
        plt.ylabel('loss')
        plt.legend()
        plt.show()
        # 使用matplotlib画出验证集的acc曲线
        plt.plot(range(1, num_epochs + 1), val_acc, label='val')
        plt.xlabel('epoch')
        plt.ylabel('acc')
        plt.legend()
        plt.show()

device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
model = SentimentClassifierRNN(len(vocab), 128, 2, device).to(device)
loss_fn = nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters(), lr=1e-3)

model.train_proc(15, train_iter, loss_fn, optimizer)
model.evaluate_proc(test_iter, loss_fn)
