#include <string.h>
#include <base/printf.h>
using namespace std;

void overflow(char *str){
	char buffer[10];
	PWRN("copy \"%s\" to buffer[10]",str);
	strcpy(buffer,str);
	PLOG("now,buffer is:\"%s\"",buffer);
	PWRN("returning");
}
int main(){
	char * str1="hello!";
	char * str2="this is an long string";
	overflow(str1);
	PWRN("successfully return");
	try{
		overflow(str2);
		PWRN("successfully return");
	}catch(...){
		PWRN("segment error");	
	}
	return 0;
}
