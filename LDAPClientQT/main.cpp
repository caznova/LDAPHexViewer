#include "stdafx.h"

int wrapQT( int argc, char *argv[] )
{
	return Singleton::Instance().Init( argc, argv );
}

int main(int argc, char *argv[])
{
	boost::thread _threadUI( boost::bind(  wrapQT , argc , argv ));
	Singleton::Instance().IO()->run();
	_threadUI.join();
}
 