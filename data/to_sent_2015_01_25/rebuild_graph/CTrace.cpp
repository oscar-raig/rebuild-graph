
#include "CTrace.hpp"


void CFuncTrace::trace(const char *Format, va_list  vaArguments){
	char szOutput[10000];
	vsnprintf(szOutput, 1000, Format, vaArguments);
	cout << this->FunctionName + " "  + szOutput << endl;
}


void CFuncTrace::trace(int level, const char *Format, ...){
	if ( level < levelTrace)
		return;
	
	va_list vaArguments;
	va_start(vaArguments, Format);
//	trace(Format,vaArguments);
	char szOutput[10000];
	vsnprintf(szOutput, 1000, Format, vaArguments);
	string traceLevelName = "DEBUG";
	switch ( level ){
		case CTrace::TRACE_ERROR:
			traceLevelName = "ERROR";
			break;
		case CTrace::TRACE_INFO:
			traceLevelName = "INFO";
			break;
	}
	cout << traceLevelName << " " << this->FunctionName + " "  + szOutput << endl;
	va_end(vaArguments);

}
