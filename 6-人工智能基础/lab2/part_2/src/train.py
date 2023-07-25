# %%
import math
import torch
import torch.nn as nn
import torch.nn.functional as F
from typing import List

# %%
class char_tokenizer:
    """
    a very simple char-based tokenizer. the tokenizer turns a string into a list of integers.
    """

    def __init__(self, corpus: List[str]):
        self.corpus = corpus
        # TODO: calculate the vocab size and create a dictionary that maps each character to a unique integer
        self.vocab_size = len(corpus)
        self.char2idx = {char: idx for idx, char in enumerate(corpus)}
        # End of your code

    def encode(self, string: str):
        # TODO: convert a string into a list of integers and return, using the dictionary you created above
        return [self.char2idx[char] for char in string]
        # End of your code
 
    def decode(self, codes: List[int]):
        # TODO: convert a list of integers into a string and return, using the dictionary you created above
        return ''.join([self.corpus[idx] for idx in codes])
        # End of your code

# %%
class Head(nn.Module):
    """single head of self-attention"""

    def __init__(self, head_size, n_embd, block_size):
        super().__init__()
        self.head_size = head_size
        # TODO: create three linear layers, Key, Query, and Value, each of which maps from n_embd to head_size
        #       assign the layers to self.Key, self.Query, and self.Value
        self.Key = nn.Linear(n_embd, head_size)
        self.Query = nn.Linear(n_embd, head_size)
        self.Value = nn.Linear(n_embd, head_size)
        self.Dropout = nn.Dropout(0.1)
        # End of your code
        self.register_buffer("tril", torch.tril(torch.ones(block_size, block_size)))

    def forward(self, inputs):
        # TODO: implement the forward function of the head
        #       the input is a tensor of shape (batch, time, n_embd)
        #       the output should be a tensor of shape (batch, time, head_size)
        #       you may use the tril buffer defined above to mask out the upper triangular part of the affinity matrix
        keys = self.Key(inputs)
        queries = self.Query(inputs)
        values = self.Value(inputs)

        inputs_shape = inputs.shape[1]
        head_size = keys.shape[-1]

        out = queries @ keys.transpose(-1, -2) * (1.0 / math.sqrt(head_size))
        out = out.masked_fill(self.tril[:inputs_shape, :inputs_shape] == 0, float('-inf'))
        out = F.softmax(out, dim=-1)
        out = self.Dropout(out) @ values
        # End of your code
        return out

# %%
class MultiHeadAttention(nn.Module):
    def __init__(self, n_heads, head_size, n_embd, block_size):
        super().__init__()
        # TODO: implement heads and projection
        #       heads is a list of n_heads Head instances
        #       projection is a linear layer that maps from n_heads * head_size to n_embd
        self.heads = nn.ModuleList([Head(head_size, n_embd, block_size) for _ in range(n_heads)])
        self.projection = nn.Linear(n_heads * head_size, n_heads * head_size)
        self.dropout = nn.Dropout(0.1)
        # End of your code

    def forward(self, inputs):
        # TODO: implement the forward function of the multi-head attention
        #       the input is a tensor of shape (batch, time, n_embd)
        #       the output should be a tensor of shape (batch, time, n_embd)
        output = torch.cat([head(inputs) for head in self.heads], dim=-1)
        output = self.projection(output)
        output = self.dropout(output)
        return output


# %%
class FeedForward(nn.Module):
    def __init__(self, n_embd):
        super().__init__()
        # TODO: implement the feed-forward network
        self.net = nn.Sequential(
            nn.Linear(n_embd, 4 * n_embd),
            nn.ReLU(),
            nn.Linear(4 * n_embd, n_embd),
            nn.Dropout(0.1)
        )
        # End of your code

    def forward(self, inputs):
        return self.net(inputs)

