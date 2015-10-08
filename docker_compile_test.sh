docker run -v `pwd`:/home/ test-rebuild-graph  /bin/sh -c "cd /home; ./configure && make && make test"
