#include "stdafx.h"

CLDAPMessageViewHex::CLDAPMessageViewHex(CLDAPMsgCallback * pLdapCallback) : CLDAPMessage(pLdapCallback),
	m_HexviewUI(nullptr)
{
}

CLDAPMessageViewHex::CLDAPMessageViewHex(CLDAPMsgCallback * pLdapCallback,size_t pCap) : CLDAPMessage(pLdapCallback,pCap),
	m_HexviewUI(nullptr)
{
}

CLDAPMessageViewHex::~CLDAPMessageViewHex(void)
{
}

CLDAPMessage* CLDAPMessageViewHex::newObject()
{
	return new CLDAPMessageViewHex(m_Callback);
}

CLDAPMessage* CLDAPMessageViewHex::newObject( size_t pCap )
{
	return new CLDAPMessageViewHex(m_Callback,pCap);
}

int CLDAPMessageViewHex::ParseBER( CLDAPMessage* pParent )
{
	if( pParent != nullptr )
		this->HexviewUI( ((CLDAPMessageViewHex*)pParent)->HexviewUI() );
	return CLDAPMessage::ParseBER(pParent);
}

void CLDAPMessageViewHex::ToHexView()
{
	if( m_HexviewUI == nullptr )
	{
		return;
	}

	if( Form() == E_PC_CONTRUCT || Class() == E_CO_CONTEXT_SPECIFIC )
	{
		int _child_size = 0;
		std::vector<CLDAPMessage*>::iterator _it_child = m_childs.begin();
		for(_it_child = m_childs.begin() ; _it_child != m_childs.end(); _it_child++) 
		{
			_child_size += ((CLDAPMessageViewHex*)*_it_child)->Length();
		}

		int _constr_hd_len = Length() - _child_size;
		if( BeginRead() )
		{
			unsigned char _rb;
			while( ReadByte( _rb ) && _constr_hd_len > 0 ) 
			{
				WriteHexViewByte( _rb );
				_constr_hd_len--;
			}
		}

		_it_child = m_childs.begin();
		for(_it_child = m_childs.begin() ; _it_child != m_childs.end(); _it_child++) 
		{
			((CLDAPMessageViewHex*)*_it_child)->ToHexView();
		}
	}
	else
	{
		if( BeginRead() )
		{
			unsigned char _rb;
			while( ReadByte( _rb ) ) 
			{
				WriteHexViewByte( _rb );
			}
		}
	}
}

int CLDAPMessageViewHex::ToText( QString & oASN )
{
	if( Form() == E_PC_CONTRUCT || Class() == E_CO_CONTEXT_SPECIFIC )
	{
		QString _tname = QString::fromStdString(TagName());
		oASN += _tname.rightJustified(_tname.size() + Dept(),'\t');
		oASN += QString(" ::= { \n");

		std::vector<CLDAPMessage*>::iterator _it_child = m_childs.begin();
		for(_it_child = m_childs.begin() ; _it_child != m_childs.end(); _it_child++) 
		{
			((CLDAPMessageViewHex*)*_it_child)->ToText(oASN);
		}

		 _tname = QString("}\n");
		oASN += _tname.rightJustified(_tname.size() + Dept(),'\t');
	}
	else
	{
		QString _tname = QString::fromStdString(TagName());
		oASN += _tname.rightJustified(_tname.size() + Dept(),'\t');
		oASN += "(";
		oASN += QString::fromStdString(RawValue());
		oASN += ")";
		oASN += ",\n";
	}
	return 1;
}

int CLDAPMessageViewHex::ToHex( QString & oASN )
{
	if( BeginRead() )
	{
		unsigned char _rb;
		while( ReadByte( _rb ) ) 
		{
			oASN += QString::number( _rb , 16 ).toUpper().rightJustified(2,'0') + " " ;
		}
	}
	return 1;
}

int CLDAPMessageViewHex::WriteHexViewByte( unsigned char pByte )
{
	//insert hex view
	HexData * _hd = m_HexviewUI->createHexData();
	_hd->Hex(pByte);
	_hd->LDAPMsgInst(this);
	m_HexviewUI->AddHexData( _hd );
	return 1;
}
