
#include "CTrace.hpp"


void CFuncTrace::trace(const char *Format, ...){
	va_list vaArguments;
	va_start(vaArguments, Format);
	char szOutput[10000];
	vsnprintf(szOutput, sizeof(szOutput), Format, vaArguments);
	cout << this->FunctionName + " "  + szOutput << endl;
	va_end(vaArguments);
}

