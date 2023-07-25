import numpy as np
import pandas as pd

class BayesianNetwork:
    def __init__(self, n_labels=10, n_pixels=784, n_values=2) -> None:
        '''
        n_labels: number of labels, 10 for digit recognition
        n_pixels: number of pixels, 256 for 16x16 image
        n_values: number of values for each pixel, 0 for black, 1 for white
        '''
        self.n_labels = n_labels
        self.n_pixels = n_pixels
        self.n_values = n_values
        # prior probability
        self.labels_prior = np.zeros(n_labels)
        self.pixels_prior = np.zeros((n_pixels, n_values))
        # conditional probability
        self.pixels_cond_label = np.zeros((n_pixels, n_values, n_labels))
    

    # fit the model with training data
    def fit(self, pixels, labels):
        '''
        pixels: (n_samples, n_pixels, )
        labels: (n_samples, )
        '''
        n_samples = len(labels)
        # TODO: calculate prior probability and conditional probability
        for label in range(self.n_labels):
            self.labels_prior[label] = np.sum(labels == label) / n_samples
            for pixel in range(self.n_pixels):
                for value in range(self.n_values):
                    self.pixels_cond_label[pixel, value, label] = np.sum((pixels[:, pixel] == value) & (labels == label)) / np.sum(labels == label)
        for pixel in range(self.n_pixels):
            for value in range(self.n_values):
                self.pixels_prior[pixel, value] = np.sum(pixels[:, pixel] == value) / n_samples

        

    # predict the labels for new data
    def predict(self, pixels):
        '''
        pixels: (n_samples, n_pixels, )
        return labels: (n_samples, )
        '''
        n_samples = len(pixels)
        labels = np.zeros(n_samples)
        # TODO: predict for new data
        for i in range(n_samples):
            label_probs = np.zeros(self.n_labels)
            for label in range(self.n_labels):
                label_probs[label] = self.labels_prior[label]
                for pixel in range(self.n_pixels):
                    label_probs[label] *= self.pixels_cond_label[pixel, pixels[i, pixel], label]
                for pixel in range(self.n_pixels):
                    label_probs[label] *= self.pixels_prior[pixel, pixels[i, pixel]]
            labels[i] = np.argmax(label_probs)
        return labels
    

    # calculate the score (accuracy) of the model
    def score(self, pixels, labels):
        '''
        pixels: (n_samples, n_pixels, )
        labels: (n_samples, )
        '''
        n_samples = len(labels)
        labels_pred = self.predict(pixels)
        return np.sum(labels_pred == labels) / n_samples


if __name__ == '__main__':
    # load data
    train_data = np.loadtxt('../data/train.csv', delimiter=',', dtype=np.uint8)
    test_data = np.loadtxt('../data/test.csv', delimiter=',', dtype=np.uint8)
    pixels_train, labels_train = train_data[:, :-1], train_data[:, -1]
    pixels_test, labels_test = test_data[:, :-1], test_data[:, -1]
    # build bayesian network
    bn = BayesianNetwork()
    bn.fit(pixels_train, labels_train)
    print('test score: %f' % bn.score(pixels_test, labels_test))