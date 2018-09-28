import random
import string
from decimal import Decimal

MAX_KEY_VAL = 18446744073709551615
MAX_VAL_LEN = 10

def generate_random_value():
    return "".join([random.choice(string.ascii_letters) for _ in range(1, MAX_VAL_LEN)])

if __name__ == "__main__":
    for num in range(1, 20):
        values = list()
        output_filename = "tests/{:02d}.t".format(num)
        with open( output_filename, 'w') as output:
            for _ in range( random.randint( 10000, 20000)):
                key = random.random() * MAX_KEY_VAL/2
                value = generate_random_value()
                values.append( (key, value) )
                output.write( "{0:.0f}\t{1:}\n".format( key, value ) )
        
        output_filename = "tests/{:02d}.a".format( num )
        with open( output_filename, 'w') as output:
            values = sorted( values, key=lambda x: x[0] )
            for value in values:
                output.write( "{0:.0f}\t{1:}\n".format(value[0], value[1]) )   