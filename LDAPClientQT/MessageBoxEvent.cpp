#include "stdafx.h"



MessageBoxEvent::MessageBoxEvent(  E_MSG_TYPE kind, 
								 const QString &caption, const QString& message,
								 int button0, int button1, int button2 ) :  QEvent((QEvent::Type)ECE_MESSAGEBOX)
{
	this->kind    = kind;
	this->caption = caption;
	this->message = message;
	this->button0 = button0;
	this->button1 = button1;
	this->button2 = button2;
	this->resp = 0;
}

MessageBoxEvent::~MessageBoxEvent()
{

}