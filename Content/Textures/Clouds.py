import png
import time
from opensimplex import OpenSimplex

def gen(new_file_name):
    last_time = time.time();
    simplex0 = OpenSimplex()
    tilex = 1024
    tiley = 1024
    matrix = [[0 for _ in range(tilex)] for _ in range(tiley)]
    for i in range(tiley):
        if (time.time() - last_time > 1):
            last_time = time.time();
            print("Row " + str(i) + " from " + str(tiley) + ".")
        for u in range(tilex):
            val = simplex0.noise2d(x=i / 2, y=u / 2)
            val += 0.6 * simplex0.noise2d(x=i / 5, y=u / 5)
            val += 0.7 * simplex0.noise2d(x=i / 25, y=u / 25)
            val += 0.4 * simplex0.noise2d(x=i / 125, y=u / 125)
            matrix[i][u] = 1 if val > 0.7 else 0

    newf = open(new_file_name, 'wb')
    writer = png.Writer(tilex, tiley, greyscale=True, bitdepth=1)
    writer.write(newf, matrix)


if __name__ == "__main__":
    gen("clouds.png")
