#include "stdafx.h"


HexViewUI::HexViewUI(QWidget * pParent) : QAbstractScrollArea( pParent ),
	m_Row(0),
	m_Col(DEFAULT_COL),
	m_viewRow(0)
{
	m_Left = rect().left();
	m_Top = rect().top();
	m_Height =  (DEFAULT_ROW * 27 );
	m_Width =  (DEFAULT_COL * 27 );
	m_Height += 30;
	m_Width += 30;
	setGeometry( m_Left , m_Top , m_Width , m_Height );

	setFixedWidth( m_Width );
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}


HexViewUI::~HexViewUI(void)
{
	ClearHexData();
	while( m_recycle_hxdata.size() > 0 )
	{
		HexData* _d = m_recycle_hxdata.front();
		m_recycle_hxdata.pop();
		delete _d;
	}

	while( m_hexBlocks.size() > 0 )
	{
		HexBlockUI * _blk = m_hexBlocks[m_hexBlocks.size() - 1];
		QObject::disconnect(_blk, SIGNAL(MouseEnter(HexBlockUI*)), this, SLOT(OnBlockActive(HexBlockUI*)));
		_blk->deleteLater();
		m_hexBlocks.pop_back();
	}
}

void HexViewUI::ClearHexData()
{
	std::vector<HexData*>::iterator _it_del = m_hexData.begin();
	for( _it_del = m_hexData.begin() ; _it_del != m_hexData.end(); _it_del++ )
	{
		destroyHexData( (*_it_del) );
	}
	m_hexData.clear();
}

void HexViewUI::AddHexData(HexData * pHd)
{
	m_hexData.push_back(pHd);
}

void HexViewUI::PopBack(int pLen)
{
	while( pLen > 0 && m_hexData.size() > 0 )
	{ 
		HexData * _d = *(m_hexData.end() - 1);
		destroyHexData( _d );
		m_hexData.pop_back();
		--pLen;
	}
}

void HexViewUI::ReArrange()
{
	//clear view
	std::vector<HexBlockUI*>::iterator _it_block = m_hexBlocks.begin();
	for( ; _it_block != m_hexBlocks.end() ; ++_it_block )
	{
		if(  (*_it_block)->getHexData() )
		{
			(*_it_block)->getHexData()->Hide();
		}
	}

	size_t _startIdx = m_viewRow * m_Col;
	_it_block = m_hexBlocks.begin();
	for( size_t i  = _startIdx ; i < m_hexData.size() ; ++i )
	{
		if( _it_block != m_hexBlocks.end() )
		{
			HexData * _hd = m_hexData[i];
			_hd->Show( *_it_block );
			++_it_block;
		}
	}
}

void HexViewUI::Refresh()
{
	size_t _hexRowCount = (m_hexData.size() / m_Col)  + 1;
	verticalScrollBar()->setRange( 0, 0 );
	if( _hexRowCount > m_Row )
	{
		verticalScrollBar()->setRange( 0, _hexRowCount - m_Row );
	}
	ReArrange();
}

void HexViewUI::RefreshBlock(size_t nRow)
{
	if( nRow == m_Row &&  nRow != 0) return;

	size_t _y = 15;
	size_t _x = 15;

	int _delta = nRow - m_Row;
	bool _increase = (_delta > 0 ?  true : false );
	int _oldCountBlock =  m_Col * m_Row;
	m_Row = (nRow == 0 ?  m_Row : nRow );
	//HEX view size
	{
		for(size_t i = 0 ; i < m_Col * m_Row ; ++i )
		{
			if( ( i ) % m_Col  == 0 && i != 0 )
			{
				_x = 15;
				_y += 27;
			}

			if( (_increase && i >= _oldCountBlock) || nRow == 0)
			{
				HexBlockUI * _blk = new HexBlockUI(this);
				QObject::connect(_blk, SIGNAL(MouseEnter(HexBlockUI*)), this, SLOT(OnBlockActive(HexBlockUI*)));
				QObject::connect(_blk, SIGNAL(MouseLeave(HexBlockUI*)), this, SLOT(OnBlockInactive(HexBlockUI*)));
				QObject::connect(_blk, SIGNAL(LeftClick(HexBlockUI*)), this, SLOT(OnBlockLeftClick(HexBlockUI*)));
				QObject::connect(_blk, SIGNAL(RightClick(HexBlockUI*)), this, SLOT(OnBlockRightClick(HexBlockUI*)));
				_blk->setGeometry(_x,_y, 25 , 25 );
				_blk->show();
				m_hexBlocks.push_back(_blk);
			}

			_x += 27;
		}
		if( !_increase )
		{
			while( m_hexBlocks.size() > m_Col * m_Row )
			{
				HexBlockUI * _blk = m_hexBlocks[m_hexBlocks.size() - 1];
				QObject::disconnect(_blk, SIGNAL(MouseEnter(HexBlockUI*)), this, SLOT(OnBlockActive(HexBlockUI*)));
				_blk->deleteLater();
				m_hexBlocks.pop_back();
			}
		}
	}
	Refresh();
}

