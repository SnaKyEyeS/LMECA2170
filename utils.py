import numpy as np


def loguniform(low=0, high=1, size=None, base=10):
    """
    Return a random number following a loguniform distribution, from:
    https://stackoverflow.com/questions/43977717/how-do-i-generate-log-uniform-distribution-in-python
    """
    return np.power(base, np.random.uniform(low, high, size))
