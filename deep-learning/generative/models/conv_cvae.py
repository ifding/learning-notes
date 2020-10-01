"""
Tensorflow implementation of Conditional Auto-Encoding Variational Bayes and beta-vae
    https://arxiv.org/abs/1312.6114
Reference:
    https://github.com/mpatacchiola/Y-AE/blob/master/models/cvae.py
Author:
    ifding. Aug.2.2019
"""

import tensorflow as tf
import numpy as np
from time import gmtime, strftime
import os

tf.compat.v1.disable_eager_execution()

class Autoencoder(object):
    def __init__(self, channels=1, conv_filters=8, style_size=32, content_size=10, ksize=(3,3), start_iteration=0, dir_header="./", wdecay=0.0, beta=1.0):
        '''Init method
        @param channels tot image channels
        @param conv_filters (int) the number of filters in the convolutional layers
        @param style_size (int) the number of units in the style layer
        @param content_size (int) the number of units in the content layer
        @param ksize (tuple) size of the kernels
        @param start_iteration the first iteration index
        @param dir_header the path to the root folder
        @param wdecay (float) the amount of weight decay to apply
        @param beta the hyperparameter used in beta-VAE, beta=1 is a standard VAE.
        '''
        self.dir_header = dir_header
        self.start_iteration = start_iteration
        self.channels = channels
        self.beta = beta
        self.conv_filters = conv_filters
        self.style_size = style_size
        self.ksize = ksize
        self.content_size = content_size
        self.weight_initializer = None #Tf default init
        self.weight_initializer_implicit = None
        self.bias_initializer_implicit = None

        if(wdecay > 0.0):
            self.regularizer = tf.keras.regularizers.l2(wdecay) #0.00001 is a good choice
        else:
            self.regularizer = None  
                       
        self.x = tf.keras.layers.Input(shape=(32, 32, self.channels), name='x_input')        
        self.labels = tf.keras.layers.Input(shape=(), dtype=tf.int64, name='label_input')

        mu, std_log_sq = self._build_Encoder(self.x)       
        Encoder = tf.keras.Model(inputs=self.x, outputs=[mu, std_log_sq])
        
        code_reshaped = self._sampling(mu, std_log_sq)

        Decoder = self._build_Decoder()
        self.output = Decoder(code_reshaped, training=True) 

        var_list = Encoder.trainable_variables + Decoder.trainable_variables

        self._set_loss()
       
        ##Train ops
        self.learning_rate = tf.compat.v1.placeholder(tf.float32)
        self.train_op = tf.compat.v1.train.AdamOptimizer(learning_rate=self.learning_rate, 
                                               beta1=0.9, beta2=0.999, 
                                               epsilon=1e-08).minimize(self.loss, var_list=var_list)

        self.tf_saver = tf.compat.v1.train.Saver()
        self.train_iteration = start_iteration        
        
        self._set_summary() 
        
    def _sampling(self, mu, std_log_sq):
        
        self.mu = tf.squeeze(mu, axis=[1,2])
        
        self.std_log_sq = tf.squeeze(std_log_sq, axis=[1,2])
        
        eps = tf.random.normal([tf.shape(self.x)[0], self.style_size], 0, 1, dtype=tf.float32)            
        # z = mu + sigma*epsilon
        self.code_style = self.mu + (tf.sqrt(tf.exp(self.std_log_sq)) * eps)            
       
        self.code_content_deterministic = tf.one_hot(indices=self.labels, depth=self.content_size)
        
        code = tf.concat([self.code_style, self.code_content_deterministic], axis=1)

        ##Transpose-Convolution-1 -> (2, 2, conv_filters*4)
        code_reshaped = tf.reshape(code, [tf.shape(code)[0], 1, 1, self.style_size+self.content_size])
        
        return code_reshaped
          
    def _build_Encoder(self, x):
            conv_1 = tf.keras.layers.Conv2D(filters=self.conv_filters, 
                                            strides=(2,2), kernel_size=self.ksize, 
                                            padding="same", activation=None, 
                                            kernel_regularizer=self.regularizer, 
                                            kernel_initializer=self.weight_initializer, 
                                            name="conv_1")(x)
            conv_1 = tf.keras.layers.BatchNormalization(axis=-1, momentum=0.99, epsilon=0.001, 
                                                        name="norm_1")(conv_1)
            conv_1 = tf.keras.layers.LeakyReLU(name="relu_1")(conv_1)

            ##Conv-2 -> (8, 8, conv_filters*2)
            conv_2 = tf.keras.layers.Conv2D(filters=self.conv_filters*2, 
                                            strides=(2,2), kernel_size=self.ksize, 
                                            padding="same", activation=None, 
                                            kernel_regularizer=self.regularizer, 
                                            kernel_initializer=self.weight_initializer, 
                                            name="conv_2")(conv_1)
            conv_2 = tf.keras.layers.BatchNormalization(axis=-1, momentum=0.99, 
                                                        epsilon=0.001, name="norm_2")(conv_2)
            conv_2 = tf.keras.layers.LeakyReLU(name="relu_2")(conv_2)

            ##Conv-3 -> (4, 4, conv_filters*4)
            conv_3 = tf.keras.layers.Conv2D(filters=self.conv_filters*4, 
                                            strides=(2,2), kernel_size=self.ksize, 
                                            padding="same", activation=None, 
                                            kernel_regularizer=self.regularizer, 
                                            kernel_initializer=self.weight_initializer, 
                                            name="conv_3")(conv_2)
            conv_3 = tf.keras.layers.BatchNormalization(axis=-1, momentum=0.99, 
                                                        epsilon=0.001, name="norm_3")(conv_3)
            conv_3 = tf.keras.layers.LeakyReLU(name="relu_3")(conv_3)

            ##Conv-4 -> (2, 2, conv_filters*4)
            conv_4 = tf.keras.layers.Conv2D(filters=self.conv_filters*4, 
                                            strides=(2,2), kernel_size=self.ksize, 
                                            padding="same", activation=None, 
                                            kernel_regularizer=self.regularizer, 
                                            kernel_initializer=self.weight_initializer, 
                                            name="conv_4")(conv_3)
            conv_4 = tf.keras.layers.BatchNormalization(axis=-1, momentum=0.99, 
                                                        epsilon=0.001, name="norm_4")(conv_4)
            conv_4 = tf.keras.layers.LeakyReLU(name="relu_4")(conv_4)
            
            ##Conv-5 STYLE
            mu = tf.keras.layers.Conv2D(filters=self.style_size, 
                                        strides=(2,2), kernel_size=self.ksize,
                                        padding="same", activation=None, 
                                        kernel_regularizer=self.regularizer, 
                                        kernel_initializer=self.weight_initializer_implicit, 
                                        bias_initializer=self.bias_initializer_implicit, 
                                        name="mu")(conv_4)            
            
            ##Conv-6 CONTENT           
            std_log_sq = tf.keras.layers.Conv2D(filters=self.style_size, strides=(2,2),
                                                kernel_size=self.ksize, 
                                                padding="same", activation=None, 
                                                kernel_regularizer=self.regularizer, 
                                                kernel_initializer=self.weight_initializer_implicit,
                                                bias_initializer=self.bias_initializer_implicit, 
                                                name="std")(conv_4)
                      
            return mu, std_log_sq
        
            
    def _build_Decoder(self):
        Decoder = tf.keras.Sequential([
            ##Transpose-Convolution-1 -> (2, 2, conv_filters*4)
            tf.keras.layers.Conv2DTranspose(filters=self.conv_filters*4, kernel_size=self.ksize, 
                                            strides=(2,2), padding="same", activation=None, 
                                            kernel_regularizer=self.regularizer, 
                                            kernel_initializer=self.weight_initializer),
            tf.keras.layers.BatchNormalization(axis=-1, momentum=0.99, epsilon=0.001),
            tf.keras.layers.LeakyReLU(),
            ##Transpose-Convolution-2 -> (4, 4, conv_filters*4)
            tf.keras.layers.Conv2DTranspose(filters=self.conv_filters*4, kernel_size=self.ksize, 
                                            strides=(2,2), padding="same", activation=None, 
                                            kernel_regularizer=self.regularizer, 
                                            kernel_initializer=self.weight_initializer),
            tf.keras.layers.BatchNormalization(axis=-1, momentum=0.99, epsilon=0.001),
            tf.keras.layers.LeakyReLU(),
            ##Transpose-Convolution-3 -> (8, 8, conv_filters*4)
            tf.keras.layers.Conv2DTranspose(filters=self.conv_filters*4, kernel_size=self.ksize, 
                                            strides=(2,2), padding="same", activation=None, 
                                            kernel_regularizer=self.regularizer, 
                                            kernel_initializer=self.weight_initializer),
            tf.keras.layers.BatchNormalization(axis=-1, momentum=0.99, epsilon=0.001),
            tf.keras.layers.LeakyReLU(),
            ##Transpose-Convolution-4 -> (16, 16, conv_filters*4)
            tf.keras.layers.Conv2DTranspose(filters=self.conv_filters*2, kernel_size=self.ksize, 
                                            strides=(2,2), padding="same", activation=None, 
                                            kernel_regularizer=self.regularizer, 
                                            kernel_initializer=self.weight_initializer),           
            tf.keras.layers.BatchNormalization(axis=-1, momentum=0.99, epsilon=0.001),
            tf.keras.layers.LeakyReLU(),
            ##Transpose-Convolution-5 -> (32, 32, conv_filters*2)
            tf.keras.layers.Conv2DTranspose(filters=self.conv_filters, kernel_size=self.ksize, 
                                            strides=(2,2), padding="same", activation=None, 
                                            kernel_regularizer=self.regularizer, 
                                            kernel_initializer=self.weight_initializer),
            tf.keras.layers.BatchNormalization(axis=-1, momentum=0.99, epsilon=0.001),
            tf.keras.layers.LeakyReLU(),
            ##Transpose-Convolution-6 -> (32, 32, 3)
            tf.keras.layers.Conv2DTranspose(filters=self.channels, kernel_size=self.ksize, 
                                            strides=(1,1), padding="same", activation='sigmoid', 
                                            kernel_regularizer=self.regularizer, 
                                            kernel_initializer=self.weight_initializer)
        ]) 
        return Decoder
    
    
    def _set_loss(self):
        ##Reconstruction loss
        #self.reconstruction_loss = tf.reduce_sum(tf.square(tf.subtract(self.x, self.output)), axis=[1,2,3])[:,None]
        #reconstruction_loss = tf.reduce_sum(tf.square(tf.subtract(self.x, self.output)), axis=[1,2,3], keepdims=True)
        #self.reconstruction_loss = tf.reduce_mean(reconstruction_loss)
        self.reconstruction_loss = tf.reduce_mean(tf.square(tf.subtract(self.x, self.output))) #L2 loss
        
        ##KL-loss
        kl_loss = -0.5 * tf.reduce_sum(1.0 + self.std_log_sq - tf.square(self.mu) - tf.exp(self.std_log_sq), 
                                            axis=1, keepdims=True)
        self.kl_loss = tf.reduce_mean(kl_loss)

        ##Global Loss
        self.beta_placeholder = tf.compat.v1.placeholder(tf.float32)
        #self.loss = tf.reduce_mean(self.reconstruction_loss + (self.beta_placeholder * self.kl_loss))
        self.loss = self.reconstruction_loss + (self.beta_placeholder * self.kl_loss)        

    
    def _set_summary(self):
        #Summaries
        with tf.compat.v1.variable_scope("Summaries"):
            tf.compat.v1.summary.image("input_images", self.x, max_outputs=8, family="original")
            tf.compat.v1.summary.image("reconstruction_images", self.output, max_outputs=8, family="reconstructed_left")
            tf.compat.v1.summary.scalar("loss", self.loss, family="_loss_main")
            tf.compat.v1.summary.scalar("reconstruction_loss", self.reconstruction_loss, family="losses_reconstruction")
            tf.compat.v1.summary.histogram("hist_style", self.code_style, family="code")            

    def init_summary(self, sess):
        '''Init the summary folder with current time and date
        @param sess (tf.Session) the current session
        '''
        summary_id = strftime("%H%M%S_%d%m%Y", gmtime())
        summary_folder = self.dir_header + '/log/' + summary_id + '_iter_' + str(self.start_iteration)
        self.tf_summary_writer = tf.compat.v1.summary.FileWriter(summary_folder, sess.graph)
        self.summaries = tf.compat.v1.summary.merge_all() #merge all the previous summaries
        
    def forward(self, sess, input_feature):
        '''Feed-forward pass in the autoencoder
        @param sess (tf.Session) the current session
        @param input_feature (np.array) matrix of features
        @return (np.array) the output of the autoencoder (reconstruction)
        '''
        output = sess.run([self.output], feed_dict={self.x: input_feature})
        return output

    def forward_conditional(self, sess, input_features, input_labels):
        '''Forward step conditioned on the labels
        @param sess (tf.Session) the current session
        @param input_feature (np.array) matrix of features
        @param input_labels (np.array) array of labels
        @param lambda_e explicit mixing coefficient
        @param lambda_i implicit mixing coefficient
        @return (float) the output  (reconstruction)
        '''
        output = sess.run([self.output], feed_dict={self.x: input_features, 
                                                    self.labels: input_labels})
        return output[0]
        
    def test(self, sess, input_features, input_labels):
        '''Single step test of the autoencoder
        @param sess (tf.Session) the current session
        @param input_feature (np.array) matrix or array of feature
        @param lambda_e explicit mixing coefficient
        @param lambda_i implicit mixing coefficient
        @return (float) the losses: loss, loss_r, loss_c, acc_c, loss_e, loss_i
        '''
        loss, loss_r  = sess.run([self.loss, self.reconstruction_loss], 
                                 feed_dict={self.x: input_features, self.labels: input_labels})
        return loss_r #return the reconstruction loss

    def train(self, sess, input_features, input_labels, learning_rate, iteration, summary_rate=250):
        '''Single step training of the autoencoder
        @param sess (tf.Session) the current session
        @param input_feature (np.array) matrix of features
        @param input_labels (np.array) array of labels
        @param learning_rate
        @param lambda_e explicit mixing coefficient
        @param lambda_i implicit mixing coefficient
        @param iteration the current iteration (used for the summary index)
        @param summary_rate summary written at this rate (iterations)
        @return (float) the global loss
        '''
        #if(iteration < 5000): beta = 0.0
        #elif(iteration < 10000): beta = self.beta * 0.5
        #else: beta = self.beta
        #if(iteration < 5000): learning_rate = learning_rate
        #if(iteration < 10000): learning_rate = learning_rate*0.1
        #elif(iteration < 20000): learning_rate = learning_rate*0.01
        beta = self.beta
        _, loss, summ = sess.run([self.train_op, self.loss, self.summaries], 
                                 feed_dict={self.x: input_features, 
                                            self.labels: input_labels, 
                                            self.learning_rate: learning_rate, 
                                            self.beta_placeholder: beta})
        if(self.train_iteration % summary_rate == 0):
            self.tf_summary_writer.add_summary(summ, global_step=self.train_iteration)
            self.tf_summary_writer.flush()
        self.train_iteration = iteration
        return loss

    def save(self, sess, verbose=True):
        '''Save the model
        @param sess (tf.Session) the current session
        @param verbose (bool) if True print information on terminal
        '''
        if not os.path.exists(self.dir_header + "/model/"):
            os.makedirs(self.dir_header + "/model/")
        model_id = strftime("%H%M%S_%d%m%Y", gmtime())
        model_folder = self.dir_header + "/model/" + model_id + "_" + str(self.train_iteration) + "/model.ckpt"
        if(verbose): print("Saving network in: " + str(model_folder))
        save_path = self.tf_saver.save(sess, model_folder)

    def load(self, sess, file_path, verbose=True):
        '''Load a model
        NOTE: when loading a model the method tf.global_variables_initializer()
        must not be called otherwise the variables are set to random values
        @param sess (tf.Session) the current session
        @param path to the model folder, note that the path should end with '/model.ckpt' 
            even though this object does not exists in the path
        @param verbose (bool) if True print information on terminal
        '''
        if(verbose): print("Loading network from: " + str(file_path))
        save_path = self.tf_saver.restore(sess, file_path)
        if(verbose): print("Done!")
