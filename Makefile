lib = -lm

all: combiner.c map.c reduce.c lib_map_reduce.c lib_map_reduce.h lib_map_reduce_types.h
	cc -o combiner combiner.c $(lib)
	cc -o map map.c $(lib)
	cc -o reduce reduce.c $(lib)

combiner: combiner.c
		cc -o combiner $<

map: map.c lib_map_reduce.c lib_map_reduce.h lib_map_reduce_types.h
	cc -o map $<

reduce: reduce.c lib_map_reduce_types.h
	cc -o reduce $<

clean:
	rm combiner map reduce

