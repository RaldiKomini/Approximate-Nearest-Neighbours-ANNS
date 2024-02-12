INCLUDE= ./include
MODULES= ./modules
PROGRAMS= ./programs

CFLAGS = -Wall -Werror -g -I$(INCLUDE)

PR_OBJS= $(MODULES)/pr_modl.o
PR_H= $(INCLUDE)/pr.h

METH_OBJS= $(MODULES)/methods_modl.o
METH_H= $(INCLUDE)/methods.h

$(METH_OBJS): $(METH_H)	# method_modl.cpp depends on method.h
$(PR_OBJS):	$(PR_H)		# pr_modl.cpp depends on pr.h

clean:
	rm -f $(METH_OBJS) $(PR_OBJS)

#########	LSH   ##########

LSH_OBJS= $(PROGRAMS)/lsh.o $(MODULES)/lsh_modl.o
EXEC= lsh
LSH_H= $(INCLUDE)/lsh.h

$(EXEC): $(LSH_OBJS) $(METH_OBJS) $(PR_OBJS)
	g++ $(LSH_OBJS) $(METH_OBJS) $(PR_OBJS) -o $(EXEC)

$(LSH_OBJS): $(LSH_H)	# lsh.cpp, lsh_modl.cpp depend on: lsh.h

lsh_clean:
	rm -f $(LSH_OBJS) $(EXEC)


#########	Hypercube   ##########

CUBE_OBJS= $(PROGRAMS)/hypercube.o $(MODULES)/hypercube_modl.o
EXEC2= cube
CUBE_H= $(INCLUDE)/hypercube.h

$(EXEC2): $(CUBE_OBJS) $(METH_OBJS) $(PR_OBJS)
	g++ $(CUBE_OBJS) $(METH_OBJS) $(PR_OBJS) -o $(EXEC2)

$(CUBE_OBJS): $(CUBE_H)

cube_clean:
	rm -f $(CUBE_OBJS) $(EXEC2)


#########	Cluster   ##########

CL_OBJS= $(PROGRAMS)/cluster.o $(MODULES)/cluster_modl.o
EXEC3= cluster
CL_H= $(INCLUDE)/cluster.h

$(EXEC3): $(CL_OBJS) $(MODULES)/lsh_modl.o $(MODULES)/hypercube_modl.o $(METH_OBJS) $(PR_OBJS)
	g++ $(CL_OBJS) $(MODULES)/lsh_modl.o $(MODULES)/hypercube_modl.o $(METH_OBJS) $(PR_OBJS) -o $(EXEC3)

$(CL_OBJS): $(CL_H)

cluster_clean:
	rm -f $(CL_OBJS) $(EXEC3)


#########	Graph Search   ##########

GRAPH_OBJS= $(PROGRAMS)/graph_search.o $(MODULES)/graph_index_modl.o $(MODULES)/gnns_modl.o $(MODULES)/mrng_modl.o $(MODULES)/lsh_modl.o $(MODULES)/graph_search_modl.o
GRAPH= graph_search

$(GRAPH): $(GRAPH_OBJS) $(METH_OBJS) $(PR_OBJS)
	g++ $(GRAPH_OBJS) $(METH_OBJS) $(PR_OBJS) $(CFLAGS) -o $(GRAPH)

graph_clean:
	rm -f $(GRAPH_OBJS) $(GRAPH)