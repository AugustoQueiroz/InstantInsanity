import random
import argparse

parser = argparse.ArgumentParser('Gerador de instâncias com n cubos (e n cores) para o problema da insanidade instantânea')
parser.add_argument('-n', type=int, required=True, help='O número de cubos/cores da instância.')

n = parser.parse_args().n

colors = [i for i in range(n)]

sides = []
for _ in range(4):
    random.shuffle(colors)
    sides.append(colors[:])

print(n)
for i in range(n):
    print(f'{sides[0][i]} {sides[2][i]}')
    print(f'{sides[1][i]} {sides[3][i]}')
    print(f'{random.choice(colors)} {random.choice(colors)}')