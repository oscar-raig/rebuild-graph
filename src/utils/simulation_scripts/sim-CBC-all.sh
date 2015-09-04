#!/bin/bash
export BASE_DIR=/Users/oscarraig/bitbucket/rebuild-graph/data/threshold_accepting/example_graphs/
#sh simulation.sh -n 10 -i 3 -g ../example_graphs/circ-inipy.txt
#sh simulation.sh -n 10 -i 3 -g ../example_graphs/clus-inipy.txt
#sh simulation.sh -n 200 -i 3 -g $BASE_DIR/sfba-inipy.txt
#sh simulation.sh -n 200 -i 3 -g $BASE_DIR/rand-inipy.txt
sh simulation.sh -n 50 -i 3 -g $BASE_DIR/swws-inipy.txt
