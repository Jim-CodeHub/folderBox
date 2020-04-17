#include <iostream>
#include <string.h>
#include <syswatcher.hpp>

using namespace NS_SYSWATCHER;
using namespace std;

int wd[2];

void evFun( struct _watched_src src )
{
	cout << "Callback function work" << endl;

	cout << src.pathname			 << endl; /**< Full path name. */

	cout << src.mask_get			 << endl; /**< Occur events. */

	cout << src.mask_reg			 << endl; /**< Regists events. */

	cout << src.watch_fd			 << endl; /**< Watched filedescriptor. */

	if ( wd[0] == src.watch_fd )
	{
		if ( src.mask_get & IN_CREATE		) { cout << "create    event on wd :"<< wd[0] << endl; }
		if ( src.mask_get & IN_MOVED_TO		) { cout << "move to   event on wd :"<< wd[0] << endl; }
	}

	if ( wd[1] == src.watch_fd )
	{
		if ( src.mask_get & IN_DELETE		) { cout << "delete    event on wd :"<< wd[1] << endl; }
		if ( src.mask_get & IN_MOVED_FROM	) { cout << "move from event on wd :"<< wd[1] << endl; }
	}

	//sleep(1);
}

int main( void )
{
	char *buff = new char[4096]; /**< Set enough size for events. */

	try{

		syswatcher sw;

		wd[0] = sw.reg("./", IN_CREATE|IN_MOVED_TO);
		wd[1] = sw.reg("..", IN_DELETE|IN_MOVED_FROM);

		sw.poll( evFun, buff, 4096 );

	} catch (int err) {

		cout << strerror(err) <<endl;
	}

	delete [] buff;

	return 0;
}
