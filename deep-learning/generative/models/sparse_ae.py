"""
Tensorflow implementation of Sparse autoencoder
   https://web.stanford.edu/class/cs294a/sparseAutoencoder.pdf
Reference:
    https://github.com/mrquincle/keras-adversarial-autoencoders
Author:
    ifding. Jul.24.2019
"""

import numpy as np

from tensorflow.keras.layers import Input, Dense, Lambda, Layer, Add, Multiply
from tensorflow.keras.models import Model, Sequential
from tensorflow.keras.optimizers import RMSprop
from tensorflow.keras import regularizers
from tensorflow.keras.regularizers import Regularizer
from tensorflow.keras import backend as K
from utils import EPSILON, plot_digit_grid
from layers.functions import kl_normal, kl_discrete, sampling_normal, sampling_concrete


# p * log(p / p_hat) + (1-p) * log( (1-p) / (1-p_hat) ) =
# (p * (K.log(p) - K.log(1e-10 + p_hat))) + ( (1-p) * (K.log(1-p) - K.log(1e-10 + 1-p_hat)))
def kl_divergence(kl_term, p, p_hat):
    
    return (kl_term - p * K.log(1e-10 + p_hat) - (1.0-p) * K.log(1e-10 + 1.0-p_hat))

class SparseActivityRegularizer(Regularizer):

    def __init__(self, p=0.1, sparsityBeta=3.0):
        self.p = K.cast_to_floatx(p)
        self.sparsityBeta = K.cast_to_floatx(sparsityBeta)
        self.kl_term = p * K.log(p) + (1.0-p) * K.log(1.0-p)

    def __call__(self, x):
        p_hat = K.mean(x, axis=0)
        regularization = self.sparsityBeta * K.sum(kl_divergence(self.kl_term, self.p, p_hat))
        #regularization += self.sparsityBeta * K.mean(kl_divergence(self.kl_term, self.p, p_hat))

        return regularization

    def get_config(self):
        return {"name": self.__class__.__name__, "p": float(self.p), "beta": float(self.sparsityBeta)}


class SparseAE(object):
    """
    Class to handle building and training VAE models.
    """
    def __init__(self, input_dim=784, latent_dim=32, hidden_dim=256):
        """
        Setting up everything.

        Parameters
        ----------
        input_dim : int
            Dimension of original inputs.

        latent_dim : int
            Dimension of latent distribution.

        hidden_dim : int
            Dimension of hidden layer.
        """
        self.opt = None
        self.model = None
        self.input_dim = input_dim
        self.latent_dim = latent_dim
        self.hidden_dim = hidden_dim

    def fit(self, x_train, num_epochs=1, batch_size=100, val_split=.1,
            learning_rate=1e-3, reset_model=True):
        """
        Training model
        """
        self.batch_size = batch_size
        self.num_epochs = num_epochs
        if reset_model:
            self._set_model()

        if x_train.shape[0] % batch_size != 0:
            raise(RuntimeError("Training data shape {} is not divisible by batch size {}"
                               .format(x_train.shape[0], self.batch_size)))

        self.model.fit(x_train, x_train,
                       epochs=self.num_epochs,
                       batch_size=self.batch_size,
                       validation_split=val_split)

    def _set_model(self):
        """
        Setup model (method should only be called in self.fit())
        """
        print("Setting up model...")
        
        # regularizer (in part-based p=0.05 and beta=3.0, here p=0.01 and beta=6.0 is used...)
        #regularizer = SparseActivityRegularizer(0.01, 6.0)
        regularizer = SparseActivityRegularizer(0.05, 3) 
        
        #regularizer = regularizers.l1(10e-8)
        #regularizer = regularizers.l1(10e-5)
        
        
        # Encoder
        inputs = Input(shape=(self.input_dim,), name='encoder_input')
        hidden = Dense(self.hidden_dim, activation='relu', 
                kernel_regularizer=regularizers.l2(0.003),
                activity_regularizer=regularizer)(inputs) 
        
        z = Dense(self.latent_dim, activation='relu', 
                kernel_regularizer=regularizers.l2(0.003),
                activity_regularizer=regularizer)(hidden)
        
        # Decoder
        decoder = Sequential([
            Dense(self.hidden_dim, input_dim=self.latent_dim, activation='relu'),
            Dense(self.input_dim, activation='sigmoid')
        ])

        generated_inputs = decoder(z)
        
        self.model = Model(inputs=inputs, outputs=generated_inputs)
        
        self.encoder = Model(inputs, z)       

        self.generator = decoder
        
        # Compile models
        self.opt = RMSprop()
        self.model.compile(optimizer=self.opt, loss='mse')
        # Loss and optimizer do not matter here as we do not train these models

        print("Completed model setup.")

    def extract_features(self, x, batch_size):
        return self.encoder.predict(x, batch_size=batch_size)  

    def _reconstruction_loss(self, x, x_generated):
        """
        keras.losses.binary_crossentropy gives the mean
        over the last axis. we require the sum
        """
        return K.sum(K.binary_crossentropy(x, x_generated), axis=-1)   
    
