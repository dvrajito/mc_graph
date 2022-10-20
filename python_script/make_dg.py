#!/usr/bin/env python
from weight_graph import *
from topol import *

def make_dg(nnodes, narcs):
    x=WeightGraph()
    x.make_random(nnodes, narcs)
    filename="dg%d_%fdv.txt" %(nnodes, float(narcs)/nnodes)
    comment="A text graph"
    x.write(filename, comment)
    
def make_sp(nnodes, narcs):
    x=WeightGraph()
    x.make_random_form(nnodes, narcs, 0)
    filename="sp%d_%fdv.txt" %(nnodes, float(narcs)/nnodes)
    comment="A text graph"
    x.write(filename, comment)
    
def make_ukn(nnodes, narcs, low, high):
    x=WeightGraph()
    x.make_rand_weight(nnodes, narcs, low, high)
    filename="ukn%d_%fdv.txt" %(nnodes, float(narcs)/nnodes)
    comment="A text graph"
    x.write(filename, comment)

def make_sizes():
    sizes = [50, 60, 70, 80, 90, 100, 125, 150, 175, 200]
    #sizes = [200]
    for i in sizes:
        make_sp(i, 2*i)
        step = i*(i-1)/20
        for j in range(1,11):
            print i, j
            make_sp(i, j*step)

def make_thr():
    sizes = [5, 10, 15, 20, 25]
    steps = [0.5, 1, 2, 3]
    for small in sizes:
        for large in sizes:
            for step in steps:
                make_thorus_diag(small, large, 1, step)
    
if __name__ == '__main__' :
    x_axs = 10
    rate = 1.5
    axes = (10, 15)
    sizes = [5, 10, 15, 20]
    steps = [0.5, 1, 2, 3]
    for y_axs in axes:
        for z_axs in axes:
            for div_x in sizes:
                for div_yz in sizes:
                    for step in steps:
                        make_ellipsoid(x_axs, y_axs, z_axs, div_x, div_yz, rate)
