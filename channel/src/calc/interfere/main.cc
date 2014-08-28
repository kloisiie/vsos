#include <base/env.h>
#include <base/printf.h>
#include <calc_session/client.h>
#include <calc_session/connection.h>

using namespace Genode;

int main(void)
{
	Calc::Connection h;
	int i=0;
	PERR("start interuption");
	while(true){
		i=i%2;
		switch(i){
			case 0: h.mul(1,1);
				break;
			case 1: h.div(32,2);
				break;		
		}
		i++;
	}
	return 0;
}
