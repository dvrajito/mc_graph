import Tkinter
from graph_functions import *
import math

class GraphHandle:
    def __init__(self):
        self.nr_nodes=0
        self.nr_arcs=0
        self.graph=[]
        self.first_radius=1.0
        self.second_radius=100.0
        self.points=[]
        self.cluster=[]
        self.hc=[]
        self.directed=1

    def read_hc_graph(self,filename):
        self.nr_nodes=0
        self.nr_arcs=0
        self.graph=[]
        input=open(filename,"r")
        if input:
            line=input.readline()
            nr_clauses=string.atoi(line)
            for i in range(nr_clauses):
                line=input.readline()
            line=input.readline()
            line=input.readline()
            words=string.split(line,' ')
            self.nr_nodes=string.atoi(words[0])
            line=input.readline()
            words=string.split(line,' ')
            self.nr_arcs=string.atoi(words[0])
            self.graph=[]
            error=0
            while (not error) and (len(self.graph) < self.nr_arcs):
                line=input.readline()
                if line:
                    words=string.split(line,' ')
                    for j in range(len(words)):
                        if len(words[j])>1:
                            self.graph.append(str_to_arc(words[j]))
                else:
                    error=1
            line=input.readline()
            if line:
                line=input.readline()
                if line:
                    self.hc=string_to_hc(line,self.nr_nodes)
            input.close()
            mn=min_graph(self.graph)
            trans_graph(self.graph,mn)
            trans_hc(self.hc,mn)
        else:
            print "Could not open file named:",filename

    def make_points_simple(self):
        import whrandom
        import math
        pi=math.pi
        generator = whrandom.whrandom()
        rad=self.first_radius
        self.points=[]
        for i in range(self.nr_nodes):
            alpha=i*2*pi/self.nr_nodes
            beta=(1-2*(i%2))*generator.uniform(0.0,pi)
            x=rad*math.cos(alpha)*math.cos(beta)
            y=rad*math.sin(beta)
            z=rad*math.sin(alpha)*math.cos(beta)
            self.points.append([x,y,z])
            
    def make_points_spiral(self):
        import whrandom
        import math
        pi=math.pi
        generator = whrandom.whrandom()
        self.points=[]
        for i in range(self.nr_nodes):
            self.points.append([-1, -1, -1])
        queue=[0]
        rad=self.first_radius
        step=0.2*rad
        angle=0
        alpha=pi/6
        beta=pi/3
        factor=0.2
        index=0
        gr_out=make_graph_out_und(self.graph)
        while (index < len(queue)):
            v=queue[index]
            x=rad*math.cos(angle)
            y=rad*math.sin(angle)
            z=rad*factor
            self.points[v][0]=x
            self.points[v][1]=y
            self.points[v][2]=z 
            for w in gr_out[v]:
                if self.points[w][0] == -1:
                    queue.append(w)
                    self.points[w][0]=0
            rad=rad+step
            angle=angle+generator.uniform(alpha,beta)
            index=index+1
        
    def make_points_layered(self):
        #Rewrite this function
        if not self.cluster:
            self.make_cluster()
        import whrandom
        import math
        pi=math.pi
        generator = whrandom.whrandom()
        rad1=self.first_radius
        rad2=self.second_radius
        cluster_ord={}
        for i in range(len(self.cluster)):
            for j in self.cluster[i]:
                cluster_ord[j]=i
        centers=[]
        self.points=[]
        cl_nr=len(self.cluster)
        for i in range(cl_nr):
            alpha=i*2*pi/cl_nr
            beta=(1-2*(i%2))*generator.uniform(0.0,pi)
            x=rad2*math.cos(alpha)*math.cos(beta)
            y=rad2*math.sin(beta)
            z=rad2*math.sin(alpha)*math.cos(beta)
            centers.append([x,y,z])
        cl_size=len(self.cluster[0])
        for i in range(self.nr_nodes):
            alpha=(i%cl_size)*2*pi/cl_size
            beta=(1-2*(i%2))*generator.uniform(0.0,pi)
            cl=cluster_ord[i]
            x=centers[cl][0]+rad1*math.cos(alpha)*math.cos(beta)
            y=centers[cl][1]+rad1*math.sin(beta)
            z=centers[cl][2]+rad1*math.sin(alpha)*math.cos(beta)
            self.points.append([x,y,z])
            
    def make_cluster(self,cl_size=0):
        if cl_size==0:
             cl_size=int(math.ceil(math.sqrt(self.nr_nodes)))
        if not self.hc:
            self.hc=[]
            for i in range(self.nr_nodes):
                self.hc.append(i)
            self.hc.append(0)
        if self.directed:
            gr_out=make_graph_out(self.graph, self.nr_nodes)
            gr_in=make_graph_in(self.graph, self.nr_nodes)
        else:
            gr_out=make_graph_out_und(self.graph, self.nr_nodes)
            gr_in=make_graph_in_und(self.graph, self.nr_nodes)
        if (self.nr_nodes % cl_size ==0):
            cl_nr=self.nr_nodes/cl_size
        else:
            cl_nr=self.nr_nodes/cl_size+1
        mopt=-5000
        mi=-1
        for i in range(cl_size):
            cl=[]
            opt=0
            for j in range(cl_nr):
                if (j+1)*cl_size<=self.nr_nodes:
                    cluster_size=cl_size
                else:
                    cluster_size=self.nr_nodes%cl_size
                cluster={}
                for k in range(cluster_size):
                    cluster[self.hc[(i+j*cl_size+k)%self.nr_nodes]]=1
                for k in cluster.keys():
                    for n in gr_out[k]:
                        if cluster.has_key(n):
                            opt=opt+1
                        else:
                            opt=opt-1
                    for n in gr_in[k]:
                        if not cluster.has_key(n):
                            opt=opt-1
                cl.append(cluster.keys())
            if (mi==-1) or (opt>mopt):
                mopt=opt
                mi=i
                self.cluster=cl

    def print_me(self):
        print "The graph has ", self.nr_nodes, " nodes,"
        print "and ", self.nr_arcs, " arcs"
        s=""
        for i in range(self.nr_arcs):
            if (i+1)%10 == 0:
                print s
                s=""
            s="%s (%d, %d)" %(self.arcs[i][0],self.arcs[i][1])
        if self.hc:
            print self.hc
