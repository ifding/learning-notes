#The MIT License (MIT)
#Copyright (c) 2019 anonymous authors
#
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
#MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
#CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
#SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

import numpy as np
import os

class Dataset():

    def __init__(self, features_path=None, labels_path=None, tot_labels=0, verbose=True):
        if(features_path is not None and labels_path is not None):
            if(tot_labels<=0):
                raise ValueError("[ERROR] Please specify a valid value for 'tot_labels'")
            self.load(features_path, labels_path, tot_labels, verbose=verbose)
        else:
            self.size = 0
            self.tot_labels = 0
            self.features = None
            self.labels = None

    def split_portion(self, features_path, labels_path, first_output_path="./train", second_output_path="./test", split_portion=0.30, verbose=True):
        '''Given two numpy files, one with features and the other with labels,
        it generates 4 files with features and labels based on a portion.
        This is useful to divide the dataset in training and test set.
    
        @param: features_path
        @param: labels_path
        '''
        ##Load
        if(os.path.isfile(features_path)==False or os.path.isfile(labels_path)==False):
            raise ValueError("[ERROR] The features or labels files does not exist!")
        features_array = np.load(features_path)
        labels_array = np.load(labels_path)
        dataset_size = labels_array.shape[0]
        ##Get random test samples
        indices_table = np.arange(dataset_size)
        np.random.shuffle(indices_table)
        test_size = dataset_size - int(dataset_size * split_portion)
        test_indices = indices_table[0:test_size]
        train_indices = indices_table[test_size:]
        train_features_array = features_array[train_indices,:,:,:]
        train_labels_array = labels_array[train_indices,:]
        test_features_array = features_array[test_indices,:,:,:]
        test_labels_array = labels_array[test_indices,:]
        ##Save
        if(verbose): print("Saving datasets...")
        if not os.path.exists(first_output_path+"/"): os.makedirs(first_output_path+"/")
        if not os.path.exists(second_output_path+"/"): os.makedirs(second_output_path+"/")
        np.save(first_output_path + "/features", train_features_array)
        np.save(first_output_path + "/labels", train_labels_array)
        np.save(second_output_path + "/features", test_features_array)
        np.save(second_output_path + "/labels", test_labels_array)
        ##Print
        if(verbose): print("Test size: " + str(test_indices.shape)) #26366
        if(verbose): print("Train size: " + str(train_indices.shape)) # 60000
        if(verbose): print("Done!")        

    def split_chunks(self, features_path, labels_path, output_path, tot_chunks, shuffle=True, verbose=True):
        ##Load
        if(os.path.isfile(features_path)==False or os.path.isfile(labels_path)==False):
            raise ValueError("[ERROR] The features or labels files does not exist!")
        features_matrix = np.load(features_path).astype(np.uint8)
        labels_matrix = np.load(labels_path)
        rows = features_matrix.shape[0]
        ##Shuffle
        if(shuffle):
            indices = np.random.permutation(rows)
            if(verbose): print("Shuffling...")
            features_matrix = features_matrix[indices]
            labels_matrix = labels_matrix[indices]
            if(verbose): print("Done!")
        ##Split
        features_list = np.split(features_matrix, tot_chunks)
        labels_list = np.split(labels_matrix, tot_chunks)
        ##Save
        for i in range(tot_chunks):
            if(verbose): print("Saving part: " + str(i))
            np.save(output_path + "/features_" + str(i), features_list[i])
            np.save(output_path + "/labels_" + str(i), labels_list[i])

    def load(self, features_path, labels_path, tot_labels, normalizer=1.0, shuffle=True, verbose=True):
        if(os.path.isfile(features_path)==False or os.path.isfile(labels_path)==False):
            raise ValueError("[ERROR] The features or labels files does not exist!")   
        self.features = np.load(features_path) / normalizer
        self.labels = np.load(labels_path)
        self.size = self.features.shape[0]
        self.tot_labels = tot_labels
        ##Shuffle
        if(shuffle):
            indices = np.random.permutation(self.size)
            if(verbose): print("Shuffling...")
            self.features = self.features[indices]
            self.labels = self.labels[indices]
            if(verbose): print("Done!")

    def load_chunk(self, chunk_index, tot_labels, root=".", normalizer=1.0, verbose=True):
        features_path = root + "/features_" + str(chunk_index) + ".npy"
        labels_path = root + "/labels_" + str(chunk_index) + ".npy"
        if(os.path.isfile(features_path)==False or os.path.isfile(labels_path)==False):
           raise ValueError("[ERROR] The features or labels files does not exist!")
        self.features = np.load(features_path) / normalizer
        self.labels = np.load(labels_path)
        self.size = self.features.shape[0]
        self.tot_labels = tot_labels
        if(verbose): print("Loading chunk " + str(chunk_index) + " from: " + str(root))

    def return_features_labels(self, batch_size, onehot=True, shuffle=True):
        if(shuffle): indices = np.random.randint(0, self.size, size=batch_size)
        else: indices = np.arange(0, batch_size)
        if(onehot):
            values = (self.labels[indices]).astype(np.int32)
            labels = np.zeros([batch_size, self.tot_labels], dtype=np.float32)
            arange = np.arange(batch_size)
            labels[arange, values] = 1.0       
            return self.features[indices], labels
        else:
            return self.features[indices], self.labels[indices]


