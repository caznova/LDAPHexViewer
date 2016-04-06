#include "stdafx.h"
#include "HexBlockUI.h"


void HexData::Show( HexBlockUI* val )
{
	BlockRef(val);
	BlockRef()->setHexData( this );

	if( LDAPMsgInst() )
	{
		if( LDAPMsgInst()->Form() == E_PC_CONTRUCT ||  LDAPMsgInst()->Class() == E_CO_CONTEXT_SPECIFIC  )
		{
			BlockRef()->ActiveWithColor1();
		}
		else
		{
			BlockRef()->ActiveWithColor2();
		}
	}

	BlockRef()->update();
}

void HexData::Hide()
{
	if( BlockRef() )
	{
		BlockRef()->setHexData(nullptr);
		BlockRef()->ResetActiveColor();
		BlockRef()->update();
	}
	BlockRef(nullptr);
}

void HexData::Active()
{
	if( BlockRef() )
	{
		BlockRef()->ActiveState(1);
		BlockRef()->update();
	}
}

void HexData::InActive()
{
	if( BlockRef() )
	{
		BlockRef()->ActiveState(0);
		BlockRef()->update();
	}
}

//!--------------------------------------------------------------------------------------------------

HexBlockUI::HexBlockUI( QWidget * pParent ) :
	QWidget(pParent),
	m_HexData(nullptr),
	m_MouseOver(0),
	m_ActiveState(0),
	m_DefaultBg(DEFAULT_BLOCK_COLOR_R,DEFAULT_BLOCK_COLOR_G,DEFAULT_BLOCK_COLOR_B),
	m_bDefult(m_DefaultBg),
	m_bActiveDefult( QColor(77,239,175) ),
	m_bActiveColor1( QColor(255,128,64) ),
	m_bActiveColor2( QColor(65,235,254) ),
	m_bColor( m_DefaultBg ),
	m_fDefult(QApplication::font())
{
	m_fDefult.setPixelSize( rect().height() - 15 );
}


HexBlockUI::~HexBlockUI(void)
{

}

void HexBlockUI::enterEvent(QEvent *event)
{
	emit MouseEnter( this );
	QWidget::enterEvent(event);
}

void HexBlockUI::leaveEvent(QEvent *event)
{
	emit MouseLeave( this );
	QWidget::leaveEvent(event);
}

void HexBlockUI::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	painter.begin( this );
	painter.fillRect( rect() ,  m_bDefult );
	if( getHexData() != nullptr )
	{
		if( m_MouseOver || ActiveState() )
		{
			painter.fillRect( rect() ,  m_bColor );
		}
		else
		{
			painter.fillRect( rect() ,  m_bDefult );
		}

		painter.setFont( m_fDefult );
		painter.drawText(  rect() , Qt::AlignCenter, m_DrawHex );
	}
	painter.end();
	QWidget::paintEvent(event);
}

void HexBlockUI::setHexData( HexData* val )
{
	m_HexData = val;
	if( m_HexData != nullptr )
	{
		m_DrawHex =  QString::number( getHexData()->Hex() , 16 ).toUpper().rightJustified(2,'0');
	}
}

void HexBlockUI::mousePressEvent( QMouseEvent * ev)
{
	if( ev->button() == Qt::MouseButton::LeftButton )
	{
		emit LeftClick( this );
	}
	else if( ev->button() == Qt::MouseButton::RightButton )
	{
		emit RightClick( this );
	}

	QWidget::mousePressEvent(ev);
}
