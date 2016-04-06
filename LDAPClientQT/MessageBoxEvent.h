#pragma once

class MessageBoxEvent :
	public QEvent
{
public: 
	MessageBoxEvent(E_MSG_TYPE kind, const QString &caption,
		const QString& message, int button0, int button1=QMessageBox::NoButton, 
		int button2=QMessageBox::NoButton );
	~MessageBoxEvent();

	E_MSG_TYPE  kind;	// kind of message box

	QString  caption;	// caption for message box
	QString  message;	// text for message

	int  button0;			// buttons to display
	int  button1;
	int  button2;
	unsigned int  resp;
};

