#!/usr/bin/env python3

from sys import exit

MAP_SIZE = 20
RESSOURCE_COUNT = 3
PLAYER_COUNT = 2


def inside_map(row, col):
    return 0 <= row < MAP_SIZE and 0 <= col < MAP_SIZE


# Read the grid
grid = [[[None] * RESSOURCE_COUNT for _ in range(MAP_SIZE)] for __ in range(MAP_SIZE)]
plants = [[False] * MAP_SIZE] * MAP_SIZE

for row in range(MAP_SIZE):
    line = input()

    cells = line.split(' ')
    if len(cells) != MAP_SIZE:
        exit(f'La grille n\'est pas de la bonne largeur: {len(line)}')

    for col, cell in enumerate(cells):
        res = cell.split(',')
        if len(res) != RESSOURCE_COUNT:
            exit(f'La case {cell} n\'a pas le bon nombre de ressources')
        for k, r in enumerate(res):
            try:
                grid[row][col][k] = int(r)
            except ValueError:
                exit(f'La case {cell} n\'a pas composée d\'entier')


# Read plants

for player in range(PLAYER_COUNT):
    nb_plants = int(input())
    if nb_plants < 0:
        exit(f'Le nombre de plantes de joueur {player} est négatif')

    for _ in range(nb_plants):
        stats = input().split(' ')
        if len(stats) != 7 + RESSOURCE_COUNT:
            exit(f'La plante {stats} n\'a pas le bon nombre de caractèristiques')

        try:
            row, col, vie, force, pts, mov, grab = list(map(int, stats))[:-RESSOURCE_COUNT]
        except ValueError as e:
            exit('Les caractèristiques d\'une plante ne sont pas des entiers')

        if vie < 0:
            exit(f'La vie d\'une plante doit être positive: {vie}')
        if mov < 0:
            exit(f'Le rayon de mouvement d\'une plante doit être positive: {mov}')
        if grab < 0:
            exit(f'Le rayon de collecte d\'une plante doit être positive: {grab}')

        if not inside_map(row, col):
            exit(f'La plante est hors de la map: {(row, col)}')

        if plants[row][col]:
            exit(f'Une plante est déjà présente: {(row, col)}')
        plants[row][col] = True
