# Generative Models

Tensorflow 2.0 implementation of generative models, e.g. VAE, GAN


## Tutorial

- [Tutorial: Deriving the Standard Variational Autoencoder (VAE) Loss Function (2019)](https://arxiv.org/pdf/1907.08956.pdf)
- [An Introduction to Variational Autoencoders (2019)](http://arxiv.org/abs/1906.02691)
- [Tutorial on Variational Autoencoders (2016)](http://arxiv.org/abs/1606.05908)
- [Implementing Variational Autoencoders in Keras: Beyond the Quickstart Tutorial](http://louistiao.me/posts/implementing-variational-autoencoders-in-keras-beyond-the-quickstart-tutorial/)
- [Gaussian mixture VAE: lessons in Variational Inference, Generative models, and Deep Nets](http://ruishu.io/2016/12/25/gmvae/)

## Implementation

- Conditional convolutional auto-encoder, [tensorflow code](./models/conv_cae.py)
- [Auto-Encoding Variational Bayes (2013)](https://arxiv.org/pdf/1312.6114.pdf), [keras code](./models/vae.py)
- [Conditional variational auto-encoder (CVAE) (2015)](https://papers.nips.cc/paper/5775-learning-structured-output-representation-using-deep-conditional-generative-models.pdf), [keras code](./models/conditional_vae.py), [tensorflow code](./models/conv_cvae.py)
- [Adversarial autoencoders (2015)](https://arxiv.org/abs/1511.05644), [keras code](./models/aae.py), [tensorflow code](./models/conv_aae.py)
- [Sparse autoencoder (Andrew Ng)](https://web.stanford.edu/class/cs294a/sparseAutoencoder.pdf), [keras code](./models/sparse_ae.py)
- Variational Auto Encoder with a Concrete latent distribution, [The Concrete Distribution: A Continuous Relaxation of Discrete Random Variables](https://arxiv.org/abs/1611.00712) or [Categorical Reparameterization with Gumbel-Softmax](https://arxiv.org/abs/1611.01144), [keras code](./models/concrete_vae.py)

## Other's Collections

- [pytorch-generative-model-collections](https://github.com/znxlwm/pytorch-generative-model-collections)
- [Generative Models Tutorial with Demo](https://github.com/omerbsezer/Generative_Models_Tutorial_with_Demo)
- [GauravBh1010tt/DL-Seq2Seq](https://github.com/GauravBh1010tt/DL-Seq2Seq)
- [The implementation of the VAE and Gaussian Mixture VAE](https://github.com/psanch21/VAE-GMVAE)
- [Unsupervised clustering with (Gaussian mixture) VAEs](https://github.com/RuiShu/vae-clustering)

## Some Papers

### VAE

> Sequence

- [Generating Sentences from a Continuous Space (2015)](https://arxiv.org/pdf/1511.06349.pdf)
- [Disentangled Sequential Autoencoder (2018)](https://arxiv.org/abs/1803.02991)


### GAN

- Generative models are also promising in the long term future because it has a potential power to learn the natural features of a dataset automatically.

- Generative models are mostly used to generate images (vision area). In the recent studies, it will also used to generate sentences (natural language processing area).

> Image

- [Generative Adversarial Networks (2014)](https://arxiv.org/abs/1406.2661)
- [Conditional GANs (2014)](https://arxiv.org/abs/1411.1784)
- [DCGAN (2015)](https://arxiv.org/abs/1511.06434)
- [Improved Techniques for Training GANs (2016)](https://arxiv.org/abs/1606.03498)
- [Pix2Pix (2016)](https://arxiv.org/abs/1611.07004)
- [StackGAN (2016)](https://arxiv.org/abs/1612.03242)
- [CycleGAN (2017)](https://arxiv.org/abs/1703.10593)
- [Improved Training of Wasserstein GANs (2017)](https://arxiv.org/abs/1704.00028)
- [Progressively Growing GANs (2017)](https://arxiv.org/abs/1710.10196)
- [BigGAN (2018)](https://arxiv.org/abs/1809.11096)
- [StyleGAN (2018)](https://arxiv.org/abs/1812.04948)

> Sequence

- [SeqGAN: Sequence Generative Adversarial Nets with Policy Gradient (2016)](https://arxiv.org/abs/1609.05473)


### Representation Learning

- [A Neural Representation of Sketch Drawings (2017)](https://arxiv.org/abs/1704.03477)
- [Deep InfoMax (2018)](https://arxiv.org/abs/1808.06670)
- [Neural Discrete Representation Learning (2017)](https://arxiv.org/pdf/1711.00937.pdf)

