#!/share/apps/python/anaconda/bin/python

import sys
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

def main():
    "Plot Amdahl's law results (preclass Q1)"
    p = np.arange(1,129)
    S = 1.0 / (0.1 + 0.9/p)
    plt.plot(p, S)
    plt.xlabel('Processors')
    plt.ylabel('Speedup')
    plt.savefig('amdahl.svg', bbox_inches='tight')

if __name__ == "__main__":
    main()
