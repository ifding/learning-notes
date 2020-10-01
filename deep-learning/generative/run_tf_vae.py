# Reference: 
#   https://github.com/mpatacchiola/Y-AE

###
#  unzip ./datasets/mnist.zip -d ./datasets/
###

# This will add the folders `./datasets/mnist/train` and `./datasets/mnist/test` needed for the experiments.


#  cAE:      python run_tf_vae.py --arch="cae"
#  cVAE:     python run_tf_vae.py --arch="cvae"
#  beta-VAE: python run_tf_vae.py --arch="cvae" --beta=2.0
#  AAE:      python run_tf_vae.py --arch="aae"

import os
import numpy as np
import argparse
from dataset import Dataset

def main():
    ##Defining the parser
    parser = argparse.ArgumentParser(description="Tensorflow Trainer")
    parser.add_argument("--resume", type=str, help="resume from checkpoint: ./path/model.ckpt")
    parser.add_argument("--start_iteration", default=0, type=int, help="starting iterations")
    parser.add_argument("--stop_iteration", default=1000, type=int, help="starting iterations")
    parser.add_argument("--epochs", default=100, type=int, help="total epochs")
    parser.add_argument("--gpu", default=0, type=int, help="GPU index")
    parser.add_argument("--arch", default="yae", type=str, help="architecture to use for training: yae, cae")
    parser.add_argument("--implicit_units", default=32, type=int, help="implicit units in the code")
    parser.add_argument("--wdecay", default=0.0, type=float, help="Define the weight decay")
    parser.add_argument("--lrate", default= 0.0001, type=float, help="Learning rate for Adam")
    parser.add_argument("--mini_batch", default=128, type=int, help="mini-batch size")
    parser.add_argument("--lambda_e", default=1.0, type=float, help="Explicit loss mixing coefficient")
    parser.add_argument("--lambda_i", default=1.0, type=float, help="Implicit loss mixing coefficient")
    parser.add_argument("--beta", default=1.0, type=float, help="beta hyperparameter used in beta-VAE")
    args = parser.parse_args()

    #Set the GPU 
    os.environ["CUDA_DEVICE_ORDER"]="PCI_BUS_ID"
    os.environ["CUDA_VISIBLE_DEVICES"]=str(args.gpu)    
    import tensorflow as tf

    #Set global hyperparameters
    learning_rate = args.lrate
    mini_batch_size = args.mini_batch
    tot_epochs = args.epochs
    tot_labels = 10
    dataset_size = 60000
    tot_iterations = int((dataset_size / mini_batch_size) * tot_epochs)
    save_every_iteration = tot_iterations-1
    print_every_iteration = 25
    features_path = "./datasets/mnist/train/features.npy"
    labels_path = "./datasets/mnist/train/labels.npy"
    dataset_train = Dataset()
    dataset_train.load(features_path, labels_path, tot_labels, normalizer=255.0, shuffle=True, verbose=True)
    simulation_path = "./output/" + args.arch + "_ep" + str(args.epochs) + "_wdecay" + str(args.wdecay)    
    
    ##Set local hyperparameters
    if(args.arch=="cae"):
        from models.conv_cae import Autoencoder
        my_net = Autoencoder(channels=1, conv_filters=8, style_size=args.implicit_units, content_size=tot_labels, ksize=(3,3), start_iteration=args.start_iteration, dir_header=simulation_path)
    elif(args.arch=="cvae"):
        from models.conv_cvae import Autoencoder
        my_net = Autoencoder(channels=1, conv_filters=8, style_size=args.implicit_units, content_size=tot_labels, ksize=(3,3), start_iteration=args.start_iteration, dir_header=simulation_path, beta=args.beta)
    elif(args.arch=="aae"):
        from models.conv_aae import Autoencoder
        my_net = Autoencoder(channels=1, conv_filters=4, style_size=args.implicit_units, content_size=tot_labels, ksize=(3,3), start_iteration=args.start_iteration, dir_header=simulation_path) 
    else:
        raise ValueError("[ERROR] The architecture '" + args.arch + "' does not exist!")

    #Init the session
    sess = tf.compat.v1.Session(config=tf.compat.v1.ConfigProto(log_device_placement=True))
    my_net.init_summary(sess)

    if args.resume:
        print("[INFO] Resuming from checkpoint: " + str(args.resume))
        my_net.load(sess, args.resume)
    else:                           
        sess.run(tf.compat.v1.global_variables_initializer()) #WARNING: do not call it when the load() method is used

    print("[INFO] Starting training...")
    for iteration in range(args.start_iteration, tot_iterations):
        input_features, input_labels = dataset_train.return_features_labels(mini_batch_size, onehot=False)
        if(args.arch=="cae"):
            local_loss = my_net.train(sess, input_features, input_labels, 
                                       learning_rate, iteration, print_every_iteration)
        elif(args.arch=="cvae" or args.arch=="aae"):
            local_loss = my_net.train(sess, input_features, input_labels, 
                                       learning_rate, iteration, print_every_iteration)
        else:
            raise ValueError("[ERROR] The architecture '" + args.arch + "' does not exist!")

        if(iteration % print_every_iteration == 0):
            print("Iteration: " + str(iteration) + "/" + str(tot_iterations) + " [" + str(round((iteration/float(tot_iterations))*100.0, 1)) + "%]")
            print("Loss: " + str(local_loss))
            print("")
        if(iteration % save_every_iteration == 0 and iteration!=0):
                my_net.save(sess, verbose=True)


if __name__ == "__main__":
    main()
