1> make all for building all source codes 
2> make map for building mapper alone
3> make reduce for building reducer alone 
4> make combiner for building combiner alone
5> combiner and mapper make use of read.txt
6> reducer can either take input from pipe or terminal. By default it is set 
   to pipe, follow comments in reduce.c.
   To run as a standalone prog -  ./reduce 0 1
7> To run map.c as a standalone prog follow comments in map.c
   To run as a standalone prog - ./map 0 1
8> The number of lines map prog can read can be increased. To do this follow comments in map.can
9> The lib package included defnes the struct used for storing data and a quick sort function which
   takes in words and sorts them alphabetically. The sort function is case sensitive.
10>The combiner makes use of both map and reduce. 
   to run combiner - ./combiner
11>If you want to change the text being processed change the text in read.txt
