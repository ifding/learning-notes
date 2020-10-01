"""
Tensorflow implementation of Adversarial Autoencoder
    https://arxiv.org/abs/1511.05644
Reference:
    https://github.com/mpatacchiola/Y-AE/blob/master/models/aae.py
Author:
    ifding. Aug.4.2019
"""

import tensorflow as tf
import numpy as np
from time import gmtime, strftime
import os

tf.compat.v1.disable_eager_execution()

class Autoencoder(object):
    '''Adversarial Autoencoder   
    '''
    def __init__(self, channels=1, conv_filters=8, style_size=32, content_size=10, ksize=(3,3), start_iteration=0, dir_header="./", wdecay=0.0):
        '''Init method
        @param sess (tf.Session) the current session
        @param conv_filters_* (int) the number of filters in the convolutional layers
        @param code_size (int) the number of units in the code layer
        @param gradient_clip (bool) applies gradient clipping on the gradient vector
        '''
        self.dir_header = dir_header
        self.start_iteration = start_iteration
        self.channels = channels
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
            
        code_style = self._build_Encoder(self.x)       
        Encoder = tf.keras.Model(inputs=self.x, outputs=code_style) 
        
        self.code_style = tf.squeeze(code_style, axis=[1,2])
        self.code_content_deterministic = tf.one_hot(indices=self.labels, depth=self.content_size)
  
        code = tf.concat([self.code_style, self.code_content_deterministic], axis=1)
        ##Transpose-Convolution-1 -> (2, 2, conv_filters*4)
        code_reshaped = tf.reshape(code, [tf.shape(self.x)[0], 1, 1, self.style_size+self.content_size])        
            
        Decoder = self._build_Decoder()
        self.output = Decoder(code_reshaped, training=True)  
        
        Discriminator = self._build_Discriminator()
        
        #gaussian_input = tf.random.normal([int(batch_size/2), style_size], 0, 1, dtype=tf.float32)
        self.gaussian_input = tf.random.normal([tf.shape(self.x)[0], self.style_size], 0, 1, dtype=tf.float32)
        #style_split, _ = tf.split(self.code_style, [int(batch_size/2), int(batch_size/2)], 0)
        #mixed_input = tf.concat([style_split, gaussian_input], axis=0)
        mixed_input = tf.concat([self.code_style, self.gaussian_input], axis=0)         
        self.discriminator_logits = Discriminator(mixed_input)
        
        encoder_var = Encoder.trainable_variables      
        encoder_decoder_var = Encoder.trainable_variables + Decoder.trainable_variables     
        discriminator_var = Discriminator.trainable_variables
        
        self._set_loss()

        ##Global Loss
        #beta = 1.0
        #self.loss = self.reconstruction_loss + self.loss_adversarial
        ##Train ops
        self.learning_rate = tf.compat.v1.placeholder(tf.float32)
        self.train_op = tf.compat.v1.train.AdamOptimizer(learning_rate=self.learning_rate, 
                                                         beta1=0.9, beta2=0.999, 
                                                         epsilon=1e-08).minimize(
                                                                        self.reconstruction_loss, 
                                                                        var_list=encoder_decoder_var)

        self.discriminator_train_op = tf.compat.v1.train.AdamOptimizer(learning_rate=self.learning_rate, 
                                                                       beta1=0.9, beta2=0.999, 
                                                                       epsilon=1e-08
                                                                      ).minimize(self.discriminator_loss,
                                                                                 var_list=discriminator_var)

        self.generator_train_op = tf.compat.v1.train.AdamOptimizer(learning_rate=self.learning_rate, 
                                                         beta1=0.9, beta2=0.999, 
                                                         epsilon=1e-08).minimize(self.generator_loss, 
                                                                                 var_list=encoder_var)

        ##Saver
        self.tf_saver = tf.compat.v1.train.Saver()
        self.train_iteration = start_iteration       
        
        self._set_summary() 

            
    def _set_loss(self):
        batch_size = tf.shape(self.x)[0]
        ##Reconstruction loss
        self.reconstruction_loss = tf.reduce_mean(tf.square(tf.subtract(self.x, self.output))) #L2 loss
        ##Discriminator loss
        #dummy_target = tf.concat([tf.ones([int(batch_size/2), 1]), 
        #                          tf.zeros([int(batch_size/2), 1])], axis=0)
        #dummy_target = tf.concat([tf.ones([batch_size, 1]), 
        #                          tf.zeros([batch_size, 1])], axis=0)                                      
        #D_real, D_fake = tf.split(self.output_discriminator, [int(batch_size/2), int(batch_size/2)], 0)
        #self.loss_adversarial = -tf.reduce_mean(tf.log(D_real) + tf.log(1.0 - D_fake))
        #self.loss_adversarial = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(labels=dummy_target, logits=self.discriminator_logits)) 
        

        d_fake, d_real =  tf.split(self.discriminator_logits, 2, axis=0)
        lable_fake, lable_real = tf.zeros([batch_size, 1]), tf.ones([batch_size, 1])
        
        #d_fake, d_real = tf.split(self.discriminator_logits, [batch_size, batch_size], 0)
        loss_real = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(labels=lable_real, 
                                                                           logits=d_real))
        loss_fake = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(labels=lable_fake, 
                                                                           logits=d_fake))
        self.discriminator_loss = 0.5 * (loss_fake + loss_real)

        ##Generator loss
        self.generator_loss = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(labels=lable_real,
                                                                                     logits=d_fake))            


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
            conv_5 = tf.keras.layers.Conv2D(filters=self.style_size, 
                                        strides=(2,2), kernel_size=self.ksize,
                                        padding="same", activation=None, 
                                        kernel_regularizer=self.regularizer, 
                                        kernel_initializer=self.weight_initializer_implicit, 
                                        bias_initializer=self.bias_initializer_implicit, 
                                        name="mu")(conv_4)            
          
            return conv_5
        
            
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
    
    def _build_Discriminator(self):
        Discriminator = tf.keras.Sequential([
            tf.keras.layers.Dense(512, activation=None, use_bias=True, 
                                  kernel_regularizer=self.regularizer, 
                                  kernel_initializer=self.weight_initializer),
            tf.keras.layers.LeakyReLU(),
            tf.keras.layers.Dense(1, activation=None, use_bias=True, 
                                  kernel_regularizer=self.regularizer, 
                                  kernel_initializer=self.weight_initializer)           
        ])
        return Discriminator
    
    def _set_summary(self):
        #Summaries
        with tf.compat.v1.variable_scope("Summaries"):
            tf.compat.v1.summary.image("input_images", self.x, max_outputs=8, family="original")
            tf.compat.v1.summary.image("reconstruction_images", self.output, max_outputs=8, family="reconstructed_left")
            #tf.summary.scalar("loss", self.loss, family="_loss_main")
            tf.compat.v1.summary.scalar("reconstruction_loss", self.reconstruction_loss, family="losses_reconstruction")
            tf.compat.v1.summary.scalar("discriminator_loss", self.discriminator_loss, family="losses_discriminator")
            tf.compat.v1.summary.scalar("generator_loss", self.generator_loss, family="losses_generator")
            tf.compat.v1.summary.histogram("hist_style", self.code_style, family="code")
            tf.compat.v1.summary.histogram("hist_gaussian", self.gaussian_input, family="code")                       
            
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
        return loss

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
        _, _, _, loss, summ = sess.run([self.train_op, 
                                        self.discriminator_train_op, 
                                        self.generator_train_op, 
                                        self.reconstruction_loss, self.summaries], 
                                       feed_dict={self.x: input_features, 
                                                  self.labels: input_labels, 
                                                  self.learning_rate: learning_rate})
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
