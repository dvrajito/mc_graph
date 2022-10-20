import whrandom
import string

def node_to_str(node_nr):
    left_nr=node_nr/26
    right_nr=node_nr%26
    try:
        nstr=""+chr(ord('A')+left_nr)+chr(ord('a')+right_nr)
    except:
        print node_nr
        nstr=""+chr(ord('A')+left_nr)+chr(ord('A')+right_nr/2)
    return nstr

def arc_to_str(arc):
    return "%s%s" %(node_to_str(arc[0]),node_to_str(arc[1]))

def str_to_node(astr):
    if len(astr)==1:
        return ord(astr[0])-ord('!')
    elif len(astr)==2:
        return (26*(ord(astr[0])-ord('A')) +
                ord(astr[1])-ord('a'))

def small_node_to_str(node_nr):
    return ""+chr(ord('!')+node_nr)

def str_to_arc(astr):
    str1=""
    str2=""
    for i in range(len(astr)/2):
        str1=str1+astr[i]
        str2=str2+astr[i+len(astr)/2]
    return [str_to_node(str1),str_to_node(str2)]

def make_hc(nr_nodes):
    if (nr_nodes > 1):
        generator = whrandom.whrandom()
        hc=[]
        nodes=[]
        graph=[]
        for i in range(nr_nodes):
            nodes.append(i)
        for i in range(nr_nodes):
            n=generator.randint(0,len(nodes)-1)
            hc.append(nodes[n])
            nodes.remove(nodes[n])
        for i in range(nr_nodes-1):
            graph.append([hc[i],hc[i+1]])
        graph.append([hc[nr_nodes-1],hc[0]])
        return graph
    else:
        return None

def string_hc(graph):
    hc_str=""
    for i in range(len(graph)):
        x=graph[i]
        hc_str=hc_str+node_to_str(x[0])
    hc_str=hc_str+node_to_str(graph[0][0])
    return hc_str

def string_to_hc(hc_str,nr_nodes):
    node_len=len(hc_str)/(nr_nodes+1)
    hc=[]
    for i in range(nr_nodes+1):
        str_node=""
        for j in range(node_len):
            str_node = str_node+hc_str[node_len*i+j]
        node=str_to_node(str_node)
        hc.append(node)
    return hc
    
def make_graph(nr_nodes, nr_arcs):
    if (nr_arcs > 0):
        generator = whrandom.whrandom()
        arcs=[]
        for i in range(nr_nodes-1):
            for j in range(i+1,nr_nodes):
                arcs=arcs+[[i,j]]
                arcs=arcs+[[j,i]]
        if (nr_arcs <= len(arcs)):
            total_arcs=nr_arcs
        else:
            total_arcs=len(arcs)
        graph=[]
        for i in range(total_arcs):
            n=generator.randint(0,len(arcs)-1)
            graph.append(arcs[n])
            arcs.remove(arcs[n])
    return graph
    
def make_undirect_graph(nr_nodes, nr_arcs):
    if (nr_arcs > 0):
        generator = whrandom.whrandom()
        arcs=[]
        for i in range(nr_nodes-1):
            for j in range(i+1,nr_nodes):
                arcs=arcs+[[i,j]]
        if (nr_arcs <= len(arcs)):
            total_arcs=nr_arcs
        else:
            total_arcs=len(arcs)
        graph=[]
        for i in range(total_arcs):
            n=generator.randint(0,len(arcs)-1)
            graph.append(arcs[n])
            arcs.remove(arcs[n])
    return graph

def string_graph(graph):
    print_str=""
    for i in range(len(graph)):
        x=graph[i]
        if (i%15 == 14) and (i != len(graph)-1):
            print_str= print_str+node_to_str(x[0])+node_to_str(x[1])+'\n'
        else:
            print_str= print_str+node_to_str(x[0])+node_to_str(x[1])+' '
    return print_str

def merge_graph(graph1,graph2):
    graph=[]
    i1=0
    i2=0
    while ((i1<len(graph1)) and (i2<len(graph2))):
        if ((graph1[i1][0] == graph2[i2][0]) and
            (graph1[i1][1] == graph2[i2][1])):
            graph.append(graph1[i1])
            i1=i1+1
            i2=i2+1
        elif ((graph1[i1][0] < graph2[i2][0]) or
              ((graph1[i1][0] == graph2[i2][0]) and
               (graph1[i1][1] < graph2[i2][1]))):
            graph.append(graph1[i1])
            i1=i1+1
        else:
            graph.append(graph2[i2])
            i2=i2+1
    for i in range(i1,len(graph1)):
        graph.append(graph1[i])
    for i in range(i2,len(graph2)):
        graph.append(graph2[i])
    return graph

