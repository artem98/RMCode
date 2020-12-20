import numpy as np
from .utils import Polynomial, G_matrix
from dataclasses import dataclass
from copy import copy

def get_position(point):
    return sum(2**(len(point) - i - 1) for i, bit in enumerate(np.array(point).astype(np.bool)) if bit)

def get_rev_position(point):
    return sum(2**(i) for i, bit in enumerate(np.array(point).astype(np.bool)) if bit)

@dataclass
class Encode_line:
    data: np.ndarray
    polynomial: Polynomial

    def update(self):
        self.polynomial.update()
        return self
    
    def __str__(self):
        if self.polynomial.name == "":
            return "{}".format(str(self.data.astype("int"))[1:-1])
        return "{} #|# {}".format(str(self.data.astype("int"))[1:-1], self.polynomial.name)
    
    def __repr__(self):
        return self.__str__()       

    def get_value(self, point):
        return self.data[get_position(point)]
    
    def get_rev_value(self, point):
        return self.data[get_rev_position(point)]
    
    def copy(self):
        data_copy = self.data.copy()
#         copy_line.data = data_copy
#         copy_line.polynomial = self.polynomial
        copy_line = Encode_line(data = data_copy, polynomial = self.polynomial)
        return copy_line
    def reset_name(self):
        self.polynomial.name = ""
   

@dataclass
class Noise_line (Encode_line):
    def __init__ (self: object, encode_line: Encode_line, noise_num: int):
        self.data = noise(encode_line.data, noise_num)
        self.polynomial = encode_line.polynomial
        self.polynomial.name = "?"


def noise(array:np.ndarray, noise_num:int):
    array_l = len(array)
    if array_l < noise_num:
        print("Error! Array less than noise_num")
        return
    choice = np.random.choice(len(array), noise_num, replace = False)
    result_array = array.copy()
    for i in choice:
        result_array[i] = not result_array[i]
    return result_array

# test = np.random.choice(a=[False, True], size=(10, ))
# print(test, noise(test, 2))

class RM_encoder():
    def __init__ (self: object, r: int, m: int):
        self.generator = G_matrix.truncate(r, m)
        self.r = r
        self.m = m
        self.t = 2**(self.m - self.r - 1) - 1
    def encode (self, input):
        input_as_bool = np.array(input).astype(np.bool)

        data_numeric = np.dot(input_as_bool.astype(np.int), self.generator.data.astype(np.int))
        data = np.zeros(self.generator.n, dtype = np.bool)
        for i, val in enumerate(data_numeric):
            if (data_numeric[i] % 2) == 1:
                data[i] = True

        monomials = []
        for i, bit in enumerate(np.array(input).astype(np.bool)):
            if bit:
                monomials.append(self.generator.monomials[i])
        return Encode_line(data, Polynomial(monomials)).update()
    
    def encode_with_noise (self, input, noise_num: int):
        encode_line = self.encode(input)
        if noise_num > self.t:
            print("Warning! Max noise is {}".format(self.t))
        
        return Noise_line(encode_line, noise_num)