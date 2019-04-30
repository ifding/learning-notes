
## Practical

* [Decision trees](./decision-trees.md)
* [What do you mean by 1D, 2D and 3D Convolutions in CNN?](./1d-2d-and-3d-convolutions-in-cnn.md)
* [Topics Course on Deep Learning UC Berkeley](https://github.com/joanbruna/stat212b)

## Notes

### Tensor

Numpy is a great framework, but it cannot utilize GPUs to accelerate its numerical computations. 

For modern deep neural networks, GPUs often provide speedups of 50x or greater, so unfortunately numpy won’t be enough for modern deep learning.

A PyTorch Tensor is conceptually identical to a numpy array: a Tensor is an n-dimensional array, and PyTorch provides many functions for operating on these Tensors.


### Autograd

When using autograd, the forward pass of your network will define a **computational graph**; nodes in the graph will be Tensors, and edges will be functions that produce output Tensors from input Tensors. 

Backpropagating through this graph then allows you to easily compute gradients.

Each Tensor represents a node in a computational graph. 

If `x` is a Tensor that has `x.requires_grad=True` then `x.grad` is another Tensor holding the gradient of `x` with respect to some scalar value.


### Static Graphs vs Dynamic Graphs

TensorFlow’s computational graphs are static and PyTorch uses dynamic computational graphs.

In TensorFlow, we define the computational graph once and then execute the same graph over and over again, possibly feeding different input data to the graph. 

In PyTorch, each forward pass defines a new computational graph.


## Neural Networks

1. A typical training procedure for a neural network is as follows:

* Define the neural network that has some learnable parameters (or weights)
* Iterate over a dataset of inputs
* Process input through the network
* Compute the loss (how far is the output from being correct)
* Propagate gradients back into the network’s parameters
* Update the weights of the network, typically using a simple update rule (SGD): `weight = weight - learning_rate * gradient`


2. You just have to define the forward function, and the backward function (where gradients are computed) is automatically defined for you using autograd.

3. The entire `torch.nn` package only supports inputs that are a mini-batch of samples, and not a single sample.If you have a single sample, just use `input.unsqueeze(0)` to add a fake batch dimension.

For example, `nn.Conv2d` will take in a 4D Tensor of `nSamples x nChannels x Height x Width`.

4. if you follow loss in the backward direction, using its `.grad_fn` attribute, you will see a graph of computations that looks like this:

```
input -> conv2d -> relu -> maxpool2d -> conv2d -> relu -> maxpool2d
      -> view -> linear -> relu -> linear -> relu -> linear
      -> MSELoss
      -> loss
```

## Data Loading and Processing

Generally, when you have to deal with image, text, audio or video data, you can use standard python packages that load data into a numpy array. Then you can convert this array into a `torch.*Tensor`.

* For images, packages such as Pillow, OpenCV are useful
* For audio, packages such as scipy and librosa
* For text, either raw Python or Cython based loading, or NLTK and SpaCy are useful