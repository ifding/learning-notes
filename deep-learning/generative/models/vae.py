"""
Tensorflow implementation of Auto-Encoding Variational Bayes
   https://arxiv.org/abs/1312.6114
Reference:
    https://github.com/keras-team/keras/blob/master/examples/variational_autoencoder.py
    http://louistiao.me/listings/vae/variational_autoencoder.py.html
Author:
    ifding. Jul.22.2019
"""

import numpy as np

from tensorflow.keras.layers import Input, Dense, Lambda, Layer, Add, Multiply
from tensorflow.keras.models import Model, Sequential
from tensorflow.keras.optimizers import RMSprop
from tensorflow.keras import backend as K
from utils import EPSILON, plot_digit_grid
from layers.functions import kl_normal, kl_discrete, sampling_normal, sampling_concrete


class KLDivergenceLayer(Layer):

    """ Identity transform layer that adds KL divergence
    to the final model loss.
    """

    def __init__(self, *args, **kwargs):
        self.is_placeholder = True
        super(KLDivergenceLayer, self).__init__(*args, **kwargs)

    def call(self, inputs):

        mu, log_var = inputs

        kl_batch = - .5 * K.sum(1 + log_var -
                                K.square(mu) -
                                K.exp(log_var), axis=-1)

        self.add_loss(K.mean(kl_batch), inputs=inputs)

        return inputs



class VAE(object):
    """
    Class to handle building and training VAE models.
    """
    def __init__(self, input_dim=784, latent_dim=2, hidden_dim=256):
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
        self.klb_layer = KLDivergenceLayer()
        self.epsilon_std = 1.0

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
        # Encoder
        inputs = Input(shape=(self.input_dim,), name='encoder_input')
        hidden = Dense(self.hidden_dim, activation='relu')(inputs) 
        
        z_mu = Dense(self.latent_dim)(hidden)
        z_log_var = Dense(self.latent_dim)(hidden)
        
        z_mu, z_log_var = self.klb_layer([z_mu, z_log_var])
        z_sigma = Lambda(lambda t: K.exp(.5*t))(z_log_var)
        
        eps = Input(tensor=K.random_normal(stddev=self.epsilon_std,
                                   shape=(K.shape(inputs)[0], self.latent_dim)))
        
        z_eps = Multiply()([z_sigma, eps])
        z = Add()([z_mu, z_eps])
        
        # Decoder
        decoder = Sequential([
            Dense(self.hidden_dim, input_dim=self.latent_dim, activation='relu'),
            Dense(self.input_dim, activation='sigmoid')
        ])

        generated_inputs = decoder(z)
        
        self.model = Model(inputs=[inputs, eps], outputs=generated_inputs)
        
        self.encoder = Model(inputs, z_mu)       

        self.generator = decoder

        # Store latent distribution parameters
        self.z_mean = z_mu
        self.z_log_var = z_log_var


        # Compile models
        self.opt = RMSprop()
        #self.model.compile(optimizer=self.opt, loss=self._reconstruction_loss)
        self.model.compile(optimizer=self.opt, loss=self._vae_loss)        
        
        self.generator.compile(optimizer=self.opt, loss='mse')

        print("Completed model setup.")

    def extract_features(self, x, batch_size):
        return self.encoder.predict(x, batch_size=batch_size)
    
    def generate(self, latent_sample):
        """
        Generating examples from samples from the latent distribution.
        """

        return self.generator.predict(latent_sample)

    def plot(self):
        """
        Method to plot generated digits on a grid.
        """
        return plot_digit_grid(self, filename='./output/')
    
    def _vae_loss(self, x, x_generated):
        """
        Variational Auto Encoder loss.
        """
        x = K.flatten(x)        
        x_generated = K.flatten(x_generated)
        reconstruction_loss = self.input_dim * \
                                  K.binary_crossentropy(x, x_generated)
        kl_normal_loss = kl_normal(self.z_mean, self.z_log_var)

        return reconstruction_loss + kl_normal_loss
    

    def _reconstruction_loss(self, x, x_generated):
        """
        keras.losses.binary_crossentropy gives the mean
        over the last axis. we require the sum
        """
        return K.sum(K.binary_crossentropy(x, x_generated), axis=-1)   
    

    def _sampling_normal(self, args):
        """
        Sampling from a normal distribution.
        """
        z_mean, z_log_var = args
        return sampling_normal(z_mean, z_log_var, (self.batch_size, self.latent_cont_dim))

    def _sampling_concrete(self, args):
        """
        Sampling from a concrete distribution
        """
        return sampling_concrete(args, (self.batch_size, self.latent_disc_dim))
