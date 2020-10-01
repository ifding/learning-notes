"""
Tensorflow implementation of Makhzani, Alireza, et al. “Adversarial autoencoders.” 
arXiv preprint arXiv:1511.05644 (2015)
Reference:
    https://github.com/alimirzaei/adverserial-autoencoder-keras
    https://github.com/mrquincle/keras-adversarial-autoencoders
Author:
    ifding. Jul.22.2019
"""

import numpy as np

from tensorflow.keras.layers import Input, Dense, Lambda, Layer, Add, Multiply, LeakyReLU
from tensorflow.keras.models import Model, Sequential
from tensorflow.keras.optimizers import RMSprop, Adam
from tensorflow.keras import backend as K
from tensorflow.keras.initializers import RandomNormal
from utils import EPSILON, plot_digit_grid
from layers.functions import kl_normal, kl_discrete, sampling_normal, sampling_concrete
import matplotlib.pyplot as plt


class AAE(object):
    """
    Class to handle building and training VAE models.
    """
    def __init__(self, input_dim=784, latent_dim=8, hidden_dim=256):
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
        
        self.initializer = RandomNormal(mean=0.0, stddev=0.01, seed=None)

        self.optimizer = Adam(0.0002, 0.5)


    def fit(self, x_train, num_epochs=1, batch_size=100, val_split=.1,
            save_interval=10, reset_model=True):
        """
        Training model
        """
        if reset_model:
            self._set_model()

        if x_train.shape[0] % batch_size != 0:
            raise(RuntimeError("Training data shape {} is not divisible by batch size {}"
                               .format(x_train.shape[0], self.batch_size)))
            
        # Adversarial ground truths
        valid = np.ones((batch_size, 1))
        fake = np.zeros((batch_size, 1))

        # Training
        for epoch in range(num_epochs):
            
            # ---------------------
            #  Train Discriminator
            # ---------------------
                       
            idx = np.random.randint(0, x_train.shape[0], batch_size)
            imgs = x_train[idx]
            
            # Generate a batch of new images
            latent_fake = self.encoder.predict(imgs)
            # Here we generate the "TRUE" samples
            latent_real = self._sample_prior(batch_size, self.latent_dim)
                 
            # Train the discriminator
            d_loss_real = self.discriminator.train_on_batch(latent_real, valid)
            d_loss_fake = self.discriminator.train_on_batch(latent_fake, fake)
            d_loss = 0.5 * np.add(d_loss_real, d_loss_fake)
            
            # ---------------------
            #  Train Generator
            # ---------------------            

            # Train the generator
            g_loss = self.adversarial_autoencoder.train_on_batch(imgs, [imgs, valid])
            
            # Plot the progress (every 10th epoch)
            if epoch % 10 == 0:
                print ("%d [D loss: %f, acc: %.2f%%] [G loss: %f, mse: %f]" % 
                       (epoch, d_loss[0], 100*d_loss[1], g_loss[0], g_loss[1]))            

            # Save generated images (every sample interval, e.g. every 100th epoch)
            if epoch % save_interval == 0:
                self._sample_images(self.latent_dim, epoch)   

    def _set_model(self):
        """
        Setup model (method should only be called in self.fit())
        """
        print("Setting up model...")    
        # Build the encoder / decoder
        self.encoder = self._build_Encoder(self.input_dim, self.latent_dim)
        self.decoder = self._build_Decoder(self.latent_dim, self.input_dim)
        
        # Build and compile the discriminator
        self.discriminator = self._build_Descriminator(self.latent_dim)
        self.discriminator.compile(optimizer=self.optimizer,
                                   loss='binary_crossentropy',
                                   metrics=['accuracy']) 
        
        # The generator takes the image, encodes it and 
        # reconstructs it from the encoding
        img = Input(shape=(self.input_dim,))
        latent_code = self.encoder(img)
        reconstructed_img = self.decoder(latent_code)

        # For the adversarial_autoencoder model we will only train the generator
        # It will say something like: 
        #   UserWarning: Discrepancy between trainable weights and collected trainable weights, 
        #   did you set `model.trainable` without calling `model.compile` after ?
        # We only set trainable to false for the discriminator when it is part of the autoencoder...
        self.discriminator.trainable = False

        # The discriminator determines validity of the encoding
        validity = self.discriminator(latent_code)

        # The adversarial_autoencoder model  (stacked generator and discriminator)
        self.adversarial_autoencoder = Model(img, [reconstructed_img, validity])
        self.adversarial_autoencoder.compile(loss=['mse', 'binary_crossentropy'], 
                                        loss_weights=[0.999, 0.001], 
                                        optimizer=self.optimizer)        
             
        print("Completed model setup.")
        
        
    def _build_Encoder(self, img_shape, latent_dim):
        """ Build Encoder Model Based on Paper Configuration
        Args:
            img_shape (tuple) : shape of input image
            latent_dim (int) : number of latent variables
        Return:
            A sequential keras model
        """        
        deterministic = 1
        img = Input(shape=(img_shape,))
        h = Dense(512)(img)
        h = LeakyReLU(alpha=0.2)(h)
        h = Dense(512)(h)
        h = LeakyReLU(alpha=0.2)(h)
        if deterministic:
            latent_repr = Dense(latent_dim)(h)
        else:
            mu = Dense(latent_dim)(h)
            log_var = Dense(latent_dim)(h)
            latent_repr = Lambda(self._sampling_normal)([mu, log_var])
        return Model(img, latent_repr)    
        
    def _build_Decoder(self, latent_dim, img_shape):
        """ Build Decoder Model Based on Paper Configuration
        Args:
            latent_dim (int) : number of latent variables
            img_shape (tuple) : shape of target images
        Return:
            A sequential keras model
        """
        model = Sequential()
        model.add(Dense(512, input_dim=latent_dim))
        model.add(LeakyReLU(alpha=0.2))
        model.add(Dense(512))
        model.add(LeakyReLU(alpha=0.2))
        model.add(Dense(img_shape, activation='tanh'))
        z = Input(shape=(latent_dim,))
        img = model(z)
        return Model(z, img)  
    
    def _build_Descriminator(self, latent_dim):
        """ Build Descriminator Model Based on Paper Configuration
        Args:
            latent_dim (int) : number of latent variables
        Return:
            A sequential keras model
        """
        model = Sequential()
        model.add(Dense(512, input_dim=latent_dim))
        model.add(LeakyReLU(alpha=0.2))
        model.add(Dense(256))
        model.add(LeakyReLU(alpha=0.2))
        model.add(Dense(1, activation="sigmoid"))
        encoded_repr = Input(shape=(latent_dim, ))
        validity = model(encoded_repr)
        return Model(encoded_repr, validity)
       
    
    def extract_features(self, x, batch_size):
        return self.encoder.predict(x, batch_size=batch_size)
    
    def generate(self, latent_sample):
        """
        Generating examples from samples from the latent distribution.
        """

        return self.decoder.predict(latent_sample)

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
    
    def _sample_prior(self, batch_size, latent_dim):
        
        return np.random.normal(size=(batch_size, latent_dim))    
    
    def _sampling_normal(self, args):
        """
        Sampling from a normal distribution.
        """
        z_mean, z_log_var = args
        return sampling_normal(z_mean, z_log_var, (self.batch_size, self.latent_cont_dim))

    def _sample_images(self, latent_dim, epoch):
        r, c = 5, 5
        z = self._sample_prior(r*c, latent_dim)
        gen_imgs = self.decoder.predict(z)
        
        fig = plt.figure(figsize=[20, 20])
        for index,img in enumerate(gen_imgs):
            img = img.reshape((28, 28))
            ax = fig.add_subplot(r, c, index+1)
            ax.set_axis_off()
            ax.imshow(img, cmap="gray")
        fig.savefig("output/mnist_%d.png" % epoch)
        plt.close()        