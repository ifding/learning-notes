import os
import sys
import math
import argparse
import numpy as np
import matplotlib
matplotlib.use('Agg')

from models import VAE, AAE, ConcreateVAE, ConditionalVAE, SparseAE
from utils import load_mnist, plot_features

models = {
    'vae': VAE,
    'aae': AAE,
    'conditional_vae': ConditionalVAE,
    'concrete_vae': ConcreateVAE,
    'sparse_ae': SparseAE
}

def main():
    # Parsing arguments
    parser = argparse.ArgumentParser(description='Training VAEs')
    parser.add_argument('--model', type=str, default='conditional_vae')  
    parser.add_argument('--dataset', type=str, default='mnist')
    parser.add_argument('--epoch', type=int, default=10)
    parser.add_argument('--batch_size', type=int, default=100)
    parser.add_argument('--output', default='output')
    parser.add_argument('--zdims', type=int, default=256)
    parser.add_argument('--gpu', type=int, default=1)
    parser.add_argument('--resume', type=str, default=None)
    parser.add_argument('--testmode', action='store_true')

    args = parser.parse_args()

    # select gpu
    os.environ['CUDA_VISIBLE_DEVICES'] = str(args.gpu)

    # Make output direcotiry if not exists
    if not os.path.isdir(args.output):
        os.mkdir(args.output)

    # Load datasets
    if args.dataset == 'mnist':
        (x_train, y_train), (x_test, y_test) = load_mnist()

    # Construct model
    if args.model not in models:
        raise Exception('Unknown model:', args.model)

    model = models[args.model]()
    
    print(args.model)
    
    if args.model in ['conditional_vae']: # need labels
        model.fit([x_train, y_train], num_epochs=args.epoch, batch_size=args.batch_size)
        
        # display a 2D plot of the digit classes in the latent space
        z_test = model.extract_features(x_test, y_test, batch_size=100)
        plot_features(z_test, y_test)
        
        model.plot()
        
    else:
        # vae, aae, concrete_vae, sparse_ae
        model.fit(x_train, num_epochs=args.epoch, batch_size=args.batch_size)

        # display a 2D plot of the digit classes in the latent space
        z_test = model.extract_features(x_test, batch_size=100)
        plot_features(z_test, y_test)
    
    



if __name__ == '__main__':
    main()