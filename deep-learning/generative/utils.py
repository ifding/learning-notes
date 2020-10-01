"""
Reference:
    https://github.com/EmilienDupont/vae-concrete/blob/master/vae_concrete.py
Author:
    ifding. Jul.21.2019
"""

import numpy as np
import time
from tensorflow.keras.datasets import mnist
import matplotlib.pyplot as plt
from scipy.stats import norm

EPSILON = 1e-8


def load_conv_mnist():
    """
    Get normalized MNIST datasets with correct shapes (Tensorflow style).
    """
    (x_train, y_train), (x_test, y_test) = mnist.load_data()
    x_train = x_train.astype('float32') / 255.
    x_train = x_train.reshape(x_train.shape + (1,))
    x_test = x_test.astype('float32') / 255.
    x_test = x_test.reshape(x_test.shape + (1,))
    return (x_train, y_train), (x_test, y_test)

def load_mnist():
    """
    the data, shuffled and split between train and test sets
    """
    (x_train, y_train), (x_test, y_test) = mnist.load_data()
    x_train = x_train.reshape([x_train.shape[0], -1]) / 255.0
    x_test = x_test.reshape([x_test.shape[0], -1]) / 255.0
    print('MNIST Train samples', x_train.shape)
    print('MNIST Test samples', x_test.shape)
    return (x_train, y_train), (x_test, y_test)

def get_one_hot_vector(idx, dim=10):
    """
    Returns a 1-hot vector of dimension dim with the 1 at index idx

    Parameters
    ----------
    idx : int
        Index where one hot vector is 1

    dim : int
        Dimension of one hot vector
    """
    one_hot = np.zeros(dim)
    one_hot[idx] = 1.
    return one_hot


def plot_disc_digit_grid(model, fig_size=15, digit_size=28, filename='./output/'):
    """
    Plot a grid of generated digits. Each column corresponds to a different
    setting of the discrete variable, each row to a random setting of the other
    latent variables.
    Parameters
    ----------
    model : VAE model
    fig_size : int
    digit_size : int
    std_dev : float
    filename : string
    """
    figure = np.zeros((digit_size * fig_size, digit_size * fig_size))
    grid_x = np.linspace(0.05, 0.95, fig_size)
    grid_y = np.linspace(0.05, 0.95, fig_size)

    for i, xi in enumerate(grid_x):
        for j, yi in enumerate(grid_y):
            # Sample from latent distribution
            if model.latent_disc_dim:
                z_sample = np.random.rand(model.latent_cont_dim)
                c_sample = get_one_hot_vector(j % model.latent_disc_dim, model.latent_disc_dim)
                latent_sample = np.hstack((z_sample, c_sample))
            else:
                latent_sample = np.random.rand(model.latent_dim)
                latent_sample[0] = xi
                latent_sample[1] = yi
            # Generate a digit and plot it
            generated = model.generate(latent_sample)
            digit = generated[0].reshape(digit_size, digit_size)
            figure[i * digit_size: (i + 1) * digit_size,
                   j * digit_size: (j + 1) * digit_size] = digit            
            

    f = plt.figure(figsize=(10, 10))
    ax = f.add_subplot(111)
    ax.imshow(figure, cmap='Greys_r')
    f.savefig(filename + 'vae.pdf')
    
def plot_digit_grid(model, fig_size=15, digit_size=28, filename='./output/'):
    """
    Plot a grid of generated digits. Each column corresponds to a different
    setting of the discrete variable, each row to a random setting of the other
    latent variables.
    Parameters
    ----------
    model : VAE model
    fig_size : int
    digit_size : int
    std_dev : float
    filename : string
    """
    # linearly spaced coordinates on the unit square were transformed
    # through the inverse CDF (ppf) of the Gaussian to produce values
    # of the latent variables z, since the prior of the latent space
    # is Gaussian

    z1 = norm.ppf(np.linspace(0.01, 0.99, fig_size))
    z2 = norm.ppf(np.linspace(0.01, 0.99, fig_size))
    z_grid = np.dstack(np.meshgrid(z1, z2))    
    latent_sample = z_grid.reshape(fig_size*fig_size, model.latent_dim)
    x_pred_grid = model.generate(latent_sample) \
                     .reshape(fig_size, fig_size, digit_size, digit_size)

    plt.figure(figsize=(10, 10))
    plt.imshow(np.block(list(map(list, x_pred_grid))), cmap='gray')
    plt.savefig(filename + 'vae.pdf') 
    

def plot_features(z_test, y_test, filename='./output/'): 
    plt.figure(figsize=(6, 6))
    plt.scatter(z_test[:, 0], z_test[:, 1], c=y_test,
                alpha=.4, s=3**2, cmap='viridis')
    plt.colorbar()
    plt.savefig(filename + 'features.pdf')
   