# %%
import os
import pandas as pd
from sklearn.model_selection import train_test_split

class DataReader:
    def __init__(self, path='./aclImdb', is_train=True):
        self.reviews, self.labels = [], []
        for label in ['pos', 'neg']:
            folder_name = os.path.join(path, 'train' if is_train else 'test', label)
            for filename in os.listdir(folder_name):
                with open(os.path.join(folder_name, filename), mode='r', encoding='utf-8') as f:
                    self.reviews.append(f.read())
                    self.labels.append(1 if label == 'pos' else 0)
        self.data = pd.DataFrame({'reviews': self.reviews, 'labels': self.labels})

train_data = DataReader(is_train=True).data
# 分割成训练集和验证集
X_train, X_val, y_train, y_val = train_test_split(train_data['reviews'].values, train_data['labels'].values, test_size=0.2, random_state=42)
test_data = DataReader(is_train=False).data
X_test, y_test = test_data['reviews'].values, test_data['labels'].values

# %%
# 转换为BERT输入
import torch
from torch.utils.data import TensorDataset
from transformers import BertTokenizer

tokenizer = BertTokenizer.from_pretrained('bert-base-uncased', do_lower_case=True)

def convert_to_bert_inputs(data, labels):
    input_ids, attention_masks, token_type_ids = [], [], []
    for x in data:
        encoded_dict = tokenizer.encode_plus(
            x,
            max_length=512,
            pad_to_max_length=True,
            return_attention_mask=True,
            return_tensors='pt',
            truncation=True
        )
        input_ids.append(encoded_dict['input_ids'])
        attention_masks.append(encoded_dict['attention_mask'])
        token_type_ids.append(encoded_dict['token_type_ids'])
    input_ids = torch.cat(input_ids, dim=0)
    attention_masks = torch.cat(attention_masks, dim=0)
    token_type_ids = torch.cat(token_type_ids, dim=0)

    labels = torch.tensor(labels)
    return TensorDataset(input_ids, attention_masks, token_type_ids, labels)

train_dataset = convert_to_bert_inputs(X_train, y_train)
val_dataset = convert_to_bert_inputs(X_val, y_val)
test_dataset = convert_to_bert_inputs(X_test, y_test)

# %%
# 将数据放入DataLoader中
from torch.utils.data import DataLoader, RandomSampler, SequentialSampler

batch_size = 32

train_dataloader = DataLoader(
    train_dataset,
    sampler=RandomSampler(train_dataset),
    batch_size=batch_size
)

val_dataloader = DataLoader(
    val_dataset,
    sampler=SequentialSampler(val_dataset),
    batch_size=batch_size
)

test_dataloader = DataLoader(
    test_dataset,
    sampler=SequentialSampler(test_dataset),
    batch_size=batch_size
)

device = torch.device('cuda:2' if torch.cuda.is_available() else 'cpu')

# %%
import torch
import torch.nn as nn
from transformers import BertModel

# Define BERT model architecture
class SentimentClassifierBERT(nn.Module):
    def __init__(self, num_labels):
        super(SentimentClassifierBERT, self).__init__()
        self.bert = BertModel.from_pretrained('bert-base-uncased')
        self.dropout = nn.Dropout(0.1)
        self.fc = nn.Linear(self.bert.config.hidden_size, num_labels)

    def forward(self, input_ids, attention_mask, token_type_ids):
        outputs = self.bert(input_ids=input_ids,
                            attention_mask=attention_mask,
                            token_type_ids=token_type_ids)
        pooled_output = outputs[1]
        pooled_output = self.dropout(pooled_output)
        logits = self.fc(pooled_output)
        return logits

# Define optimizer and loss function
model = SentimentClassifierBERT(num_labels=2).to(device)
optimizer = torch.optim.AdamW(model.parameters(), lr=2e-5, eps=1e-8)
criterion = nn.CrossEntropyLoss()

# Train the model
num_epochs = 2
batch_size = 64

for epoch in range(num_epochs):
    print(f'Epoch {epoch+1}/{num_epochs}')
    # Train
    model.train()
    train_loss = 0.0
    for batch in train_dataloader:
        batch = tuple(t.to(device) for t in batch)
        input_ids, attention_mask, token_type_ids, labels = batch
        optimizer.zero_grad()
        logits = model(input_ids, attention_mask, token_type_ids)
        loss = criterion(logits, labels)
        loss.backward()
        optimizer.step()
        train_loss += loss.item()
    print(f'Training Loss: {train_loss:.4f}')

    # Evaluate on validation set
    model.eval()
    val_loss = 0.0
    total_preds = []
    with torch.no_grad():
        for batch in val_dataloader:
            batch = tuple(t.to(device) for t in batch)
            input_ids, attention_mask, token_type_ids, labels = batch
            logits = model(input_ids, attention_mask, token_type_ids)
            loss = criterion(logits, labels)
            val_loss += loss.item()
            preds = logits.argmax(dim=1).detach().cpu().numpy()
            total_preds.extend(preds)

    # Print statistics
    train_loss /= len(train_dataloader)
    val_loss /= len(val_dataloader)
    val_acc = (total_preds == y_val).mean()

    print(f'Epoch {epoch+1}/{num_epochs}:')
    print(f'Training Loss: {train_loss:.4f} | Validation Loss: {val_loss:.4f} | Validation Accuracy: {val_acc:.4f}')

# %%
# Test the model
model.eval()
test_loss = 0.0
total_preds = []
with torch.no_grad():
    for batch in test_dataloader:
        batch = tuple(t.to(device) for t in batch)
        input_ids, attention_mask, token_type_ids, labels = batch
        logits = model(input_ids, attention_mask, token_type_ids)
        loss = criterion(logits, labels)
        test_loss += loss.item()
        preds = logits.argmax(dim=1).detach().cpu().numpy()
        total_preds.extend(preds)

test_loss /= len(test_dataloader)
test_acc = (total_preds == y_test).mean()
print(f'Test Loss: {test_loss:.4f} | Test Accuracy: {test_acc:.4f}')
