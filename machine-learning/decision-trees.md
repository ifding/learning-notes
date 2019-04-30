

========================

Classification and regression
-----------------------------

In supervised learning, you are presented with instances (e.g. images of individuals) with labels (e.g. "BOY" or "GIRL") as training data. The task is to label new unlabeled instances (assuming the instance has the sufficient information).

* **Classification** -- labels are discrete values (often finite, often true or false).
  * labels (codomain of the target function) have no meaningful order.
* **Regression** -- labels are reals.
  * labels have a meaningful order.


Classification learning
-----------------------

* **Instances** -- inputs (vectors of features).
* **Concept** -- a function which maps instances to labels (there many concepts: |labels|^|instance space|).
  * Concepts are the set of functions mapping from the instance space to the labels space.
* **Target concept** -- the function which maps instances to the **correct** labels.
  * The target concept is a specific concept which we wish to model.
* **Hypothesis** -- set of concepts which we are willing to search for the best approximation of the target concept.
  * Subset of the concepts set. Easier space to search through, but introduces *inductive bias*.
* **Sample (training set)** -- set of inputs with correct labels.
* **Candidate** -- the "best" concept chosen from the hypothesis by the learning algorithm using the sample.
  * Element of hypothesis which best approximates the target concept according to our learning algorithm.
* **Testing set** -- set of instances with correct labels, similar to the training set, but used to measure how well the candidate performs on novel data.

The testing set should contain many examples not found in the training set.

Decision trees
--------------

A method for approximating discrete-valued target functions, the learned function is represented by a decision tree.

Decision trees represent a disjunction of conjunctions of constraints on the attribute values of instances. Each path from root to a leaf corresponds to a conjunction of attribute tests, and the tree itself to a disjunction of these conjunctions.

Sequence of tests (path of nodes starting from a root node) applied to every instance in order to arrive at its label (leaf).

_which attribute should be tested at the root of the tree?_

Each instance attribute is evaluated using a statistical test, select the best attribute, create a descendant of root node, and sort the training examples.

This forms a greedy search for an acceptable decision tree, in which the algorithm never backtracks to reconsider earlier choices.


Decision trees: learning
------------------------

1. Pick the best attribute to split the data.
2. Asked test every possible value of the attribute.
3. Follow the correct answer path.
4. Go to 1 until the possibilities has been narrowed to one answer.

How can this intuition be used to build a "tree" for all possible instances of the problem?

Decision trees: expressiveness
------------------------------

Consider the inclusive disjunction: **OR(a1, a2, ..., an)** (any). Note that the tree is "linear" and has a height of n.
```
--->a1--T-->a2--T-->...--T-->an--T-->TRUE
        |       |                |
        F       F                F
        |       |                |
        v       v                v
      FALSE   FALSE            FALSE
```

Next, consider the exclusive disjunction: **XOR(a1, a2, ..., an)** (odd parity). Note that the tree is balanced and has height O(2^n).

```
--->a1--T-->a2--T-->a3--T-->TRUE
     |       |       |
     |       |       F----->FALSE
     |       |
     |       F----->a3--T-->FALSE
     |               |
     |               F----->TRUE
     |   
     F----->a2--T-->a3--T-->FALSE
             |       |
             |        F---->TRUE
             |
             F----->a3--T-->FALSE
                     |
                     F----->TRUE
```
It's better just to add in integers mod 2.

Decision trees: expressiveness (search space)
---------------------------------------------

Given n binary attributes, how many possible decision trees are there? 2^(2^n)

Intuition:
* There are 2^n possible configurations of the attributes.
* Each "unique" decision tree maps these 2^n configurations into a 2^n-sized bit vector.
* There are 2^(2^n) possible bit vectors of size 2^n.
* Therefore, there must are 2^(2^n) possible unique classifiers.
* Note that more than one tree may map to a single classifier, so the hypothesis space is even larger (thanks to inductive bias, we can cut down the problem significantly).

ID3 Algorithm
-------------

Information Gain, that measures how well a given attribute separates the training examples according to their target classification. 

Information theory, **entropy**, that characterizes the (im)purity of an arbitrary collection of examples.

The **best attribute** is that one with the greatest information gain.

The **best attribute** is the one that splits the data into subsets whose entropies' weighted sum is the least (maximizing the information gain).

