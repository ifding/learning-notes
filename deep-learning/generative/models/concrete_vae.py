"""
Tensorflow implementation of a Variational Auto Encoder with a Concrete latent distribution. 
    See [Auto-Encoding Variational Bayes](https://arxiv.org/abs/1312.6114) and 
    [The Concrete Distribution: A Continuous Relaxation of Discrete Random Variables](https://arxiv.org/abs/1611.00712) 
    or [Categorical Reparameterization with Gumbel-Softmax](https://arxiv.org/abs/1611.01144) 
Reference:
    https://github.com/EmilienDupont/vae-concrete/blob/master/vae_concrete.py
Author:
    ifding. Jul.21.2019
"""

import numpy as np

from tensorflow.keras.layers import Input, Dense, Lambda, Flatten, Reshape, Concatenate
from tensorflow.keras.models import Model
from tensorflow.keras.optimizers import RMSprop
from tensorflow.keras import backend as K
from utils import EPSILON, plot_disc_digit_grid
from layers.functions import kl_normal, kl_discrete, sampling_normal, sampling_concrete


class ConcreateVAE(object):
    """
    Class to handle building and training VAE models.
    """
    def __init__(self, input_dim=28*28, latent_cont_dim=2,
                 latent_disc_dim=0, hidden_dim=256):
        """
        Setting up everything.

        Parameters
        ----------
        input_shape : Array-like, shape (num_rows*num_cols,)
            Shape of image.

        latent_cont_dim : int
            Dimension of latent distribution.

        latent_disc_dim : int
            Dimension of discrete latent distribution.

        hidden_dim : int
            Dimension of hidden layer.
        """
        self.opt = None
        self.model = None
        self.input_dim = input_dim
        self.latent_cont_dim = latent_cont_dim
        self.latent_disc_dim = latent_disc_dim
        self.latent_dim = self.latent_cont_dim + self.latent_disc_dim
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

        # Update parameters


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
        hidden = Dense(self.hidden_dim//2, activation='relu')(hidden)

        # Parameters for continous latent distribution
        z_mean = Dense(self.latent_cont_dim, name='z_mean')(hidden)
        z_log_var = Dense(self.latent_cont_dim, name='z_log_var')(hidden)
        # Parameters for concrete latent distribution
        if self.latent_disc_dim:
            alpha = Dense(self.latent_disc_dim, activation='softmax')(hidden)

        # Sample from latent distributions
        if self.latent_disc_dim:
            z = Lambda(self._sampling_normal)([z_mean, z_log_var])
            c = Lambda(self._sampling_concrete)(alpha)
            encoding = Concatenate()([z, c])
        else:
            encoding = Lambda(self._sampling_normal)([z_mean, z_log_var])
            
        # Decoder
        decoder_1 = Dense(self.hidden_dim//2, activation='relu')
        decoder_2 = Dense(self.hidden_dim, activation='relu')
        decoder_output = Dense(self.input_dim, activation='sigmoid')
            
        x = decoder_1(encoding)
        x = decoder_2(x)
        generated = decoder_output(x)

        self.model = Model(inputs, generated)
        
        self.encoder = Model(inputs, z_mean)

        # Set up generator
        inputs_G = Input(shape=(self.latent_dim,), name='generator_input')
        x = decoder_1(inputs_G)
        x = decoder_2(x)
        generated_G = decoder_output(x)        

        self.generator = Model(inputs_G, generated_G)

        # Store latent distribution parameters
        self.z_mean = z_mean
        self.z_log_var = z_log_var
        if self.latent_disc_dim:
            self.alpha = alpha

        # Compile models
        self.opt = RMSprop()
        #self.model.compile(optimizer=self.opt, loss=self._vae_loss)

        self.model.compile(optimizer=self.opt, loss=self._vae_loss)        
        
        
        # Loss and optimizer do not matter here as we do not train these models
        self.generator.compile(optimizer=self.opt, loss='mse')
        
        print(self.model.summary())
        
        print(self.generator.summary())

        print("Completed model setup.")
        
    def extract_features(self, x, batch_size):
        return self.encoder.predict(x, batch_size=batch_size)        

    def generate(self, latent_sample):
        """
        Generating examples from samples from the latent distribution.
        """
        # Model requires batch_size batches, so tile if this is not the case
        if latent_sample.shape[0] != self.batch_size:
            latent_sample = np.tile(latent_sample, self.batch_size).reshape(
                              (self.batch_size, self.latent_dim))
        return self.generator.predict(latent_sample, batch_size=self.batch_size)

    def plot(self):
        """
        Method to plot generated digits on a grid.
        """
        return plot_disc_digit_grid(self, filename='./output/')

    def _vae_loss(self, x, x_generated):
        """
        Variational Auto Encoder loss.
        """
        x = K.flatten(x)        
        x_generated = K.flatten(x_generated)
        reconstruction_loss = self.input_dim * \
                                  K.binary_crossentropy(x, x_generated)
        kl_normal_loss = kl_normal(self.z_mean, self.z_log_var)
        if self.latent_disc_dim:
            kl_disc_loss = kl_discrete(self.alpha)
        else:
            kl_disc_loss = 0
        return reconstruction_loss + kl_normal_loss + kl_disc_loss

    def _sampling_normal(self, args):
        """
        Sampling from a normal distribution.
        """
        z_mean, z_log_var = args
        return sampling_normal(z_mean, z_log_var, (self.latent_cont_dim,))

    def _sampling_concrete(self, args):
        """
        Sampling from a concrete distribution
        """
        return sampling_concrete(args, (self.latent_disc_dim,))
