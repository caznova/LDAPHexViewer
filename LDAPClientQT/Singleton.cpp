#include "stdafx.h"


Singleton::Singleton(void)
{
	//!--------------------------------------------------------- INIT 
	m_IO = new boost::asio::io_service();
	m_WORK = new std::auto_ptr<boost::asio::io_service::work>( new boost::asio::io_service::work(*m_IO) );
	m_TGROUP = new boost::thread_group();
	for( size_t i = 0; i < 2 ; ++i )
	{
		m_TGROUP->create_thread( [&]() { m_IO->run(); });
	}
	//!--------------------------------------------------------- INIT 
}


Singleton::~Singleton(void)
{
	m_IO->stop();
	delete m_WORK;
	delete m_IO;
	delete m_TGROUP;
}

Singleton & Singleton::Instance()
{
	static Singleton  instance; 
	return instance;
}

int Singleton::Init( int argc, char *argv[] )
{
	m_App = new QApplication(argc, argv);
	m_Main = new LDAPClientQT();
	m_Main->show();
	int exitcode = m_App->exec();
	{
		delete m_App;
		delete m_Main;
	}
	return exitcode;
}

void Singleton::PostMainEvent( QEvent *event, int priority /*= Qt::NormalEventPriority*/ )
{
	QApplication::postEvent( Instance().Main() , event );
}