```
A <- best attribute from remaining attributes (initially, all attributes)
Assign A as decision attribute for Node
For each value of A, create a new descendant of Node
Sort training examples to leaves
If examples are perfectly classified, STOP.
Else if we ran out of attributes, STOP
Else, start over for each leaf (with corresponding set of training examples)
```

The process of selecting a new attribute and partitioning the training examples is now repeated for each nonterminal descendant node, until either of two conditions is met: (1) every attribute has already been included along this path through the tree, or (2) the training examples associated with this leaf node all have the same target attribute vlaue (i.e., their entropy is zero).

The hypothesis space searched by ID3 is the set of possible decision trees.

- ID3's hypothesis space of all decision trees is a complete space of finite discrete-valued functions.
- ID3 maintains only a single current hypothesis, Candidate-Elimination method, which maintains the set of all hypotheses consistent with the available training examples. ID3 loses the capabilities that represent all consistent hypotheses.
- ID3 in its pure form performs no backtracking in its search: converging to locally optimal solutions that are not globally optimal.
- ID3 uses all training examples at each step in the search to make decisions incrementally, Find-S or Candidate-Elimination is based on individual training examples. The resulting search is much less sensitive to errors in individual training examples.

**inductive bias**: The set of assumptions that, together with the training data, deductively justify the classifications assigned by the learner to future instances.

Describing the **inductive bias** of ID3 consist of describing the basis by which it chooses one of these consistent hypotheses over the others.

ID3 chooses the first acceptable tree it encounters in its simple-to-complex, hill-climbing search through the space of possible trees.

**A closer approximation to the inductive bias** of ID3: 
- Shorter trees are preferred over longer trees. 
- Trees that place high information gain attributes close to the root are preferred over those that do not.


Restriction Biases and Preference Biases
--------------------------------------

ID3
  - searches a complete hypothesis space (one capable of expressing any finite discrete-valued function).
  - It searches incompletely through this space,
  - from simple to complex hypotheses, until its termination condition is met,
  - e.g., until it finds a hypothesis consistent with the data.
  - Its inductive bias is solely a consequence of the **ordering of hypotheses** by its search strategy.
  - Its hypothesis space introduces no additional bias.
  - The inductive bias of ID3 follows from its search strategy.
  - **preference bias**, a preference for shorter hypotheses, with no hard restriction

Candidate-Elimination
  - searches an incomplete teachable space (one that can express only a subset of the potentially teachable concepts).
  - It searches this space completely,
  - finding every hypothesis consistent with the training data.
  - Its inductive bias is solely a consequence of the **expressive power** of its hypothesis representation.
  - Its search strategy introduces no additional bias.
  - Its inductive bias follows from the definition of its search space.
  - **restriction bias**, a categorical restrication on the set of hypotheses considered.
  
preference bias vs restriction bias
  - a preference bias is more desirable than a restriction bias
    - it allows the learner to work within a complete hypothesis space that is assured to contain the unknown target function.
  - a restriction bias that strictly limits the set of potential hypotheses
    - it introduces the possibility of excluding the unknown target function altogether.


Decision trees: other considerations
------------------------------------

* How do handle continuous attributes?
  * Use intervals
    * Split age range 0-90 into 0-40 and 40-90 -- perhaps even use a modified ID3 to find the best splitting age.
* Does it make sense to repeat an attribute along a path in the tree?
  * No for finite-valued attributes.
  * However, continuous attributes can be tested with different questions
  * No question needs to be asked twice.
* When do we stop?
  * Everything classified correctly (or nearly correct -- we do not want to **overfit**).
  * No more attributes.
  * Do not **overfit**.
    * Try not to have a tree which is too big.
    * Try many trees and cross-validation.
    * Variant of cross-validation where you hold out a subset of the data and build a tree breadth-first on the remaining data. Stop when error is "low enough."
    * Build the whole tree and prune (vote if the classification is not perfect).
* Regression
  * Model output and group them (round off or cluster).
  * Report average on leaves, or vote, or locally fit a line (hybrid).


Decision trees
--------------

We learned:
* Representation (tree... set of questions)
* ID3: a top down learning algorithm
* Expressiveness of DTs
* Bias of ID3
* "Best" attribute Gain(S, A)
* Dealing with overfitting.