
#include <iostream>

#ifndef CTRACE_H
#define CTRACE_H

using namespace std;



class CTrace{

public:
	
    
    
    
};


class CFuncTrace{
public:
	std::string FunctionName;
	bool traceOnCreateAndDestroy;
public:
    CFuncTrace (bool traceOnCreateAndDestroy,string FunctionName){
		this->FunctionName = FunctionName;
		this->traceOnCreateAndDestroy = traceOnCreateAndDestroy;
		if (traceOnCreateAndDestroy)
			cout << FunctionName + ">>" << endl;
	};
	
	~CFuncTrace(){
		if (this->traceOnCreateAndDestroy)
			cout << FunctionName + "<<" << endl;
	}
	void trace(const char *szFormat, ...);
    
};
#endif