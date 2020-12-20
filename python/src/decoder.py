from .encoder import Encode_line, get_position, RM_encoder, get_rev_position
from .utils import count_k
import numpy as np
import itertools

def point_from_position (result: np.ndarray, position: int):
    for i in range(len(result)):
        val = 2**(len(result) - i - 1)
        if position >= val:
            result[i] = True
            position -= val
        else:
            result[i] = False
    return result


def point_from_rev_position (result: np.ndarray, position: int):
    for i in range(len(result)):
        val = 2**(len(result) - i - 1)
        if position >= val:
            result[len(result) - i - 1] = True
            position -= val
        else:
            result[len(result) - i - 1] = False
    return result

class RM_decoder():
    def __init__ (self: object, r: int, m: int, encoder: RM_encoder):
        self.r = r
        self.m = m
        self.encoder = encoder
    def decode (self: object, encode: Encode_line,  astype:type = np.int):
        recur_r = self.r
        result = []
        
        while recur_r >= 0:
            self.__recur_decode(encode, recur_r, result)
            recur_r -= 1
            
        arg_to_index = self.encoder.generator.monom_to_index
        k = count_k(self.r, self.m)
        numpy_result = np.zeros(k, dtype=np.bool)
        #print(result)
        for arg_coef in result:
            if arg_coef[1] == 0:
                continue
            numpy_result[arg_to_index[arg_coef[0]]] = True
        return numpy_result.astype(astype)
    def __recur_decode (self: object, encode: Encode_line, recur_r: int, result :list):
        
        b_points = np.zeros(self.m - recur_r, dtype = np.bool)
        x_points = np.zeros(recur_r, dtype = np.bool)

        full_points = np.zeros(self.m, dtype = np.bool)
        
        fix_arg_values = np.zeros(2, dtype = np.int)
        for monom_arg in itertools.combinations(np.arange(self.m), recur_r):
            monom_arg_mask = np.zeros(self.m, dtype = np.bool)
            for arg in monom_arg:
                monom_arg_mask[arg] = True

            fix_b = 0
            fix_x = 0

            fix_arg_values[0], fix_arg_values[1] = 0, 0
            for fix_b in range(2**(self.m - recur_r)):
                b_points = point_from_position (b_points, fix_b)
                fix_b_value = 0
                for fix_x in range(2**recur_r):
                    x_points = point_from_position (x_points, fix_x)
                    b_i = 0
                    x_i = 0
                    for mask_i, mask_value in enumerate(monom_arg_mask):
                        if mask_value == False:
                            full_points[mask_i] = b_points[b_i]
                            b_i += 1 
                        else:
                            full_points[mask_i] = x_points[x_i]
                            x_i += 1
                    fix_b_value += int(encode.get_rev_value(full_points))
                fix_arg_values [fix_b_value % 2] += 1
            if fix_arg_values[1] > fix_arg_values[0]:
                result.append((monom_arg, 1))
                for i in range(2**self.m):
                    full_points = point_from_rev_position (full_points, i)
                    change_flag = True
                    for mask_i, mask_value in enumerate(monom_arg_mask):
                        if mask_value == True and full_points[mask_i] != True:
                            change_flag = False
                            break
                    if change_flag:
                        encode.data[i] = not encode.data[i]
            else:
                result.append((monom_arg, 0))