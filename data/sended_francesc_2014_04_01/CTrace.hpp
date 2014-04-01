
#include <iostream>


using namespace std;



class CTrace{

public:
    
    
    
};


class CFuncTrace{
public:
	std::string m_szFunctionName;
public:
    CFuncTrace (string szFunctionName){
		m_szFunctionName = szFunctionName;
		
		cout << m_szFunctionName + ">>" << endl;
		
		
	};
	
	~CFuncTrace(){
		cout << m_szFunctionName + "<<" << endl;
	}
	
  //  void trace(const char *szToTrace);
	void trace(const char *szFormat, ...);
    
};
