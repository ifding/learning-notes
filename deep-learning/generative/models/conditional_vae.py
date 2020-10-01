"""
Tensorflow implementation of Learning Structured Output Representation using Deep Conditional Generative Models 
    https://papers.nips.cc/paper/5775-learning-structured-output-representation-using-deep-conditional-generative-models.pdf
Reference:
    https://github.com/nnormandin/Conditional_VAE/blob/master/conditional_vae.py
Author:
    ifding. Jul.24.2019
"""

import numpy as np

from tensorflow.keras.layers import Input, Dense, Lambda, Flatten, Reshape, Concatenate
from tensorflow.keras.models import Model
from tensorflow.keras.optimizers import RMSprop
from tensorflow.keras import backend as K
from tensorflow.keras.utils import to_categorical
import matplotlib.pyplot as plt
from utils import EPSILON
from layers.functions import kl_normal, kl_discrete, sampling_normal, sampling_concrete



class ConditionalVAE(object):
    """
    Class to handle building and training VAE models.
    """
    def __init__(self, input_dim=28*28, classes=10, latent_dim=2, hidden_dim=256):
        """
        Setting up everything.

        Parameters
        ----------
        input_shape : Array-like, shape (num_rows*num_cols,)
            Shape of image.

        latent_dim : int
            Dimension of latent distribution.
            
        hidden_dim : int
            Dimension of hidden layer.
        """
        self.opt = None
        self.model = None
        self.input_dim = input_dim
        self.classes = classes
        self.latent_dim = latent_dim
        self.hidden_dim = hidden_dim

    def fit(self, train_data, num_epochs=1, batch_size=100, val_split=.1,
            learning_rate=1e-3, reset_model=True):
        """
        Training model
        """
        self.batch_size = batch_size
        self.num_epochs = num_epochs
        x_train, y_train = train_data
        y_train = to_categorical(y_train) #one-hot encoded vectors
        if reset_model:
            self._set_model()

        if x_train.shape[0] % batch_size != 0:
            raise(RuntimeError("Training data shape {} is not divisible by batch size {}"
                               .format(x_train.shape[0], self.batch_size)))


        self.model.fit([x_train, y_train], x_train,
                       epochs=self.num_epochs,
                       batch_size=self.batch_size,
                       validation_split=val_split)

    def _set_model(self):
        """
        Setup model (method should only be called in self.fit())
        """
        print("Setting up model...")
        # Define the input layers for x and y
        x = Input(shape=(self.input_dim,), name='encoder_x')
        label = Input(shape=(self.classes,), name='encoder_label')
        
        # Concatenate the x and y for the inputs of Encoder
        inputs = Concatenate()([x, label])
        hidden = Dense(self.hidden_dim, activation='relu')(inputs) 
        hidden = Dense(self.hidden_dim//2, activation='relu')(hidden)

        # Parameters for continous latent distribution
        z_mean = Dense(self.latent_dim, name='z_mean')(hidden)
        z_log_var = Dense(self.latent_dim, name='z_log_var')(hidden)

        # Sample from latent distributions
        z = Lambda(self._sampling_normal)([z_mean, z_log_var])
        # merge latent space with label
        encoding = Concatenate()([z, label])
            
        # Decoder
        decoder_1 = Dense(self.hidden_dim//2, activation='relu')
        decoder_2 = Dense(self.hidden_dim, activation='relu')
        decoder_output = Dense(self.input_dim, activation='sigmoid')
            
        hidden = decoder_1(encoding)
        hidden = decoder_2(hidden)
        outputs = decoder_output(hidden)
        
        # Set up model and encoder
        self.model = Model([x, label], outputs)
        self.encoder = Model([x, label], z_mean)
        
        # Set up decoder
        decoder_inputs = Input(shape=(self.latent_dim+self.classes,))
        decoder_hidden = decoder_1(decoder_inputs)
        decoder_hidden = decoder_2(decoder_hidden)
        decoder_outputs = decoder_output(decoder_hidden)        
        
        self.decoder = Model(decoder_inputs, decoder_outputs)

        # Store latent distribution parameters
        self.z_mean = z_mean
        self.z_log_var = z_log_var

        # Compile models
        self.opt = RMSprop()
        self.model.compile(optimizer=self.opt, 
                           loss=self._vae_loss, 
                           metrics = [self._reconstruction_loss, self._kl_loss])        

        
        print(self.model.summary())
        
        print(self.decoder.summary())

        print("Completed model setup.")
        
    def extract_features(self, x, y, batch_size):
        y = to_categorical(y)
        return self.encoder.predict([x, y], batch_size=batch_size)        

    def generate(self, latent_sample):
        """
        Generating examples from samples from the latent distribution.
        """
        return self.decoder.predict(latent_sample)

    def plot(self):
        """
        Method to plot generated digits on a grid.
        """
        dig = 3
        sides = 8
        max_z = 1.5

        img_it = 0
        for i in range(0, sides):
            z1 = (((i / (sides-1)) * max_z)*2) - max_z
            for j in range(0, sides):
                z2 = (((j / (sides-1)) * max_z)*2) - max_z
                z_ = [z1, z2]
                vec = self._construct_numvec(dig, z_)
                decoded = self.generate(vec)
                plt.subplot(sides, sides, 1 + img_it)
                img_it +=1
                plt.imshow(decoded.reshape(28, 28), cmap = plt.cm.gray)
                plt.axis('off')
        plt.subplots_adjust(left=0, bottom=0, right=1, top=1, wspace=0, hspace=.2)
        plt.savefig('./output/conditional_vae.pdf')       

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
    
    def _kl_loss(self, y_true, y_pred):
        """
        KL_loss.
        """
        return kl_normal(self.z_mean, self.z_log_var)
    
    def _reconstruction_loss(self, y_true, y_pred):
        """
        reconstruction_loss.
        """
        return K.sum(K.binary_crossentropy(y_true, y_pred), axis=-1)    
    
    def _sampling_normal(self, args):
        """
        Sampling from a normal distribution.
        """
        z_mean, z_log_var = args
        return sampling_normal(z_mean, z_log_var, (self.latent_dim,))

    def _construct_numvec(self, digit, z = None):
        out = np.zeros((1, self.latent_dim+self.classes))
        out[:, digit + self.latent_dim] = 1.
        if z is None:
            return(out)
        else:
            for i in range(len(z)):
                out[:,i] = z[i]
            return(out)