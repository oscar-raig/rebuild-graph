#!/bin/bash
export BASE_DIR=/Users/oscarraig/bitbucket/rebuild-graph/data/threshold_accepting/example_graphs/
#sh simulation.sh -n 2000 -i 2 -g $BASE_DIR/clus-inipy.txt
sh simulation.sh -n 200 -i 2 -g $BASE_DIR/sfba-inipy.txt
sh simulation.sh -n 200 -i 2 -g $BASE_DIR/rand-inipy.txt
sh simulation.sh -n 200 -i 2 -g $BASE_DIR/swws-inipy.txt
sh simulation.sh -n 200 -i 2 -g $BASE_DIR/circ-inipy.txt

