INCLUDE= ./include
MODULES= ./modules
PROGRAMS= ./programs

CFLAGS = -Wall -Werror -g -I$(INCLUDE) -O3

PR_OBJS= $(MODULES)/pr_modl.o
PR_H= $(INCLUDE)/pr.h

METH_OBJS= $(MODULES)/methods_modl.o
METH_H= $(INCLUDE)/methods.h

$(METH_OBJS): $(METH_H)	# method_modl.cpp depends on method.h
$(PR_OBJS):	$(PR_H)		# pr_modl.cpp depends on pr.h

clean:
	rm -f $(METH_OBJS) $(PR_OBJS)


GRAPH_OBJS= $(PROGRAMS)/graph_search.o $(MODULES)/graph_index_modl.o $(MODULES)/gnns_modl.o $(MODULES)/mrng_modl.o $(MODULES)/lsh_modl.o $(MODULES)/graph_search_modl.o
GRAPH= graph_search

$(GRAPH): $(GRAPH_OBJS) $(METH_OBJS) $(PR_OBJS)
	g++ $(GRAPH_OBJS) $(METH_OBJS) $(PR_OBJS) $(CFLAGS) -o $(GRAPH)

graph_clean:
	rm -f $(GRAPH_OBJS) $(GRAPH)