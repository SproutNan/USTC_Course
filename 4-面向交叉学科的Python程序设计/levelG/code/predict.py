import numpy as np


def get_exp_seires(score: float):
    factor: int = None
    if score > 0:
        factor = 2
    else:
        factor = 3
    return np.logspace(factor, factor**2, num=30, base=10, dtype='int')


def get_inner_product(score: float, array: np.ndarray, tag: bool = True) -> float:
    exp_arr = get_exp_seires(score)
    sum = np.sum(exp_arr)
    ret = np.sum(np.multiply(exp_arr, array))
    if tag:
        return ret / sum
    else:
        return ret / sum

def computeCost(X, y, theta):
    inner: np.matrix = np.power(((X * theta.T) - y), 2)
    return inner.sum() / (2 * len(X))

def predict(score: float, array: np.ndarray, tag: bool = True) -> float:
    get_inner_product(score, array, tag)
    X = np.matrix(list(zip([1] * 30, range(30))))
    y = np.matrix(array).T
    average = np.sum(array) / len(array)
    theta = np.matrix(np.array([average, 0]))
    g, _ = gradientDescent(X, y, theta, 0.0001, 500)
    return g[0, 0] + (g[0, 1] * 30)
    
def gradientDescent(X: np.matrix, y: np.matrix, theta: np.matrix, alpha: float, iters: int):
    m, _ = X.shape
    cost = []
    
    for _ in range(iters):
        error: np.matrix = ((X * theta.T) - y).T * X / m
        theta = theta - alpha * error
        cost.append(computeCost(X, y, theta))
    return theta, cost