import numpy as np
from tensorflow.keras import backend as K
from utils import EPSILON


def kl_normal(z_mean, z_log_var):
    """
    KL divergence between N(0,1) and N(z_mean, exp(z_log_var)) where covariance
    matrix is diagonal.

    Parameters
    ----------
    z_mean : Tensor

    z_log_var : Tensor

    dim : int
        Dimension of tensor
    """
    # Sum over columns, so this now has size (batch_size,)
    kl_per_example = .5 * (K.sum(K.square(z_mean) + K.exp(z_log_var) - 1 - z_log_var, axis=1))
    return K.mean(kl_per_example)


def kl_discrete(dist):
    """
    KL divergence between a uniform distribution over num_cat categories and
    dist.

    Parameters
    ----------
    dist : Tensor - shape (None, num_categories)

    num_cat : int
    """
    num_categories = tuple(dist.get_shape().as_list())[1]
    dist_sum = K.sum(dist, axis=1)  # Sum over columns, this now has size (batch_size,)
    dist_neg_entropy = K.sum(dist * K.log(dist + EPSILON), axis=1)
    return np.log(num_categories) + K.mean(dist_neg_entropy - dist_sum)


def sampling_concrete(alpha, out_shape, temperature=0.67):
    """
    Sample from a concrete distribution with parameters alpha.

    Parameters
    ----------
    alpha : Tensor
        Parameters
    """
    uniform = K.random_uniform(shape=out_shape)
    gumbel = - K.log(- K.log(uniform + EPSILON) + EPSILON)
    logit = (K.log(alpha + EPSILON) + gumbel) / temperature
    return K.softmax(logit)


def sampling_normal(z_mean, z_log_var, out_shape):
    """
    Sampling from a normal distribution with mean z_mean and variance z_log_var
    """
    epsilon = K.random_normal(shape=out_shape, mean=0., stddev=1.)
    return z_mean + K.exp(z_log_var / 2) * epsilon