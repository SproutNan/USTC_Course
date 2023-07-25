# %% [markdown]
# # 定义图卷积层

# %%
import torch
import random
import torch.nn as nn
import torch.nn.functional as F
from torch.nn import Linear, Parameter
from torch_geometric.nn import MessagePassing
import torch_geometric.transforms as T
from torch_geometric.datasets import Planetoid
from torch_geometric.utils import add_self_loops, degree
from torch_geometric.nn import PairNorm

def drop_edge(edge_index, drop_rate=0.5):
    num_keep = int(edge_index.shape[1] * (1 - drop_rate))
    temp = [True] * num_keep + [False] * (edge_index.shape[1] - num_keep)
    random.shuffle(temp)
    return edge_index[:, temp]

class GCNConv(MessagePassing):
    def __init__(self, in_channels, out_channels):
        super().__init__(aggr='add')
        self.lin = Linear(in_channels, out_channels, bias=False)
        self.bias = Parameter(torch.Tensor(out_channels))
        self.reset_parameters()

    def reset_parameters(self):
        self.lin.reset_parameters()
        self.bias.data.zero_()

    def forward(self, x, edge_index):
        edge_index = drop_edge(edge_index, drop_rate=0.2)
        # 自环
        edge_index, _ = add_self_loops(edge_index, num_nodes=x.size(0))
        x = self.lin(x)
        row, col = edge_index
        deg = degree(col, x.size(0), dtype=x.dtype)
        deg_inv_sqrt = deg.pow(-0.5)
        deg_inv_sqrt[deg_inv_sqrt == float('inf')] = 0
        norm = deg_inv_sqrt[row] * deg_inv_sqrt[col]

        out = self.propagate(edge_index, x=x, norm=norm)
        return out + self.bias
    
    def message(self, x_j, norm):
        return norm.view(-1, 1) * x_j

# %% [markdown]
# # 定义图卷积网络

# %%
class GCN(nn.Module):
    def __init__(self, input, hidden, output):
        super().__init__()
        self.conv1 = GCNConv(input, hidden)
        self.norm = PairNorm()
        self.conv5 = GCNConv(hidden, output)
    
    def encode(self, x, edge_index):
        x = self.conv1(x, edge_index)
        x = F.relu(x)
        x = self.norm(x)
        return self.conv5(x, edge_index)
    
    def decode(self, z, edge_label_index):
        return (z[edge_label_index[0]] * z[edge_label_index[1]]).sum(dim=-1)
    
    def decode_all(self, z):
        prob_adj = z @ z.t()
        return (prob_adj > 0).nonzero(as_tuple=False).t()
    
    def forward(self, data):
        x, edge_index = data.x, data.edge_index
        x = self.encode(x, edge_index)
        return F.log_softmax(x, dim=1)

# %% [markdown]
# # 节点分类任务

# %%
def eval_node_classifier(model, data, mask):
    model.eval()
    pred = model(data).argmax(dim=1)
    correct = (pred[mask] == data.y[mask])
    acc = correct.sum().item() / mask.sum().item()
    return acc

def train_node_classifier(model, data, optimizer, criterion, n_epochs = 600):
    for epoch in range(n_epochs):
        model.train()
        optimizer.zero_grad()
        out = model(data)
        loss = criterion(out[data.train_mask], data.y[data.train_mask])
        loss.backward()
        optimizer.step()

        pred = out.argmax(dim=1)
        acc = eval_node_classifier(model, data, data.val_mask)

        if epoch % 10 == 0:
            print('.', end='')
    return model

def node_classifier():
    # 读取数据集
    dataset = Planetoid(root='./data/Cora', name='Cora', transform=T.NormalizeFeatures())
    data = dataset[0]

    # 分成训练集、测试集和验证集
    split = T.RandomNodeSplit(num_val=0.1, num_test=0.2)
    data = split(data)

    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
    model = GCN(dataset.num_node_features, 16, dataset.num_classes).to(device)
    optimizer = torch.optim.Adam(model.parameters(), lr=0.01, weight_decay=5e-4)
    criterion = nn.CrossEntropyLoss()
    model = train_node_classifier(model, data, optimizer, criterion)

    test_acc = eval_node_classifier(model, data, data.test_mask)
    print(f'\nTest Accuracy: {test_acc}')

# %% [markdown]
# # 链路预测任务

# %%
from sklearn.metrics import roc_auc_score
from torch_geometric.utils import negative_sampling
    
@torch.no_grad()
def eval_link_predictor(model, data):
    model.eval()
    z = model.encode(data.x, data.edge_index)
    out = model.decode(z, data.edge_label_index).view(-1).sigmoid()
    auc_roc = roc_auc_score(data.edge_label.detach().cpu().numpy(), out.detach().cpu().numpy())
    return auc_roc

def train_link_predictor(model, train_data, val_data, optimizer, criterion, n_epochs=600):
    for epoch in range(n_epochs):
        model.train()
        optimizer.zero_grad()
        z = model.encode(train_data.x, train_data.edge_index)

        # 从正样本中采样负样本
        neg_edge_index = negative_sampling(
            edge_index=train_data.edge_index,
            num_nodes=train_data.num_nodes,
            num_neg_samples=train_data.edge_label_index.size(1),
            method='sparse'
        )

        edge_label_index = torch.cat(
            [train_data.edge_label_index, neg_edge_index], 
            dim=-1
        )

        edge_label = torch.cat(
            [train_data.edge_label,
             torch.zeros(neg_edge_index.size(1))],
             dim=0
        )

        pred = model.decode(z, edge_label_index).view(-1)
        loss = criterion(pred, edge_label)
        loss.backward()
        optimizer.step()

        auc = eval_link_predictor(model, val_data)

        if epoch % 10 == 1:
            print(f'.', end="")

    return model

# %%
import torch_geometric.transforms as T

def link_predictor():
    # 读取数据集
    dataset = Planetoid(root='./data/Cora', name='Cora', transform=T.NormalizeFeatures())
    data = dataset[0]

    split = T.RandomLinkSplit(
        num_val=0.05,
        num_test=0.1,
        is_undirected=True,
        add_negative_train_samples=False,
        neg_sampling_ratio=1.0,
    )

    train_data, val_data, test_data = split(data)

    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
    model = GCN(dataset.num_node_features, 128, 64).to(device)
    optimizer = torch.optim.Adam(model.parameters(), lr=0.01)
    criterion = nn.BCEWithLogitsLoss()
    model = train_link_predictor(model, train_data, val_data, optimizer, criterion)

    auc = eval_link_predictor(model, test_data)
    print(f'\nTest AUC: {auc}')

# %% [markdown]
# # 测试内容

# %%
node_classifier()
link_predictor()


