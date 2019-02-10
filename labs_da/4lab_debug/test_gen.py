import sys
import random
import string
import copy
from random import choice
from string import ascii_uppercase
import re

def get_random_key(a, b):
    return ''.join(choice("ABC") for i in range(random.randint(a, b)))

if __name__ == "__main__":
    test_file_name = "tests/{:02d}".format( 1 )
    with open( "{0}.t".format( test_file_name ), 'w' ) as output_file:
        
        a = get_random_key(1,2)
        b = get_random_key(1,2)
        c = get_random_key(1,2)
        pattern = a +" "+ b + " " + c
        output_file.write(pattern)
        output_file.write("\n")

        
        for k in range(10000):
            t = get_random_key(1, 2)
            output_file.write(t)
            output_file.write(" ")
            if k % 10 == 9:
                output_file.write("\n")
            if k % random.randint(10, 100) == 0:
                output_file.write(pattern)
                output_file.write("\n")
            if k % random.randint(10, 100) == 0:
                output_file.write(a)
                output_file.write("\n")
                output_file.write(b + " " + c + " ")
            if k % random.randint(10, 100) == 0:
                output_file.write(a + " " +b)
                output_file.write("\n")
                output_file.write(c + " ")
            
    
    with open( "{0}.t".format( test_file_name ), 'r' ) as output_file, \
    open( "{0}.a".format( test_file_name ), 'w' ) as answer_file:
        text = ""
        cmptext = ""
        first = 0
        second = 1
        count = 0

        for line in output_file:
            for c in line:
                text += c
                if c == "\n":
                    c = " "
                cmptext += c
                if len(text) == len(pattern):
                    if cmptext == pattern:
                        answer_file.write(str(first) + ", " + str(second) + "\n")
                    if text[0] == "\n":
                        first += 1
                        second = 1
                    if text[0] == " ":
                        second += 1
                    text = text[1:]
                    cmptext = cmptext[1:]
                
                 



        
        

        