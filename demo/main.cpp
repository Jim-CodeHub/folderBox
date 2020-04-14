#include <iostream>
#include <string.h>
#include <folderBox.hpp>

using namespace NS_FOLDERBOX;
using namespace std;

int main( void )
{
	try{

		folderBox FB("./");

		FB.Poller();

	} catch (int err) {

		cout << strerror(err) <<endl;
	}

	return 0;
}
