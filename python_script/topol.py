from weight_graph import *
import os
import os.path
import string
import math

def make_filename(the_path, generic_name):
    max_nr = 0
    file_list=os.listdir(the_path)
    for the_file in file_list:
        if string.find(the_file, generic_name) == 0:
            number_str = ""
            i=len(generic_name)
            while string.find(string.digits, the_file[i]) >= 0:
                number_str = number_str + the_file[i]
                i = i+1
            number = int(number_str)
            if number > max_nr:
                max_nr = max_nr + 1
    max_nr = max_nr + 1
    filename = os.path.join(the_path,
                            "%s%d.txt" %(generic_name, max_nr))
    return filename

def compute_radius(node_nr, step):
    alpha = math.pi / node_nr
    radius = step / (2 * math.sin(alpha / 2))
    return radius

def compute_point_thr(rads, radl, alphajs, alphail):
    ptx = [rads * math.cos(alphajs) + radl, 0, rads * math.sin(alphajs)]
    pt = [ptx[0] * math.cos(alphail),
          ptx[0]*math.sin(alphail), ptx[2]]
    return pt

def make_thorus(small, large, step_small, step_large):
    the_graph = WeightGraph()
    the_graph.nr_nodes = small * large
    the_graph.nr_arcs = the_graph.nr_nodes * 2
    the_graph.graph=[]
    the_graph.weights=[]
    the_graph.points=[]
    rads = compute_radius(small, step_small)
    radl = compute_radius(large, step_large)
    alphas = 2 * math.pi/small
    alphal = 2 * math.pi/large
    for i in range(large):
        for j in range(small):
            node = i * small + j
            the_graph.points.append(compute_point_thr(rads, radl,
                                                      j*alphas, i*alphal)) 
            jp1 = i * small + (j+1) % small
            ip1 = ((i+1) % large) * small + j
            the_graph.graph.append([node, jp1])
            the_graph.graph.append([node, ip1])
    the_path = "/home/danav/develop/mc_graph/graphs/torus/"
    generic_name = "thr%d_%d_" %(small, large)
    filename = make_filename(the_path, generic_name)
    comment = "A thorus with\n%d nodes on the small circle\n" %(small)
    comment = "%s%d nodes on the large circle\n" %(comment, large)
    comment = "%sa step of %d on the small circle\n" %(comment, step_small)
    comment = "%sa step of %d on the large circle\n" %(comment, step_large)
    the_graph.compute_weights()
    the_graph.write(filename, comment)

def make_thorus_diag(small, large, step_small, step_large):
    the_graph = WeightGraph()
    the_graph.nr_nodes = small * large
    the_graph.nr_arcs = the_graph.nr_nodes * 4
    the_graph.graph=[]
    the_graph.weights=[]
    the_graph.points=[]
    rads = compute_radius(small, step_small)
    radl = compute_radius(large, step_large)
    alphas = 2 * math.pi/small
    alphal = 2 * math.pi/large
    for i in range(large):
        for j in range(small):
            node = i * small + j
            the_graph.points.append(compute_point_thr(rads, radl,
                                                      j*alphas, i*alphal)) 
            jp1 = i * small + (j+1) % small
            ip1 = ((i+1) % large) * small + j
            ijp1 = ((i+1) % large) * small + (j+1) % small
            ip1jm1 = ((i+1) % large) * small + (j-1) % small
            the_graph.graph.append([node, jp1])
            the_graph.graph.append([node, ip1])
            the_graph.graph.append([node, ijp1])
            the_graph.graph.append([node, ip1jm1])
    the_path = "/home/danav/develop/mc_graph/graphs/torusd/"
    generic_name = "thrd%d_%d_" %(small, large)
    filename = make_filename(the_path, generic_name)
    comment = "A thorus with\n%d nodes on the small circle\n" %(small)
    comment = "%s%d nodes on the large circle\n" %(comment, large)
    comment = "%sa step of %d on the small circle\n" %(comment, step_small)
    comment = "%sa step of %d on the large circle\n" %(comment, step_large)
    the_graph.compute_weights()
    the_graph.write(filename, comment)

def compute_point_eps(x_axs, y_axs, z_axs, div_x, steps, i, j):
    alphai = i * math.pi / (div_x-1)
    alphaj = 2 * j * math.pi / steps
    px = x_axs * math.cos(alphai)
    ry = y_axs * math.sin(alphai)
    rz = z_axs * math.sin(alphai)
    pt = [px, rz * math.cos(alphaj), ry * math.sin(alphaj)]
    return pt

def make_ellipsoid(x_axs, y_axs, z_axs, div_x, div_yz, rate):
    the_graph = WeightGraph()
    the_graph.nr_nodes = 0
    the_graph.nr_arcs = 0
    the_graph.graph=[]
    the_graph.weights=[]
    the_graph.points=[]
    prev_steps=0
    for i in range(div_x):
        if (i==0 or i==div_x-1):
            steps=1
        else:
            steps = int(div_yz * rate ** abs(div_x / 2 - i))
            for j in range(steps):
                node1 = the_graph.nr_nodes + j
                node2 = the_graph.nr_nodes + (j+1) % steps
                the_graph.graph.append([node1, node2])
        for j in range(steps):
            the_graph.points.append(compute_point_eps(x_axs, y_axs, z_axs,
                                                      div_x, steps, i, j))
        if (steps > prev_steps):
            for j in range(steps):
                j1 = int(rate*j)
                if (j1 >= prev_steps):
                    j1 = 0
                node1 = the_graph.nr_nodes + j
                node2 = the_graph.nr_nodes - prev_steps + j1
                the_graph.graph.append([node1, node2])
        else:
            for j1 in range(prev_steps):
                j = int(j1 * rate)
                if (j >= steps):
                    j = 0
                node1 = the_graph.nr_nodes + j
                node2 = the_graph.nr_nodes - prev_steps + j1
                the_graph.graph.append([node1, node2])
        the_graph.nr_nodes = the_graph.nr_nodes + steps
        prev_steps = steps 
    the_graph.nr_arcs = len(the_graph.graph)
    the_path = "/home/danav/develop/mc_graph/graphs/ellips/"
    generic_name = "elps%d_%d_" %(div_x, div_yz)
    filename = make_filename(the_path, generic_name)
    comment = "An ellipsoid with\n%d divisions on x\n" %(div_x)
    comment = "%s%d divisions on the y-z\n" %(comment, div_yz)
    comment = "%sx axis: %g, y axis: %g, z axis: %g\n" %(comment,
                                                         x_axs, y_axs, z_axs)
    comment = "%sa dicreasing rate of %g\n" %(comment, rate)
    the_graph.compute_weights()
    the_graph.write(filename, comment)
