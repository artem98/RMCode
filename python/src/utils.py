import numpy as np
from dataclasses import dataclass
from typing import List
from scipy.special import comb

def get_position(point):
    return sum(2**(len(point) - i - 1) for i, bit in enumerate(np.array(point).astype(np.bool)) if bit)

@dataclass
class Monomial:
    name: str
    deg: int
    args: tuple
    
@dataclass
class Polynomial:
    name: str
    deg: int
    monomials: List[Monomial]

    def __init__ (self, monomials):
        self.monomials = monomials

    def update(self):
        sorted(self.monomials, key = lambda monom: monom.deg, reverse = True)
        self.deg = -1 if len(self.monomials) == 0 else max(self.monomials[0].deg, self.monomials[-1].deg)
        self.name = ""
        for i, monom in enumerate(self.monomials):
            self.name += monom.name if i == 0 else " + " + monom.name

def count_k(r: int, m: int):
    return sum(comb(m, i, exact = True) for i in range(r + 1))

class G_matrix():
    def __init__ (self:object, shape: tuple):
        '''
        shape = (k, m) 
        n == 2**m
        '''

        self.k = shape[0]
        self.m = shape[1]
        self.n = 2**shape[1]

        self.data = np.zeros((self.k, self.n),  dtype=np.bool)
        self.monomials = np.ndarray(self.k, dtype=Monomial)
        self.monom_to_index = {}
    
#     def sort (self):
#         sort_index = np.arange(self.k)
#         sorted(sort_index, key = lambda i: get_position(self.monomials[i].args))
#         new_data = np.zeros((self.k, self.n),  dtype=np.bool)
#         new_monomials = np.ndarray(self.k, dtype=Monomial)
#         new_monom_to_index = {}
#         for i, new_i in enumerate(sort_index):
#             new_data[i] = self.data[new_i]
#             new_monomials[i] = self.monomials[new_i]
#         self.data = new_data
#         self.monomials = new_monomials
#         return self
    
    def __str__ (self):
        result = ""
        for i in range(len(self.data)):
            result += "{} #|# {}\n".format(str(self.data[i].astype("int"))[1:-1], self.monomials[i].name)
        return result
    def __repr__ (self):
        return self.__str__()
    
    def __recur_init(self):
        recur_m = 0

        self.monomials[0] = Monomial('', 0, tuple())
        self.monom_to_index[tuple()] = 0
        self.data[0][0] = True

        def copy_G_m(dst: tuple):
            for i in range(2**recur_m):
                for j in range(2**recur_m):
                    self.data[dst[0] + i][dst[1] + j] = self.data[i][j]
        def copy_and_update_info():
            for i in range(2**recur_m):
                new_args = tuple([j for j in self.monomials[i].args] + [recur_m])
                self.monomials[2**recur_m + i] = Monomial(self.monomials[i].name + "x{}".format(recur_m), self.monomials[i].deg + 1, new_args)
                self.monom_to_index[new_args] = 2**recur_m + i

        while recur_m < self.m:
            copy_G_m((0, 2**recur_m))
            copy_G_m((2**recur_m, 2**recur_m))
            copy_and_update_info()
            recur_m += 1
        
        self.monomials[0].name = '1'
        return self

    @classmethod
    def full(cls:type, m: int):
        return cls(shape = (2**m, m)).__recur_init()

    @classmethod
    def truncate(cls:type, r: int, m: int):
        full_G = cls.full(m)

        if r == m:
            return full_G

        k = count_k(r, m)
        truncated_G = cls(shape = (k, m))
        j = 0
        for i in range(2**m):
            if full_G.monomials[i].deg <= r:
                truncated_G.data[j] = full_G.data[i]
                truncated_G.monomials[j] = full_G.monomials[i]
                truncated_G.monom_to_index[full_G.monomials[i].args] = j
                j += 1
            if j == k:
                break
        return truncated_G
