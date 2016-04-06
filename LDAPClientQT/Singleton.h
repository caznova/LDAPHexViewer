#pragma once

class QApplication;
class LDAPClientQT;

class Singleton
{
public:
	Singleton(void);
	~Singleton(void);

	static Singleton & Instance();

	boost::asio::io_service* IO() const { return m_IO; }
	std::auto_ptr<boost::asio::io_service::work>* WORK() const { return m_WORK; }
	boost::thread_group* TGROUP() const { return m_TGROUP; }
	QApplication* App() const { return m_App; }
	LDAPClientQT* Main() const { return m_Main; }

	int Init( int argc, char *argv[]  );
	void PostMainEvent(QEvent *event, int priority = Qt::NormalEventPriority);
private:
	boost::asio::io_service*							m_IO;
	std::auto_ptr<boost::asio::io_service::work>*		m_WORK;
	boost::thread_group*								m_TGROUP;

	QApplication*										m_App;
	LDAPClientQT*										m_Main;
};

