#include "stdafx.h"

ViewLDAPUI::ViewLDAPUI(CLDAPMessage* pLdpMsg,QWidget * parent) : QWidget(parent)
{
	setWindowFlags( Qt::WindowFlags::enum_type::Window );

	size_t _x = 200;
	size_t _y = 200;
	size_t _width = 0;
	size_t _height = 0;

	m_mainLayout = new QGridLayout;
	m_HexViewUI = new HexViewUI(this);
	_width = (m_HexViewUI->rect().width() * 2) + 50;
	_height = m_HexViewUI->rect().height() + 20;

	setGeometry( QRect(_x,_y,_width,_height) );
	m_TextEdit = new QTextEdit(this);
	m_mainLayout->addWidget(m_HexViewUI, 0, 0);
	m_mainLayout->addWidget(m_TextEdit, 0, 1);
	setLayout(m_mainLayout);
	m_mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
	setWindowTitle(tr("View Hex"));

	QObject::connect(m_HexViewUI, SIGNAL(ShowASN(QString)), this, SLOT( OnShowASN(QString)));
	QObject::connect(m_HexViewUI, SIGNAL(SelASN(QString,unsigned int)), this, SLOT( OnSelASN(QString,unsigned int)));


	m_ldpMsg = new  CLDAPMessageViewHex( nullptr );
	*m_ldpMsg = *(CLDAPMessageViewHex*)pLdpMsg;
	m_ldpMsg->HexviewUI( getHexViewUI() );
	m_ldpMsg->ParseBER( nullptr );
	m_ldpMsg->ToHexView();
	getHexViewUI()->Refresh();

	QString _fulltext ;
	m_ldpMsg->ToText( _fulltext );
	OnShowASN( _fulltext );
}


ViewLDAPUI::~ViewLDAPUI(void)
{
	QObject::disconnect(m_HexViewUI, SIGNAL(ShowASN(QString)), this, SLOT( OnShowASN(QString)));
	QObject::disconnect(m_HexViewUI, SIGNAL(SelASN(QString,unsigned int)), this, SLOT( OnSelASN(QString,unsigned int)));

	delete m_HexViewUI;
	delete m_TextEdit;
	delete m_mainLayout;
	delete m_ldpMsg;
}


void ViewLDAPUI::OnShowASN( QString pAsn )
{
	m_TextEdit->setPlainText(pAsn);
}

void ViewLDAPUI::OnSelASN( QString pAsn,unsigned int pChildIdx )
{
	m_TextEdit->moveCursor(QTextCursor::Start);

	QList<QTextEdit::ExtraSelection> extraSelections;
	QStringList _l = pAsn.split( "\n" );
	QStringList::Iterator _itl = _l.begin();
	while( _itl != _l.end() )
	{
		QString _f = (*_itl);
		size_t _child_idx = 0;
		if ( m_TextEdit->find(_f) )
		{
			_child_idx++;
			//if( _child_idx == pChildIdx - 1 )
			{
				QTextEdit::ExtraSelection selection;
				selection.format.setBackground( Qt::green );
				selection.format.setProperty(QTextFormat::FullWidthSelection, true);
				selection.cursor = m_TextEdit->textCursor();
				selection.cursor.clearSelection();
				extraSelections.append(selection);
			}
		}
		_itl++;
	}

	m_TextEdit->setExtraSelections(extraSelections);
	m_TextEdit->moveCursor(QTextCursor::Start);
}

void ViewLDAPUI::closeEvent( QCloseEvent *ev )
{
	this->deleteLater();
	QWidget::closeEvent(ev);
}
