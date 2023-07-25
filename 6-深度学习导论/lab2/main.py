# %% [markdown]
# # 设备初始化

# %%
import torch
import torch.nn as nn

def nnfw_loginfo():
    print(f"LOG: PyTorch version {torch.__version__}")
    print(f"LOG: CUDA version {torch.version.cuda}")

def nnfw_set_device(cuda_device: int = 0):
    device = torch.device(f"cuda:{cuda_device}")
    print(f"LOG: Using device {device}")
    return device

# %% [markdown]
# # 定义模型

# %%
kernel_size = 3

class CNN(nn.Module):
    def __init__(self):
        super(CNN, self).__init__()
        self.conv1 = nn.Sequential(
            nn.Conv2d(
                in_channels=3,
                out_channels=32,
                kernel_size=kernel_size,
                padding=1,
            ),
            nn.BatchNorm2d(32),
            nn.ReLU(),
            nn.Conv2d(
                in_channels=32,
                out_channels=64,
                kernel_size=kernel_size,
                padding=1,
            ),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=2, stride=2),
        )
        self.conv2 = nn.Sequential(
            nn.Conv2d(
                in_channels=64,
                out_channels=128,
                kernel_size=kernel_size,
                padding=1,
            ),
            nn.BatchNorm2d(128),
            nn.ReLU(),
            nn.Conv2d(
                in_channels=128,
                out_channels=128,
                kernel_size=kernel_size,
                padding=1,
            ),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=2, stride=2),
        )
        self.conv3 = nn.Sequential(
            nn.Conv2d(
                in_channels=128,
                out_channels=256,
                kernel_size=kernel_size,
                padding=1,
            ),
            nn.BatchNorm2d(256),
            nn.ReLU(),
            nn.Conv2d(
                in_channels=256,
                out_channels=256,
                kernel_size=kernel_size,
                padding=1,
            ),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=2, stride=2),
        )
        self.conv4 = nn.Sequential(
            nn.Conv2d(
                in_channels=256,
                out_channels=512,
                kernel_size=kernel_size,
                padding=1,
            ),
            nn.BatchNorm2d(512),
            nn.ReLU(),
            nn.Conv2d(
                in_channels=512,
                out_channels=512,
                kernel_size=kernel_size,
                padding=1,
            ),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=2, stride=2),
        )
        self.out = nn.Sequential(
            nn.Linear(2048, 1024),
            nn.ReLU(),
            nn.Dropout(p=0.4),
            nn.Linear(1024, 10),
        )

    def forward(self, x):
        x = self.conv1(x)
        x = self.conv2(x)
        x = self.conv3(x)
        x = self.conv4(x)
        x = x.view(x.size(0), -1)
        output = self.out(x)
        return output

# %% [markdown]
# # 建立神经网络

# %%
nnfw_loginfo()
cnn = CNN()
device = nnfw_set_device()
cnn.to(device)

# %% [markdown]
# # 获取训练集和测试集

# %%
# 获取CIFAR10数据集（使用pytorch）
import torchvision
from torch.utils.data import random_split
from torch.utils.data import DataLoader
import matplotlib.pyplot as plt

data_raw = torchvision.datasets.CIFAR10(
    root="./", 
    train=True,
    transform=torchvision.transforms.ToTensor(),
    download=False,
)

# 数据集大小
len_raw = len(data_raw)
divide_rate = 0.8
# 训练集大小
len_train = int(len_raw * divide_rate)

# 划分为训练集和验证集
train_set, val_set = random_split(data_raw, [len_train, len_raw - len_train])

# 测试，用plt查看第一张图片
# plt.imshow(train_set[0][0].permute(1, 2, 0))
# 对应的标签
# print(train_set[0][1])
# plt.show()

# %% [markdown]
# # 训练

# %%
optimizer = torch.optim.Adam(cnn.parameters(), lr=0.001)

decayRate = 0.92
lrdecayer = torch.optim.lr_scheduler.ExponentialLR(optimizer=optimizer, gamma=decayRate)

loss_func = nn.CrossEntropyLoss()

batch_size = 8
epoches = 20

train_loader = DataLoader(
    dataset=train_set,
    batch_size=batch_size,
    shuffle=True,
    pin_memory=True,
)

# 每轮训练后都进行验证
predict_loader = DataLoader(
    dataset=val_set,
    batch_size=1000,
    shuffle=True,
    pin_memory=True,
)

loss_for_plot = []
accuracy_for_plot = []

for epoch in range(epoches):
    epoch_loss = 0

    for step, (x, y) in enumerate(train_loader):
        x, y = x.to(device), y.to(device)
        output = cnn(x)
        loss = loss_func(output, y)
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()
        epoch_loss += loss.data.cpu().numpy()
        if step % 1000 == 0:
            print(f"Epoch: {epoch}, Step: {step}, Loss: {loss}")
    
    loss_for_plot.append(epoch_loss / len(train_loader))
            
    # 验证
    x, y = next(iter(predict_loader))
    x, y = x.to(device), y.to(device)
    output = cnn(x)
    pred_y = torch.max(output, 1)[1].data.cpu().numpy()
    accuracy = float((pred_y == y.data.cpu().numpy()).astype(int).sum()) / float(y.size(0))
    print(f"Epoch: {epoch}, Accuracy: {accuracy}")

    lrdecayer.step()

    accuracy_for_plot.append(accuracy)


# %%
# 把loss和accuracy画在一张图上
fig, ax1 = plt.subplots()
plt.xticks(range(0, epoches))
fig.suptitle('loss and accuracy for training')
ax2 = ax1.twinx()
ax1.plot(loss_for_plot, 'g-')
ax2.plot(accuracy_for_plot, 'b-')
ax2.set_ylim(0, 1)
ax1.set_xlabel('epoch')
ax1.set_ylabel('loss', color='g')
ax2.set_ylabel('accuracy', color='b')
plt.show()


# %% [markdown]
# # 测试集上测试

# %%
# 在测试集上测试
test_set = torchvision.datasets.CIFAR10(
    root="./",
    train=False,
    transform=torchvision.transforms.ToTensor(),
    download=False,
)

test_loader = DataLoader(
    dataset=test_set,
    batch_size=1000,
    shuffle=True,
    pin_memory=True,
)

x, y = next(iter(test_loader))
x, y = x.to(device), y.to(device)
output = cnn(x)
pred_y = torch.max(output, 1)[1].data.cpu().numpy()
accuracy = float((pred_y == y.data.cpu().numpy()).astype(int).sum()) / float(y.size(0))
print(f"Test Accuracy: {accuracy}")



