clean:
	make -C graph_gslgraph/test clean
	make -C graph_analyze/test clean
	make -C graph_analyze/app clean
	make -C graph_compare/test clean
	make -C graph_core/test clean
	make -C graph_indicators/test/ clean
	make -C gsl_utillities/gsl_io/test/ clean

test:
	make -C gsl_utillities/gsl_io/test test
	make -C graph_gslgraph/test test
	make -C graph_analyze/test test
	make -C graph_compare/test  test
	make -C graph_core/test test
	make -C graph_indicators/test/ test


app:
	make -C graph_analyze/app
	make -C graph_reconstruction/src
	
