import random

a = 1
while( a != 0):
    a = random.random()
    print(a) 
    print('{0:.20f}'.format(a * 18446744073709551615))
    i = input()