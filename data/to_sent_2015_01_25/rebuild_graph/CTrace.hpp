
#include <iostream>
#include <cstdarg>
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
	string GetTimeDate(){
		
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];
		
		time (&rawtime);
		timeinfo = localtime(&rawtime);
		
		strftime(buffer,80,"%d-%m-%Y %I:%M:%S ",timeinfo);
		std::string str(buffer);
		
		return str;
	}
    CFuncTrace (bool traceOnCreateAndDestroy,string FunctionName){

		levelTrace = CTrace::TRACE_INFO;
//		levelTrace = CTrace::TRACE_DEBUG;
//		levelTrace = CTrace::TRACE_ERROR;
		
		this->FunctionName = FunctionName;
		this->traceOnCreateAndDestroy = traceOnCreateAndDestroy;
		if (traceOnCreateAndDestroy)
			cout <<  GetTimeDate() << FunctionName + ">>" << endl;
	};
	
	~CFuncTrace(){
		if (this->traceOnCreateAndDestroy)
			cout <<  GetTimeDate() << FunctionName + "<<" << endl;
	}
	void trace( const char *Format,va_list  vaArguments);
	void trace(int level, const char *Format, ...);
    
};
#endif
