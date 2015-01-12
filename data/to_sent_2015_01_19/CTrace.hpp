
#include <iostream>

#ifndef CTRACE_H
#define CTRACE_H

using namespace std;



class CTrace{

public:
	
	enum level
	{
		TRACE_DEBUG,
		TRACE_INFO,
		TRACE_ERROR
	};
	
	
};


class CFuncTrace{
public:
	int levelTrace;
	std::string FunctionName;
	bool traceOnCreateAndDestroy;
public:
    CFuncTrace (bool traceOnCreateAndDestroy,string FunctionName){

		levelTrace = CTrace::level::TRACE_INFO;
//		levelTrace = CTrace::level::TRACE_DEBUG;
//		levelTrace = CTrace::level::TRACE_ERROR;
		
		this->FunctionName = FunctionName;
		this->traceOnCreateAndDestroy = traceOnCreateAndDestroy;
		if (traceOnCreateAndDestroy)
			cout << FunctionName + ">>" << endl;
	};
	
	~CFuncTrace(){
		if (this->traceOnCreateAndDestroy)
			cout << FunctionName + "<<" << endl;
	}
	void trace( const char *Format,va_list  vaArguments);
	void trace(int level, const char *Format, ...);
    
};
#endif