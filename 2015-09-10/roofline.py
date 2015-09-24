#!/share/apps/python/anaconda/bin/python

import sys
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

def main():
    "Roofline plot for Totient nodes"
    p = np.arange(1,129)
    
    alpha = 59.0  # Memory bandwidth (GByte/s)
    cmax = 460.8  # Peak floating point performance (GFlop/s)

    fig, ax = plt.subplots()
    plt.xlabel('Operational intensity (flops/byte)')
    plt.ylabel('Performance (Gflops/sec)')
    intensity = np.linspace(1.0/32,10)

    # Add line for various max c values
    def add_cmax(cmax, fmt, lbl):
        crossover = cmax/alpha
        intensity0 = np.linspace(1.0/32, crossover)
        intensity1 = np.linspace(crossover, 10)
        ax.loglog(intensity,  0*intensity +cmax, '{0}-'.format(fmt))
        ax.loglog(intensity1, 0*intensity1+cmax, fmt, linewidth=2.0, label=lbl)
        ax.loglog(intensity0, alpha*intensity0, 'k-', linewidth=2.0)
        print("{0}: crossover intensity {1}".format(lbl, crossover))
        
    add_cmax(460.8, 'k-', 'Peak DP performance')
    add_cmax(460.8/12, 'r-', 'Single-core peak')
    add_cmax(460.8/12/16, 'b-', 'Single-core scalar peak')

    # Add memory bandwidth term
    ax.loglog(intensity,  alpha*intensity, 'k--', linewidth=2.0)

    ax.set_xscale('log', basex=2)
    ax.set_yscale('log', basey=2)
    lgd = plt.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.)    
    plt.savefig('totient.svg', bbox_extra_artists=(lgd,), bbox_inches='tight')

if __name__ == "__main__":
    main()
