

# Deep Learning With Python, C++ and Julia


## Basics
- [Deep learning core concepts](resources/deep-learning-core-concepts.md).
- [Deep learning training](resources/deep-learning-training.md).
- [Understanding LSTM networks](resources/understanding_LSTM_networks.md)
- [The Unreasonable Effectiveness of Recurrent Neural Networks](http://karpathy.github.io/2015/05/21/rnn-effectiveness/) shows a bunch of real life examples
- [Deep Learning, NLP, and Representations](http://colah.github.io/posts/2014-07-NLP-RNNs-Representations/) for an overview on word embeddings and RNNs for NLP
- [Understanding LSTM Networks](http://colah.github.io/posts/2015-08-Understanding-LSTMs/) is about LSTMs work specifically, but also informative about RNNs in general
- [Calculus on **Computational Graphs**](http://colah.github.io/posts/2015-08-Backprop/)

More examples:

- [apachecn/MachineLearning](https://github.com/apachecn/MachineLearning)
- [Implementation of Reinforcement Learning Algorithms. Python, OpenAI Gym, Tensorflow](https://github.com/dennybritz/reinforcement-learning)
- [lawlite19/DeepLearning_Python](https://github.com/lawlite19/DeepLearning_Python)
- [A collection of tutorials and examples for solving and understanding machine learning and pattern classification tasks](https://github.com/rasbt/pattern_classification)
- [Deep Learning papers reading roadmap for anyone who are eager to learn this amazing tech](https://github.com/songrotek/Deep-Learning-Papers-Reading-Roadmap)
- [Content for Udacity's Machine Learning curriculum](https://github.com/udacity/machine-learning)
- [This is the lab repository of my honours degree project on machine learning](https://github.com/ShokuninSan/machine-learning)
- [A curated list of awesome Machine Learning frameworks, libraries and software](https://github.com/josephmisiti/awesome-machine-learning)
- [Bare bones Python implementations of some of the fundamental Machine Learning models and algorithms](https://github.com/eriklindernoren/ML-From-Scratch)
- [The "Python Machine Learning" book code repository and info resource](https://github.com/rasbt/python-machine-learning-book)

## Deep Learning with PyTorch

**PyTorch 0.4+** is recommended. 

### 1 - PyTorch basics
* [Offical PyTorch tutorials](http://pytorch.org/tutorials/) for more tutorials (some of these tutorials are included there)
* [PyTorch Basics](pytorch/01-basics/pytorch_basics/main.py)

### 2 - Intermediate
* [Convolutional Neural Network](pytorch/02-intermediate/convolutional_neural_network/main.py#L35-L56)
* [Deep Residual Network](pytorch/02-intermediate/deep_residual_network/main.py#L76-L113)
* [Recurrent Neural Network](pytorch/02-intermediate/recurrent_neural_network/main.py#L39-L58)
* [Bidirectional Recurrent Neural Network](pytorch/02-intermediate/bidirectional_recurrent_neural_network/main.py#L39-L58)
* [Language Model (RNN-LM)](pytorch/02-intermediate/language_model/main.py#L30-L50)

### 3 - Advanced
* [Image Captioning (CNN-RNN)](tutorials/03-advanced/image_captioning)
* [Deep Convolutional GAN (DCGAN)](tutorials/03-advanced/deep_convolutional_gan)
* [Variational Auto-Encoder](tutorials/03-advanced/variational_auto_encoder)
* [Neural Style Transfer](tutorials/03-advanced/neural_style_transfer)
* [Generative Adversarial Networks](pytorch/03-advanced/generative_adversarial_network/main.py#L41-L57)
* [Variational Auto-Encoder](pytorch/03-advanced/variational_autoencoder/main.py#L38-L65)
* [Neural Style Transfer](pytorch/03-advanced/neural_style_transfer)
* [Image Captioning (CNN-RNN)](pytorch/03-advanced/image_captioning)

### 4 - Utilities
* [TensorBoard in PyTorch](pytorch/04-utils/tensorboard)


### More Examples
- [spro/practical-pytorch](https://github.com/spro/practical-pytorch)
- [jcjohnson's PyTorch examples](https://github.com/jcjohnson/pytorch-examples) for a more in depth overview (including custom modules and autograd functions)
- [chenyuntc/pytorch-book](https://github.com/chenyuntc/pytorch-book)



## Deep Learning with TensorFlow

**TensorFlow v2.0**  is recommended. 
- **Hello World** ([notebook](tensorflow_v2/notebooks/1_Introduction/helloworld.ipynb)). Very simple example to learn how to print "hello world" using TensorFlow 2.0.
- **Basic Operations** ([notebook](tensorflow_v2/notebooks/1_Introduction/basic_operations.ipynb)). A simple example that cover TensorFlow 2.0 basic operations.

### 2 - Basic Models
- **Linear Regression** ([notebook](tensorflow_v2/notebooks/2_BasicModels/linear_regression.ipynb)). Implement a Linear Regression with TensorFlow 2.0.
- **Logistic Regression** ([notebook](tensorflow_v2/notebooks/2_BasicModels/logistic_regression.ipynb)). Implement a Logistic Regression with TensorFlow 2.0.
- **Simple Neural Network** ([notebook](tensorflow_v2/notebooks/2_BasicModels/simple_Neural_Network.ipynb)). Implement a Simple Neural Network with TensorFlow 2.0.
- **Create your own layer** ([notebook](tensorflow_v2/notebooks/2_BasicModels/create_custom_layer.ipynb)). Create your own layer with TensorFlow 2.0.

### 3 - Neural Networks

#### Supervised
- **Simple Neural Network** ([notebook](tensorflow_v2/notebooks/3_NeuralNetworks/neural_network.ipynb)). Use TensorFlow 2.0 'layers' and 'model' API to build a simple neural network to classify MNIST digits dataset.
- **Simple Neural Network (low-level)** ([notebook](tensorflow_v2/notebooks/3_NeuralNetworks/neural_network_raw.ipynb)). Raw implementation of a simple neural network to classify MNIST digits dataset.
- **Convolutional Neural Network** ([notebook](tensorflow_v2/notebooks/3_NeuralNetworks/convolutional_network.ipynb)). Use TensorFlow 2.0 'layers' and 'model' API to build a convolutional neural network to classify MNIST digits dataset.
- **Convolutional Neural Network (low-level)** ([notebook](tensorflow_v2/notebooks/3_NeuralNetworks/convolutional_network_raw.ipynb)). Raw implementation of a convolutional neural network to classify MNIST digits dataset.


#### Unsupervised
- **Auto-Encoder** ([notebook](tensorflow_v2/notebooks/3_NeuralNetworks/autoencoder.ipynb)). Build an auto-encoder to encode an image to a lower dimension and re-construct it.
- **DCGAN (Deep Convolutional Generative Adversarial Networks)** ([notebook](tensorflow_v2/notebooks/3_NeuralNetworks/dcgan.ipynb)). Build a Deep Convolutional Generative Adversarial Network (DCGAN) to generate images from noise.


### 4 - Utilities
- **Save and Restore a model** ([notebook](tensorflow_v2/notebooks/4_Utils/save_restore_model.ipynb)). Save and Restore a model with TensorFlow 2.0.
- **Build Custom Layers & Modules** ([notebook](tensorflow_v2/notebooks/4_Utils/build_custom_layers.ipynb)). Learn how to build your own layers / modules and integrate them into TensorFlow 2.0 Models.


## Deap Learning with Julia

Julia: Looks like Python, feels like Lisp, runs like Fortran

### Why julia?

As ML models began to need the full power of a programming language,getting Python to scale to ML’s heavy computational demands is far harder than you might expect. Python’s semantics also make it fundamentally difficult to provide model-level parallelism or compile models for small devices. 

More details:

- [On Machine Learning and Programming Languages](https://julialang.org/blog/2017/12/ml&pl)
- [Why your next project should use Julia](https://speakerdeck.com/eford/why-your-next-project-should-use-julia)


### Julia Basics

 - [Tutorials and information on the Julia language for MIT numerical-computation courses](https://github.com/stevengj/julia-mit)
 - [An Introduction to Julia for Data Science and Scientific Computing](http://ucidatascienceinitiative.github.io/IntroToJulia/)
 - [Introducing Julia wikibook](https://en.wikibooks.org/wiki/Introducing_Julia)
 - [Learn Julia the hard way](https://github.com/chrisvoncsefalvay/learn-julia-the-hard-way)
 - [Hands-on Julia](https://github.com/dpsanders/hands_on_julia)
 - [The-Julia-Express](https://github.com/bkamins/The-Julia-Express)
 - [Julia by Example](https://juliabyexample.helpmanual.io/)
 - [Learn Julia via Examples](https://github.com/scidom/StatsLearningByExample.jl)
 - [Programming in Julia (Quantitative Economics)](https://lectures.quantecon.org/jl/learning_julia.html)


### Deap Learning

- [Machine Learning, Neural Networks, NLP, Speech Recognition and Voice tools](https://github.com/svaksha/Julia.jl/blob/master/AI.md)
- [Julia implementation of the scikit-learn API](https://github.com/cstjean/ScikitLearn.jl)
- [Mocha.jl - Deep Learning framework for Julia](https://github.com/pluskid/Mocha.jl)
- [Merlin.jl - Deep Learning for Julia](https://github.com/hshindo/Merlin.jl)
- [Knet.jl - Koç University deep learning framework](https://github.com/denizyuret/Knet.jl)
- [Julia package for text analysis](https://github.com/JuliaText/TextAnalysis.jl)
- [MXNet Julia Package - flexible and efficient deep learning in Julia](https://github.com/dmlc/MXNet.jl)
- [A Julia wrapper for TensorFlow](https://github.com/malmaud/TensorFlow.jl)
- [Flux is the ML library that doesn't make you tensor](https://github.com/FluxML/Flux.jl)
- [A julia wrapper for Keras](https://github.com/invenia/Keras.jl)


### Resources

- [A Julia cheat sheet](https://github.com/stevengj/1806/blob/master/julia/Julia-cheatsheet.pdf)
- [A Fast Track to Julia: A Quick and Dirty Overview (Cheatsheet)](https://juliadocs.github.io/Julia-Cheat-Sheet/)
- [Curated decibans of Julia language](https://github.com/svaksha/Julia.jl)
- [Julia Observer - A website for browsing Julia packages](https://juliaobserver.com/)
