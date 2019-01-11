import sys
import random
import string
import copy
from random import choice
from string import ascii_uppercase

def get_random_key(a, b):
    return ''.join(choice("ABC") for i in range(random.randint(a, b)))

if __name__ == "__main__":
    test_file_name = "tests/{:02d}".format( 1 )
    with open( "{0}.t".format( test_file_name ), 'w' ) as output_file, \
        open( "{0}.a".format( test_file_name ), 'w' ) as answer_file:

        pattern = get_random_key(2, 2)
        output_file.write(pattern)
        output_file.write(" ")
        pattern = get_random_key(2, 2)
        output_file.write(pattern)
        output_file.write("\n")

        
        for k in range(100000):
            t = get_random_key(2, 2)
            output_file.write(t)
            output_file.write(" ")
            if k % 10 == 0:
                output_file.write("\n")
        

        