# %%
class Block(nn.Module):
    def __init__(self, n_embd, n_heads, block_size):
        super().__init__()
        # TODO: implement the block of transformer using the MultiHeadAttention and 
        # FeedForward modules, along with the layer normalization layers
        self.attention = MultiHeadAttention(n_heads, n_embd // n_heads, n_embd, block_size)
        self.norm1 = nn.LayerNorm(n_embd)
        self.feedforward = FeedForward(n_embd)
        self.norm2 = nn.LayerNorm(n_embd)
        self.dropout = nn.Dropout(0.1)
        # End of your code

    def forward(self, inputs):
        # TODO: implement the forward function of the block, you may refer to the docs of this experiment
        out = self.norm1(inputs)
        out = self.attention(out) + inputs
        out = self.norm2(out)
        out = self.feedforward(out) + out
        out = self.dropout(out)
        return out
        # End of your code

# %%
class Transformer(nn.Module):
    def __init__(self, vocab_size, n_embd, n_heads, n_layers, block_size):
        super().__init__()
        # TODO: create the embedding table, the stack of blocks, the layer normalization layer, 
        # and the linear layers.
        # assign the layers to self.embedding, self.blocks, self.norm, and self.linear
        self.block_size = block_size
        self.embedding = nn.Embedding(vocab_size, n_embd)
        self.block_embedding = nn.Embedding(block_size, n_embd)
        self.blocks = nn.Sequential(*[Block(n_embd, n_heads, block_size) for _ in range(n_layers)])
        self.norm = nn.LayerNorm(n_embd)
        self.linear = nn.Linear(n_embd, vocab_size)
        # End of your code

    def forward(self, inputs, labels=None):
        # TODO: implement the forward function of the transformer
        #       the input is a tensor of shape (batch, context)
        #       the output should be a tensor of shape (batch, context, vocab_size)

        batch, context = inputs.shape
        inputs = self.embedding(inputs) + self.block_embedding(torch.arange(context, device=inputs.device))
        attentions = self.blocks(inputs)
        logits = self.linear(self.norm(attentions))
        # End of your code

        # compute the loss
        
        if labels is None:
            loss = None
        else:
            batch, time, channel = logits.shape
            logits = logits.view(batch * time, channel)
            labels = labels.view(batch * time)
            loss = F.cross_entropy(logits, labels)
        return logits, loss

    def generate(self, inputs, max_new_tokens):
        # TODO: generate new tokens from the transformer, using the inputs as the context,
        #  and return the generated tokens with length of max_new_tokens
        for _ in range(max_new_tokens):
            ex_inputs = inputs[:, -(self.block_size):]
            logits, _ = self(ex_inputs)
            logits = logits[:, -1, :]
            pred = F.softmax(logits, dim=1)
            new_token = torch.multinomial(pred, num_samples=1)
            inputs = torch.cat((inputs, new_token), dim=1)
        # End of your code
        return inputs

# %%
# define the hyperparameters
batch_size = 16
block_size = 256
max_iters = 5000 # set the number of training iterations as you like
eval_interval = 50
learning_rate = 1e-3
device = "cuda" if torch.cuda.is_available() else "cpu"
eval_iters = 200
n_embd = 64
n_heads = 8
n_layers = 6

# read the dataset
with open("../data/input.txt", "r", encoding="utf-8") as f:
    text = f.read()
chars = sorted(list(set(text)))

# initialize the vocabulary
tokenizer = char_tokenizer(chars)
encode = tokenizer.encode
decode = tokenizer.decode
n_vocab = tokenizer.vocab_size

# separate the dataset into train and validation
train_data = torch.tensor(encode(text[: -len(text) // 10]), dtype=torch.long)
val_data = torch.tensor(encode(text[-len(text) // 10 :]), dtype=torch.long)

def get_batch(split):
    data = train_data if split == "train" else val_data
    ix = torch.randint(len(data) - block_size, (batch_size,))
    x = torch.stack([data[i : i + block_size] for i in ix])
    y = torch.stack([data[i + 1 : i + block_size + 1] for i in ix])
    x, y = x.to(device), y.to(device)
    return x, y


@torch.no_grad()
def estimate_loss(model):
    out = {}
    model.eval()
    for split in ["train", "val"]:
        losses = torch.zeros(eval_iters)
        for k in range(eval_iters):
            x, y = get_batch(split)
            logits, loss = model(x, y)
            losses[k] = loss.item()
        out[split] = losses.mean()
    return out


def generate(model, context):
    context = torch.tensor(encode(context), dtype=torch.long, device=device)
    context = context.unsqueeze(0)
    print(decode(model.generate(context, max_new_tokens=500)[0].tolist()))


def train(model):
    optimizer = torch.optim.AdamW(model.parameters(), lr=learning_rate)

    train_loss = []
    val_loss = []

    for iter in range(max_iters):
        
        if iter % eval_interval == 0:
            losses = estimate_loss(model)
            train_loss.append(losses["train"])
            val_loss.append(losses["val"])
            print(
                f"step {iter}: train loss {losses['train']:.4f}, val loss {losses['val']:.4f}"
            )

        inputs, labels = get_batch("train")

        logits, loss = model(inputs, labels)
        optimizer.zero_grad(set_to_none=True)
        loss.backward()
        optimizer.step()
    return train_loss, val_loss
        
# define the model
model = Transformer(n_vocab, n_embd, n_heads, n_layers, block_size).to(device)
train_loss, val_loss = train(model)

# %%
# plot the loss curve
import matplotlib.pyplot as plt

plt.plot(train_loss, label="train")
plt.plot(val_loss, label="val")
plt.legend()
plt.show()

# %%
generate(model, "All the world's a stage, and all the men and ")