def sort_graph(graph):
    for i in range(len(graph)-1):
        for j in range(i+1, len(graph)):
            if ((graph[i][0] > graph[j][0]) or
                ((graph[i][0] == graph[j][0]) and
                 (graph[i][1] > graph[j][1]))):
                x=graph[i][0]
                graph[i][0]=graph[j][0]
                graph[j][0]=x
                x=graph[i][1]
                graph[i][1]=graph[j][1]
                graph[j][1]=x

def sort_graph_for(graph):
    arcs={}
    for i in range(len(graph)):
        x=graph[i]
        if arcs.has_key(x[0]):
            arcs[x[0]].append(i)
        else:
            arcs[x[0]]=[i]
    return arcs

def sort_graph_back(graph):
    arcs={}
    for i in range(len(graph)):
        x=graph[i]
        if arcs.has_key(x[1]):
            arcs[x[1]].append(i)
        else:
            arcs[x[1]]=[i]
    return arcs

def min_graph(graph):
    min_node=graph[0][0]
    for i in range(len(graph)):
        if graph[i][0]<min_node:
            min_node=graph[i][0]
        if graph[i][1]<min_node:
            min_node=graph[i][1]
    return min_node

def trans_graph(graph,node):
    for i in range(len(graph)):
        graph[i][0]=graph[i][0]-node
        graph[i][1]=graph[i][1]-node
        
def trans_hc(hc_graph,node):
    for i in range(len(hc_graph)):
        hc_graph[i]=hc_graph[i]-node

def file_graph(graph,filename,comment):
    output = open(filename,"w")
    graph1=sort_graph_for(graph)
    graph2=sort_graph_back(graph)
    output.write("%d\n" %(len(graph1)+len(graph2)))
    for i in graph1.keys():
        str="%d " %(len(graph1[i]))
        for j in graph1[i]:
            str=str+"%d " %(j)
        output.write(str)
        output.write('\n')
    for i in graph2.keys():
        str="%d " %(len(graph2[i]))
        for j in graph2[i]:
            str=str+"%d " %(j)
        output.write(str)
        output.write('\n')
    output.write('\n')
    if len(graph1)>=len(graph2):
        output.write("%d nodes\n" %(len(graph1)))
    else:
        output.write("%d nodes\n" %(len(graph2)))
    output.write("%d arcs\n" %(len(graph)))
    output.write(string_graph(graph))
    output.write("\n%s" %(comment))
    output.close()

def string_to_graph(str):
    str_arcs=string.split(str,' ')
    graph=[]
    for i in str_arcs:
        graph.append(str_to_arc(str_arcs[i]))
    return graph

def make_hc_graph(nr_nodes,nr_arcs,filename):
    hc_gr=make_hc(nr_nodes)
    hc_str=string_hc(hc_gr)
    rand_gr=make_graph(nr_nodes,nr_arcs)
    sort_graph(hc_gr)
    sort_graph(rand_gr)
    final_gr=merge_graph(hc_gr,rand_gr)
    comment="Hamiltonian circuit:\n%s" %hc_str
    file_graph(final_gr,filename,comment)

def make_graph_out(graph, nnodes=0):
    graph_out={}
    for i in range(nnodes):
        graph_out[i] = []
    for i in range(len(graph)):
        if (not graph_out.has_key(graph[i][0])):
            graph_out[graph[i][0]]=[]
        graph_out[graph[i][0]].append(graph[i][1])
    return graph_out

def make_graph_in(graph, nnodes=0):
    gr_in={}
    for i in range(nnodes):
        gr_in[i] = []
    for i in range(len(graph)):
        if ( not gr_in.has_key(graph[i][1])):
            gr_in[graph[i][1]]=[]
        gr_in[graph[i][1]].append(graph[i][0])
    return gr_in

def make_graph_out_und(graph, nnodes=0):
    graph_out={}
    for i in range(nnodes):
        graph_out[i] = []
    for i in range(len(graph)):
        if (not graph_out.has_key(graph[i][0])):
            graph_out[graph[i][0]]=[]
        graph_out[graph[i][0]].append(graph[i][1])
        if (not graph_out.has_key(graph[i][1])):
            graph_out[graph[i][1]]=[]
        graph_out[graph[i][1]].append(graph[i][0])
    return graph_out

def make_graph_in_und(graph, nnodes=0):
    gr_in={}
    for i in range(nnodes):
        gr_in[i] = []
    for i in range(len(graph)):
        if ( not gr_in.has_key(graph[i][1])):
            gr_in[graph[i][1]]=[]
        gr_in[graph[i][1]].append(graph[i][0])
        if ( not gr_in.has_key(graph[i][0])):
            gr_in[graph[i][0]]=[]
        gr_in[graph[i][0]].append(graph[i][1])
    return gr_in
