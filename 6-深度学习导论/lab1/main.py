# %% [markdown]
# # func define

# %%
import numpy as np
import pandas as pd
import os
import tqdm
import time

import torch
from torch import nn
from torch.optim import SGD
from torch.utils.data import DataLoader, TensorDataset

GD_FILES = ['data_train.csv', 'data_verify.csv', 'data_test.csv']
GD_RATES = [1, 0.5, 0.3]

def generate_data_check() -> bool:
    """
    check if data is already generated
    :return: true of false
    """
    file_list = os.listdir()
    return True if (file_list.count(GD_FILES[0]) and
                    file_list.count(GD_FILES[1]) and
                    file_list.count(GD_FILES[2])) else False

def generate_data(x_min: float, x_max: float, n: int):
    """
    generate data for training, verification and testing
    """
    if not generate_data_check():
        x = np.random.uniform(x_min, x_max, size=int(n*sum(GD_RATES)))
        y = np.log2(x) + np.cos(np.pi * x/2)
        df = pd.DataFrame({'x': x, 'y': y})
        df.drop_duplicates(keep='first', inplace=True)

        length = df.shape[0]
        df1 = df.iloc[range(int(n*GD_RATES[0]))]
        df2 = df.iloc[range(int(n*GD_RATES[0]) + 1, int(n*GD_RATES[0]) + int(n*GD_RATES[1]))]
        df3 = df.iloc[range(int(n*GD_RATES[0]) + int(n*GD_RATES[1]) + 1, length)]
        df1.to_csv('data_train.csv', index=False)
        df2.to_csv('data_verify.csv', index=False)
        df3.to_csv('data_test.csv', index=False)
        print('LOG: train, verify, test data generated successfully.')
    else:
        print('LOG: data sets are already generated, exit.')

def nnfw_loginfo():
    """
    log pytorch info
    """
    print(f"LOG: PyTorch version {torch.__version__}")
    # Check PyTorch has access to MPS (Metal Performance Shader, Apple's GPU architecture)
    print(f"LOG: MPS availability is {torch.backends.mps.is_available()}")

def nnfw_set_device():
    """
    set device for pytorch
    """
    device = "mps" if torch.backends.mps.is_available() else "cpu"
    device = torch.device(device)
    print(f"LOG: Using device {device}")
    return device

class MyModel(nn.Module):
    def __init__(self, hidden_size: int):
        super(MyModel, self).__init__()
        self.layers = nn.Sequential(
            nn.Linear(1, hidden_size),
            nn.Tanh(),
            nn.Linear(hidden_size, hidden_size),
            nn.Tanh(),
            nn.Linear(hidden_size, hidden_size),
            nn.Tanh(),
            nn.Linear(hidden_size, 1)
        )

    def forward(self, x):
        return self.layers(x)

# %% [markdown]
# # framework

# %%
n=50000
generate_data(x_min=1, x_max=16, n=n)
nnfw_loginfo()
device = nnfw_set_device()

# %%
# 从csv文件中读取数据
# 读取数据集
batches = 32

datasets = []
for i in range(3):
    df = pd.read_csv(GD_FILES[i])
    x = df['x'].to_numpy()[:, np.newaxis]
    y = df['y'].to_numpy()[:, np.newaxis]
    datasets.append((torch.Tensor(x), torch.Tensor(y)))

data_train = TensorDataset(*datasets[0])
data_train_loader = DataLoader(data_train, batches, shuffle=True)

data_verify = TensorDataset(*datasets[1])
data_verify_loader = DataLoader(data_verify, batches, shuffle=True)

data_test = TensorDataset(*datasets[2])
data_test_loader = DataLoader(data_test, batches, shuffle=True)

# %%
# 编写训练框架
lr = 0.01
model = MyModel(hidden_size=32)
criterion = nn.MSELoss()
optimizer = SGD(model.parameters(), lr=lr)
epochs = 400

# 最终需要的 loss 结果
loss_for_summary = []
label_for_summary = []
# 训练过程中的 loss
losses = []

# tqdm 进度条
qdm = tqdm.tqdm(total=epochs, desc='Training', ncols=80, leave=False)

start_time = time.time()
for epoch in range(epochs):
    for x, y in data_train_loader:
        y_pred = model(x)
        loss = criterion(y_pred, y)
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()
    losses.append(loss.item())
    qdm.update(1)
qdm.close()
end_time = time.time()

# %%
# 训练过程中的 loss 曲线
import matplotlib.pyplot as plt
plt.title('loss curve during training')
plt.plot(losses)
plt.show()

label_for_summary.append('average MSE of trainning')
loss_for_summary.append('{:.6f}'.format(sum(losses)/epochs))

label_for_summary.append('minimum MSE of trainning')
loss_for_summary.append('{:.6f}'.format(min(losses)))

# %%
# 在验证集上测试
model.eval()
with torch.no_grad():
    for x, y in data_verify_loader:
        y_pred = model(x)
        loss = criterion(y_pred, y)
        losses.append(loss)
    plt.title('loss curve with verify data')
    plt.plot(losses)
    plt.axvline(x=epochs, color='r')
    # plt.yscale('log')
    plt.show()

# 把验证集上的预测结果和真实结果画在一张图上
with torch.no_grad():
    xx, yy, ypp = [], [], []
    for x, y in data_verify_loader:
        xx = xx + x.numpy().tolist()
        yy = yy + y.numpy().tolist()
        y_pred = model(x)
        ypp = ypp + y_pred.numpy().tolist()
    plt.title('performance on verify data')
    plt.plot(xx, yy, 'b.', label='real')
    plt.plot(xx, ypp, 'r.', label='pred')
    plt.legend()
    plt.show()
    
    label_for_summary.append('MSE of verifying')
    loss_for_summary.append('{:.6f}'.format(sum(losses[epochs:])/(len(losses)-epochs)))

# %%
# 测试模型，把测试集上的预测结果和真实结果画在一张图上
with torch.no_grad():
    xx, yy, ypp = [], [], []
    losses = []
    for x, y in data_test_loader:
        xx = xx + x.numpy().tolist()
        yy = yy + y.numpy().tolist()
        y_pred = model(x)
        ypp = ypp + y_pred.numpy().tolist()
        losses.append(criterion(y_pred, y))
    plt.title('performance on test data')
    plt.plot(xx, yy, 'b.', label='real')
    plt.plot(xx, ypp, 'r.', label='pred')
    plt.legend()
    plt.show()

    label_for_summary.append('MSE of testing')
    loss_for_summary.append('{:.6f}'.format(sum(losses)/len(losses)))

# 输出当前模型的超参数
print('\n============================TRAINNING SUMMARY============================')
print(model.layers)

print('MSE:')
for i in range(len(loss_for_summary)):
    print(f'  {label_for_summary[i]} is {loss_for_summary[i]}')

print(f'epochs={epochs}, batches={batches}, lr={lr}')
print(f'dataset cases:')
print(f'  train set: {int(n*GD_RATES[0])} cases')
print(f'  verify set: {int(n*GD_RATES[1])} cases')
print(f'  test set: {int(n*GD_RATES[2])} cases')
print(f'trainning time: {end_time-start_time} seconds')


