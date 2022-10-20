from graph_handle import *
from graph_functions import *
import math
import random

class WeightGraph(GraphHandle):
    def __init__(self):
        GraphHandle.__init__(self)
        self.weights=[]
        self.directed=0

    def make_rand_weight(self, nnodes, narcs, low, high):
        self.nr_nodes=nnodes
        self.nr_arcs = narcs
        self.graph = make_undirect_graph(nnodes, narcs)
        sort_graph(self.graph)
        self.make_points_spiral()
        self.random_weights(low, high)

    def make_random(self, nnodes, narcs):
        self.nr_nodes=nnodes
        self.nr_arcs = narcs
        self.graph = make_undirect_graph(nnodes, narcs)
        sort_graph(self.graph)
        if nnodes <= 10:
            self.make_points_simple()
        else:
            self.make_points_layered()
        self.compute_weights()
        
    def make_random_form(self, nnodes, narcs, form):
        self.nr_nodes=nnodes
        self.nr_arcs = narcs
        self.graph = make_undirect_graph(nnodes, narcs)
        sort_graph(self.graph)
        if form == 0:
            self.make_points_spiral()
        self.compute_weights()

    def random_weights(self, low, high):
        self.weights=[]
        gen=random.generator()
        for i in range(self.nr_arcs):
            wght = low + gen.random()*(high-low)
            self.weights.append(wght)
        
    def compute_weights(self):
        self.weights=[]
        for i in range(self.nr_arcs):
            wght=self.distance(self.graph[i][0], self.graph[i][1])
            self.weights.append(wght)

    def total_weight(self):
        tweight = 0
        for w in self.weights:
            tweight = tweight + w
        return tweight

    def distance(self, node1, node2):
        d = 0
        for i in range(3):
            di = self.points[node1][i]-self.points[node2][i]
            d = d + di*di
        return math.sqrt(d)

    def write(self, filename, comment):
        output = open(filename,"w")
        output.write("%d nodes\n" %(self.nr_nodes))
        output.write("%d edges\n" %(self.nr_arcs))
        for i in range(self.nr_arcs):
            output.write("%s %f " %(arc_to_str(self.graph[i]),self.weights[i]))
            if (i+1)%5 == 0:
                output.write('\n')
        output.write('\n')
        for i in range(self.nr_nodes):
            output.write("%f %f %f\n" %(self.points[i][0],
                                        self.points[i][1],
                                        self.points[i][2]))
        output.write("\n%s" %(comment))
        output.close()

    def read(self, filename):
        import string
        input = open(filename, "r")
        line=string.split(input.readline())
        self.nr_nodes=string.atoi(line[0])
        line=string.split(input.readline())
        self.nr_arcs=string.atoi(line[0])
        line=string.split(input.readline())
        j=0
        self.graph=[]
        for i in range(self.nr_arcs):
            self.graph.append(str_to_arc(line[j]))
            j=j+1
            self.weights.append(string.atof(line[j]))
            j=j+1
            if j >= len(line):
                line=string.split(input.readline())
                j=0
        self.points=[]
        for i in range(self.nr_nodes):
            line=string.split(input.readline())
            #print line
            pt=[]
            for j in range(3):
                pt.append(string.atof(line[j]))
            self.points.append(pt)
        #self.print_me()

    def print_me(self):
        print "The graph has ", self.nr_nodes, " nodes,"
        print "and ", self.nr_arcs, " arcs"
        s=""
        for i in range(self.nr_arcs):
            if (i+1)%10 == 0:
                print s
                s=""
            arc=self.graph[i]
            s="%s (%d, %d) %f" %(s, arc[0], arc[1], self.weights[i])
        if (self.points):
            for i in self.points:
                print i