HexData* HexViewUI::createHexData()
{
	HexData* _use;
	if( m_recycle_hxdata.size() > 0 )
	{
		_use = m_recycle_hxdata.front();
		m_recycle_hxdata.pop();
	}
	else
	{
		_use = new HexData();
	}
	return _use;
}

void HexViewUI::destroyHexData( HexData* pHd )
{
	pHd->Hide();
	m_recycle_hxdata.push( pHd );
}

void HexViewUI::scrollContentsBy( int dx, int dy )
{
	m_viewRow -= dy;
	ReArrange();
}

void HexViewUI::OnBlockActive( HexBlockUI* hBlk )
{
	hBlk->ActiveState(1);
	HexData * _hd = hBlk->getHexData();
	if( _hd )
	{
		CLDAPMessageViewHex * _hx =  _hd->LDAPMsgInst();
		if( _hx )
		{
			QString _a ;
			_hx->ToText( _a );
			emit SelASN( _a , _hx->ChildIdx() );
			size_t _construct_length = _hx->Length();
			size_t _startIdx = m_viewRow * m_Col;
			size_t _ffIdx = 0;
			for( size_t i  = _startIdx ; i < m_hexData.size() ; ++i )
			{
				HexData * _hd_next = m_hexData[i];
				CLDAPMessageViewHex * _hx_next =  _hd_next->LDAPMsgInst();

				if( _hx == _hx_next  )
				{
					if( !_ffIdx ) _ffIdx = i;
					_hd_next->Active( );
				}
				else
				{
					//check if construct
					if( (_hx->Form() == E_PC_CONTRUCT || _hx->Class() == E_CO_CONTEXT_SPECIFIC) && i <= _construct_length + _ffIdx && _ffIdx )
					{
						_hd_next->Active( );
					}
				}
			}//loop
		}
	}
}

void HexViewUI::OnBlockInactive( HexBlockUI* hBlk )
{
	hBlk->ActiveState(0);
	HexData * _hd = hBlk->getHexData();
	if( _hd )
	{
		CLDAPMessageViewHex * hx = _hd->LDAPMsgInst();
		if( hx )
		{
			for( size_t i = 0 ; i < m_hexData.size() ; ++i )
			{
				HexData * _hd_next = m_hexData[i];
				_hd_next->InActive( );
			}
		}
	}
}

void HexViewUI::OnBlockLeftClick( HexBlockUI* hBlk )
{
	HexData * _hd = hBlk->getHexData();
	if( _hd )
	{
		CLDAPMessageViewHex * _hx =  _hd->LDAPMsgInst();
		if( _hx )
		{
			QString _a ;
			_hx->ToHex( _a );
			QClipboard * clipboard = QApplication::clipboard();
			clipboard->setText( _a );
		}
	}
}

void HexViewUI::OnBlockRightClick( HexBlockUI* hBlk )
{
	HexData * _hd = hBlk->getHexData();
	if( _hd )
	{
		CLDAPMessageViewHex * _hx =  _hd->LDAPMsgInst();
		if( _hx )
		{
			QString _a ;
			_hx->ToText( _a );
			QClipboard * clipboard = QApplication::clipboard();
			clipboard->setText( _a );
		}
	}
}

void HexViewUI::resizeEvent( QResizeEvent * ev )
{
	int _hdelta = ev->size().height();
	_hdelta -= 20;
	_hdelta = _hdelta / 27;
	size_t newRow = floor(_hdelta);
	RefreshBlock( newRow );
	QWidget::resizeEvent(ev);
}

