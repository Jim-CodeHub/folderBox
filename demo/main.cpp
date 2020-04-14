#include <iostream>
#include <string.h>
#include <folderBox.hpp>

using namespace NS_FOLDERBOX;
using namespace std;

void Add( const void *val, size_t _size )
{
	cout << "Add work" <<endl;

	cout << (char *)val << endl;

	return;
}

void Del( const void *val, size_t _size )
{
	cout << "Del work" <<endl;

	cout << (char *)val << endl;

	return;
}

int main( void )
{
	try{

		folderBox FB("./");

		FB.Poller( Add, Del );

	} catch (int err) {

		cout << strerror(err) <<endl;
	}

	return 0;
}
