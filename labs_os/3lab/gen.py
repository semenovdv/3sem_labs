import random

if __name__ == "__main__":
    with open("test.txt", 'w') as out:
        out.write("{0} ".format(100000))
        for i in range(100000):
            N = random.randint(0, 100)
            out.write("{0} ".format(N))