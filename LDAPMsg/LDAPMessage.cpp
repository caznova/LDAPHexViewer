#include "stdafx.h"


CLDAPMessage::CLDAPMessage(CLDAPMsgCallback * pLdapClient)
	: m_Callback(pLdapClient), 
	m_payload_length(0),
	m_position(0)
{
	m_data = new std::vector<unsigned char>;
	AllocSize(INITIAL_DATA_BUFFER);

	Init();
}

CLDAPMessage::CLDAPMessage(CLDAPMsgCallback * pLdapCallback,size_t pCap)
	: m_Callback(pLdapCallback),  
	m_payload_length(0),
	m_position(0)
{
	m_data = new std::vector<unsigned char>;
	AllocSize( pCap );

	Init();
}

CLDAPMessage::~CLDAPMessage(void)
{
	Clean();

	boost::unordered::unordered_map<unsigned char,OnFnTagType*>::iterator  _it=   m_MapFnTag.begin();
	while( _it != m_MapFnTag.end() )
	{
		delete _it->second;
		++_it;
	}
	m_MapFnTag.clear();

	delete m_data;
}

void CLDAPMessage::Init()
{
	Form(0);
	Class(0);
	ID(0);
	Parent(nullptr);
	IsOpHandled(0);
	Dept(0);
	IsEncoder(0);
	//MapTag
	MapFNTag(E_TAG_EOC, boost::bind(&CLDAPMessage::OnTagEOC, this, _1) );
	MapFNTag(E_TAG_BOOLEAN, boost::bind(&CLDAPMessage::OnTagBoolean, this, _1) );
	MapFNTag(E_TAG_INTEGER, boost::bind(&CLDAPMessage::OnTagInteger, this, _1) );
	MapFNTag(E_TAG_BIT_STRING, boost::bind(&CLDAPMessage::OnTagBitString, this, _1) );
	MapFNTag(E_TAG_OCTET_STRING, boost::bind(&CLDAPMessage::OnTagOctString, this, _1) );
	MapFNTag(E_TAG_NULL, boost::bind(&CLDAPMessage::OnTagNull, this, _1) );
	MapFNTag(E_TAG_OBJECT_ID, boost::bind(&CLDAPMessage::OnTagObjectID, this, _1) );
	MapFNTag(E_TAG_OBJECT_DESC, boost::bind(&CLDAPMessage::OnTagObjectDesc, this, _1) );
	MapFNTag(E_TAG_EXTERNAL, boost::bind(&CLDAPMessage::OnTagExternal, this, _1) );
	MapFNTag(E_TAG_REAL, boost::bind(&CLDAPMessage::OnTagReal, this, _1) );
	MapFNTag(E_TAG_ENUMERATED, boost::bind(&CLDAPMessage::OnTagEnumerated, this, _1) );
	MapFNTag(E_TAG_EMBEDDED_PDV, boost::bind(&CLDAPMessage::OnTagEmbeddedPDV, this, _1) );
	MapFNTag(E_TAG_UTF8_STRING, boost::bind(&CLDAPMessage::OnTagUTF8String, this, _1) );
	MapFNTag(E_TAG_NUMERIC_STRING, boost::bind(&CLDAPMessage::OnTagNumericString, this, _1) );
	MapFNTag(E_TAG_PRINTABLE_STRING, boost::bind(&CLDAPMessage::OnTagPrintableString, this, _1) );
	MapFNTag(E_TAG_TELETEXTSTRING, boost::bind(&CLDAPMessage::OnTagTeletextString, this, _1) );
	MapFNTag(E_TAG_IA5_STRING, boost::bind(&CLDAPMessage::OnTagIA5String, this, _1) );
	MapFNTag(E_TAG_UTCTIME, boost::bind(&CLDAPMessage::OnTagUTCTime, this, _1) );
	MapFNTag(E_TAG_GENERALIZE_TIME, boost::bind(&CLDAPMessage::OnTagGeneralizeTime, this, _1) );
	MapFNTag(E_TAG_GRAPHIC_STRING, boost::bind(&CLDAPMessage::OnTagGraphicString, this, _1) );
	MapFNTag(E_TAG_VISIBLE_STRING, boost::bind(&CLDAPMessage::OnTagVisibleString, this, _1) );
	MapFNTag(E_TAG_GENERAL_STRING, boost::bind(&CLDAPMessage::OnTagGeneralString, this, _1) );
	MapFNTag(E_TAG_UNIVERSAL_STRING, boost::bind(&CLDAPMessage::OnTagUniversalString, this, _1) );
	MapFNTag(E_TAG_BMP_STRING, boost::bind(&CLDAPMessage::OnTagBMPString, this, _1) );
	MapFNTag(E_TAG_SEQ_OF, boost::bind(&CLDAPMessage::OnTagSequence, this, _1) );
	MapFNTag(E_TAG_SET, boost::bind(&CLDAPMessage::OnTagSet, this, _1) );
	MapFNTag(E_TAG_CONTEXTUAL, boost::bind(&CLDAPMessage::OnTagContextual, this, _1) );
}

size_t CLDAPMessage::Length() const
{
	return m_payload_length;
}


void CLDAPMessage::Length( size_t pLen )
{
	m_payload_length = pLen;
}

const unsigned char* CLDAPMessage::Payload() const
{
	return (unsigned char*)&(m_data->at(0));
}

unsigned char*  CLDAPMessage::Payload()
{
	return (unsigned char*)&(m_data->at(0));
}

size_t CLDAPMessage::CheckAllocSize() const
{
	return m_data->max_size();
}

int CLDAPMessage::AllocSize(size_t val )
{
	m_data->resize( val );
	return 1;
}

int CLDAPMessage::CheckAllocSize( size_t val )
{
	if( val >= m_data->size() )
	{
		m_data->resize( val + GROWTH_SIZE );
	}

	return 1;
}

int CLDAPMessage::Clean()
{
	std::vector<CLDAPMessage*>::iterator _it_del = m_childs.begin();
	for(_it_del = m_childs.begin() ; _it_del != m_childs.end(); _it_del++) 
	{
		delete (*_it_del);
	}

	return 1;
}


CLDAPMessage* CLDAPMessage::newObject()
{
	return new CLDAPMessage(m_Callback);
}

CLDAPMessage* CLDAPMessage::newObject( size_t pCap )
{
	return new CLDAPMessage(m_Callback,pCap);
}

CLDAPMessage & CLDAPMessage::operator=( const CLDAPMessage &rhs )
{
	if( &rhs == this ) return *this;

	Clean();

	Seek(0);
	Write( (unsigned char*) rhs.Payload() , rhs.Length() );
	return *this;
}

CLDAPMessage & CLDAPMessage::operator+( const CLDAPMessage &rhs )
{
	if( &rhs == this ) 
	{
		CLDAPMessage _tmp( nullptr );
		_tmp = rhs;
		Write( (unsigned char*) _tmp.Payload() , _tmp.Length() );
		return *this;
	}

	Write( (unsigned char*) rhs.Payload() , rhs.Length() );
	return *this;
}

CLDAPMessage & CLDAPMessage::operator+=( const CLDAPMessage &rhs )
{
	if( &rhs == this ) 
	{
		CLDAPMessage _tmp( nullptr );
		_tmp = rhs;
		Write( (unsigned char*) _tmp.Payload() , _tmp.Length() );
		return *this;
	}

	Write( (unsigned char*) rhs.Payload() , rhs.Length() );
	return *this;
}

void CLDAPMessage::Msg( const char * pMsg )
{
	if( m_Callback == nullptr ) return;

	std::auto_ptr<BYTE_AUTOPTR> _e(new BYTE_AUTOPTR(256));
	_e->sprintf_s("CLDAPMessage::[%s]%s",m_TagName.c_str(),pMsg);
	m_Callback->DoMsg( ( (const char*)_e->Buffer() ) );
}

void CLDAPMessage::Exception( const char * pException )
{
	if( m_Callback == nullptr ) return;

	std::auto_ptr<BYTE_AUTOPTR> _e(new BYTE_AUTOPTR(256));
	_e->sprintf_s("CLDAPMessage::[%s][%d][%d]-%s",m_TagName.c_str(),Dept(),m_payload_length,pException);
	m_Callback->DoException( std::exception( (const char*)_e->Buffer() ) );
}

int CLDAPMessage::BeginRead()
{
	m_position = 0;
	return 1;
}

int CLDAPMessage::Seek( size_t pPosition )
{
	if( pPosition < 0 || pPosition >= m_data->capacity()  )
	{
		return 0;
	}

	m_position = pPosition;
	return 1;
}

int CLDAPMessage::SeekToEnd()
{
	m_position = m_payload_length;
	return 1;
}

int CLDAPMessage::ReadByte( unsigned char & oByte )
{
	if(  m_position > m_payload_length - sizeof(unsigned char) )
	{
		return 0;
	}
	memcpy( &oByte, Payload() + m_position, sizeof(unsigned char) );
	m_position += sizeof(unsigned char);
	return 1;
}


int CLDAPMessage::ReadWord( unsigned short & oWord )
{
	if(  m_position > m_payload_length - sizeof(unsigned short) )
	{
		return 0;
	}
	memcpy( &oWord, Payload() + m_position, sizeof(unsigned short) );
	m_position += sizeof(unsigned short);
	return 1;
}

int CLDAPMessage::ReadDword( unsigned int & oDword )
{
	if(  m_position > m_payload_length - sizeof(unsigned int)  )
	{
		return 0;
	}
	memcpy( &oDword, Payload() + m_position, sizeof(unsigned int) );
	m_position += sizeof(unsigned int);
	return 1;
}

int CLDAPMessage::ReadLongLong( unsigned long long & oLongLong )
{
	if(  m_position > m_payload_length  - sizeof(unsigned long long) )
	{
		return 0;
	}
	memcpy( &oLongLong, Payload() + m_position, sizeof(unsigned long long) );
	m_position += sizeof(unsigned long long);
	return 1;
}

int CLDAPMessage::ReadFloat( float & oFloat )
{
	if(  m_position > m_payload_length - sizeof(float) )
	{
		return 0;
	}
	memcpy( &oFloat, Payload() + m_position, sizeof(float) );
	m_position += sizeof(float);
	return 1;
}

int CLDAPMessage::ReadStringA( size_t & oStrSize, char * oString )
{
	if( m_position > m_payload_length )
	{
		return 0;
	}

	unsigned char * _start_pos = Payload()  + m_position;
	unsigned char * _cpyret = (unsigned char *)strncpy(oString,(char*)( Payload()  + m_position), INITIAL_DATA_BUFFER - m_position );
	if( _cpyret != _start_pos )
	{
		return 0;
	}

	oStrSize = strlen((const char *)_cpyret) + 1;
	m_position += oStrSize;
	return 1;
}

int CLDAPMessage::Read( unsigned char * oBuf , size_t pReadSize )
{
	if(  m_position > m_payload_length - pReadSize )
	{
		return 0;
	}
	memcpy( oBuf , Payload() + m_position, pReadSize );
	m_position += pReadSize;
	return 1;
}

int CLDAPMessage::WriteByte( unsigned char pByte )
{
	if( !CheckAllocSize(m_position + sizeof(unsigned char))  )
	{
		return 0;
	}
	memcpy(  Payload() + m_position ,&pByte, sizeof(unsigned char) );
	if( m_position == m_payload_length )
		m_payload_length += sizeof(unsigned char );
	m_position += sizeof(unsigned char);
	return 1;
}

int CLDAPMessage::WriteWord( unsigned short pWord )
{
	if( !CheckAllocSize(m_position + sizeof(unsigned short)) )
	{
		return 0;
	}
	memcpy(  Payload() + m_position ,&pWord, sizeof(unsigned short) );
	if( m_position == m_payload_length )
		m_payload_length += sizeof(unsigned short );
	m_position += sizeof(unsigned short);
	return 1;
}

int CLDAPMessage::WriteDword( unsigned int pDword )
{
	if( !CheckAllocSize(m_position + sizeof(unsigned int))  )
	{
		return 0;
	}
	memcpy( Payload() + m_position , &pDword, sizeof(unsigned int) );
	if( m_position == m_payload_length )
		m_payload_length += sizeof(unsigned int );
	m_position += sizeof(unsigned int);
	return 1;
}

int CLDAPMessage::WriteLongLong( unsigned long long pLongLong )
{
	if( !CheckAllocSize(m_position + sizeof(unsigned long long))  )
	{
		return 0;
	}
	memcpy( Payload() + m_position ,&pLongLong , sizeof(unsigned long long ) );
	if( m_position == m_payload_length )
		m_payload_length += sizeof(unsigned long long );
	m_position += sizeof(unsigned long long );
	return 1;
}

int CLDAPMessage::WriteFloat( float pFloat )
{
	if( !CheckAllocSize(m_position + sizeof(float))  )
	{
		return 0;
	}
	memcpy( Payload() + m_position , &pFloat, sizeof(float) );
	if( m_position == m_payload_length )
		m_payload_length += sizeof( float );
	m_position += sizeof( float );
	return 1;
}

int CLDAPMessage::WriteStringA( size_t & StrSize, char * String )
{
	if( !CheckAllocSize(m_position + StrSize + 1) )
	{
		return 0;
	}

	unsigned char * _start_pos = Payload()  + m_position;
	unsigned char * _cpyret = (unsigned char *)strcpy((char*)( Payload()  + m_position),String );
	if( _cpyret != _start_pos )
	{
		return 0;
	}
	StrSize = strlen((const char *)_cpyret);
	if( m_position == m_payload_length )
		m_payload_length += (StrSize + 1);
	m_position += StrSize + 1;
	Payload()[m_position] = 0;
	return 1;
}

int CLDAPMessage::Write( unsigned char * oBuf, size_t pWriteSize )
{
	if( !CheckAllocSize(m_position + pWriteSize) )
	{
		return 0;
	}
	memcpy( Payload() + m_position , oBuf , pWriteSize );
	if( m_position == m_payload_length )
		m_payload_length += pWriteSize;
	m_position += pWriteSize;

	return 1;
}

int CLDAPMessage::WriteFromHex( const char * pHexStr )
{
	std::string _s(pHexStr);
	boost::algorithm::trim(_s);
	boost::algorithm::to_lower(_s);
	boost::algorithm::replace_all(_s," ","");
	boost::algorithm::replace_all(_s,"\r","");
	boost::algorithm::replace_all(_s,"\n","");

	if( _s.size() == 0 ) 
	{
		return 0;
	}

	std::string::iterator _it_hexstr = _s.begin();

	for( ; _it_hexstr != _s.end() && _it_hexstr + 1 !=  _s.end() ; _it_hexstr = _it_hexstr + 2 )
	{
		std::string _hexbyte( _it_hexstr , _it_hexstr + 2 ); 
		unsigned short byte = 0;
		std::istringstream _iss(_hexbyte);
		_iss >> std::hex >> byte;
		WriteByte( byte % 0x100 );
	}

	return 1;
}

//!---------------------------------------------------------------------------------------------------------
//!---------------------------------------------------------------------------------------------------------
//!---------------------------------------------------------------------------------------------------------

int CLDAPMessage::EncodeBEROctetString( char * String )
{
	size_t _strSize = strlen(String);

	if( !CheckAllocSize(m_position + _strSize)  )
	{
		return 0;
	}

	WriteByte( E_TAG_OCTET_STRING );
	size_t _lSize = EncodeBERLength(_strSize);

	unsigned char * _start_pos = Payload()  + m_position;
	unsigned char * _cpyret = (unsigned char *)memcpy((char*)( Payload()  + m_position), String , _strSize );
	if( _cpyret != _start_pos )
	{
		return 0;
	}

	if( m_position == m_payload_length )
		m_payload_length += (_strSize );
	m_position += _strSize ;
	return _strSize + 1 + _lSize; //E_TAG_CONTEXTUAL + Len + LenSize
}

int CLDAPMessage::EncodeBERContextual( char * Ctx , size_t pLen )
{
	if( !CheckAllocSize(m_position + pLen)  )
	{
		return 0;
	}

	WriteByte( E_TAG_CONTEXTUAL );
	size_t _lSize = EncodeBERLength(pLen);

	unsigned char * _start_pos = Payload()  + m_position;
	unsigned char * _cpyret = (unsigned char *)memcpy((char*)( Payload()  + m_position), Ctx , pLen );
	if( _cpyret != _start_pos )
	{
		return 0;
	}

	if( m_position == m_payload_length )
		m_payload_length += (pLen );
	m_position += pLen ;
	return pLen + 1 + _lSize; //E_TAG_CONTEXTUAL + Len + LenSize
}

int CLDAPMessage::EncodeBERLength( size_t pLen , unsigned char pIndefinite /* = 0 */ )
{
	if ( pIndefinite )
	{
		WriteByte( INDEFINITE_LENGTH );
	}
	else if( pLen <= MAX_DEFINITE_LENGTH ) //short form
	{
		WriteByte( pLen );
	}
	else //long form
	{
		unsigned int _tmp_len = pLen;
		WriteByte( 0 ); unsigned char *_len = CurrentPayloadPtr() - 1; 
		for(int i = 3 ; i >= 0 ; --i )
		{
			unsigned char _byte =  *((unsigned char*)&(_tmp_len) + i);
			if( _byte != 0  )
			{
				WriteByte( _byte );
			}
		}
		*_len = CurrentPayloadPtr() - _len - 1;
		*_len |= 0x80;
		return *_len + 1;//Len + LenSize
	}
	return 1;//LenSize
}

int CLDAPMessage::EncodeBERInteger( int pInteger )
{
	WriteByte( E_TAG_INTEGER );

	int _tmp_len = pInteger;
	WriteByte( 0 ); register unsigned char *_len = CurrentPayloadPtr() - 1;
	for(int i = 3 ; i >= 0 ; --i )
	{
		unsigned char _byte =  *((unsigned char*)&(_tmp_len) + i);
		if( _byte != 0  )
		{
			WriteByte( _byte );
		}
	}
	*_len =  (CurrentPayloadPtr() - _len) - 1 ;
	return *_len + 1 + 1; //Len + E_TAG_INTEGER + LenSize
}


int CLDAPMessage::EncodeBERNull()
{
	WriteByte( E_TAG_NULL );
	WriteByte( 0x00 );
	return 1;
}

int CLDAPMessage::EncodeBERIA5String( char * String )
{
	return 1;
}

int CLDAPMessage::EncodeBERUTF8String( char * String )
{
	return 1;
}

int CLDAPMessage::EncodeBERBITString( char * String )
{
	return 1;
}

int CLDAPMessage::EncodeBERReal( double Real )
{
	return 1;
}

int CLDAPMessage::EncodeBERObjectID( char * ID )
{
	return 1;
}

int CLDAPMessage::EncodeBERObjectDesc( char * Desc )
{
	return 1;
}

int CLDAPMessage::EncodeBERBoolean( unsigned char Bool )
{
	WriteByte( E_TAG_BOOLEAN );
	WriteByte( 1 ); //Lazy write length
	if( Bool )
	{
		WriteByte(0xFF);
	}
	else
	{
		WriteByte(0x00);
	}
	return 1;
}

int CLDAPMessage::EncodeBEREOC()
{
	return 1;
}

//!---------------------------------------------------------------------------------------------------------
//!---------------------------------------------------------------------------------------------------------
//!---------------------------------------------------------------------------------------------------------

int CLDAPMessage::DecodeBERLength( int & oLength , unsigned char & pIndefinite )
{
	pIndefinite = 0;

	size_t _DataLen = 0;
	unsigned char _SizeOfLen;
	ReadByte(_SizeOfLen);
	if (_SizeOfLen > INDEFINITE_LENGTH )	 
	{
		//long form
		_SizeOfLen = (_SizeOfLen & 0x7F); //erase bit 8
		int _l = _SizeOfLen;
		while( _l > 0 )
		{
			--_l;
			unsigned char _bS;
			ReadByte( _bS );
			_DataLen |= (_bS) << (8 * (_l));
		}
		_SizeOfLen += 1; //own
	}
	else if (_SizeOfLen == INDEFINITE_LENGTH )
	{
		pIndefinite = 1;
		Exception("not support indefinite form.");
	}
	else
	{
		//short form
		_DataLen = _SizeOfLen;
		_SizeOfLen = 1; //short form re-correct size-of-length
	}
	oLength = _DataLen;
	return 1;
}

int CLDAPMessage::DecodeBEROctetString( char * pBuffer , size_t pBufLen )
{
	int _Length;
	unsigned char Indefinite;
	if( DecodeBERLength( _Length , Indefinite ) )
	{
		if( _Length >= (int)pBufLen ) return 0; //error

		pBuffer[_Length] = 0;
		int _l = _Length;
		int _idx = 0;
		while( _l > 0 )
		{
			--_l;
			unsigned char _bS;
			ReadByte( _bS );
			pBuffer[_idx++] = _bS;
		}
		return 1;
	}
	return 0;
}

int CLDAPMessage::DecodeBERContextual( char * pBuffer , size_t pBufLen )
{
	return 1;
}

int CLDAPMessage::DecodeBERInteger( int & oInteger )
{
	int _Length;
	 unsigned char Indefinite;
	if( DecodeBERLength( _Length , Indefinite ) )
	{
		int _val = 0;
		int _l = _Length;
		while( _l > 0 )
		{
			--_l;
			unsigned char _bS;
			ReadByte( _bS );
			_val |= (_bS) << (8 * (_l));
		}
		oInteger = _val;
		return 1;
	}

	return 0;
}

int CLDAPMessage::DecodeBERNull()
{
	return 1;
}

int CLDAPMessage::DecodeBERIA5String( char * pBuffer , size_t pBufLen )
{
	return 1;
}

int CLDAPMessage::DecodeBERUTF8String( char * pBuffer , size_t pBufLen )
{
	return 1;
}

int CLDAPMessage::DecodeBERBITString( char * pBuffer , size_t pBufLen )
{
	return 1;
}

int CLDAPMessage::DecodeBERReal( double & Real )
{
	return 1;
}

int CLDAPMessage::DecodeBERObjectID( char * pBuffer , size_t pBufLen )
{
	return 1;
}

int CLDAPMessage::DecodeBERObjectDesc( char * pBuffer , size_t pBufLen )
{
	return 1;
}

int CLDAPMessage::DecodeBERBoolean( unsigned char & oBool )
{
	return 1;
}

int CLDAPMessage::DecodeBEREOC()
{
	return 1;
}

//!---------------------------------------------------------------------------------------------------------
//!---------------------------------------------------------------------------------------------------------
//!---------------------------------------------------------------------------------------------------------

void CLDAPMessage::BuildMessage(unsigned char pType , unsigned int pMsgId , CLDAPMessage & pTagMsg )
{
	//Header
	WriteByte(pType);

	//get len msgID
	size_t _posBeforeMsgID = Length();
	size_t _msgIdLen = EncodeBERInteger( pMsgId );
	Seek(_posBeforeMsgID);

	//Packet size
	EncodeBERLength( pTagMsg.Length() + _msgIdLen  ); 
	//msgID
	EncodeBERInteger( pMsgId );

	Write( pTagMsg.Payload() , pTagMsg.Length() );
}

int CLDAPMessage::EncodeBER( std::string & oError )
{
	if( IsEncoder() )
	{
		WriteByte( LDAPTag() );
		if( Form() == E_PC_CONTRUCT )
		{
			std::vector<CLDAPMessage*>::iterator _it_child = m_childs.begin();
			//encode
			for(_it_child = m_childs.begin() ; _it_child != m_childs.end(); _it_child++) 
			{
				(*_it_child)->IsEncoder(1);
				(*_it_child)->EncodeBER(oError);
			}
			//sum sizes
			size_t _overall = 0;
			for(_it_child = m_childs.begin() ; _it_child != m_childs.end(); _it_child++) 
			{
				_overall += (*_it_child)->Length();
			}
			EncodeBERLength( _overall );
			//write data
			for(_it_child = m_childs.begin() ; _it_child != m_childs.end(); _it_child++) 
			{
				Write( (*_it_child)->Payload() , (*_it_child)->Length() );
			}
		}
		else
		{
			CallFNTag( LDAPTag() , Parent() );
		}

		return 1;
	}

	return 0;
}

int CLDAPMessage::WriteTag( unsigned char pType , CLDAPMessage & pTagMsg )
{
	//Header
	WriteByte(pType);

	//Packet size
	EncodeBERLength( pTagMsg.Length() );

	//write data
	Write( pTagMsg.Payload() , pTagMsg.Length() );
	return 1;
}

int CLDAPMessage::WriteOp( unsigned char OpCode , CLDAPMessage & pOpMsg )
{
	//Op
	WriteByte(OpCode);

	//Packet size
	EncodeBERLength( pOpMsg.Length() );

	Write( pOpMsg.Payload() , pOpMsg.Length() );
	return 1;
}

unsigned char * CLDAPMessage::CurrentPayloadPtr()
{
	return  (Payload() + m_position);
}

size_t CLDAPMessage::RemainPayloadSize()
{
	return m_payload_length - m_position;
}

int CLDAPMessage::IsCompletedData(unsigned char & oOpCode,size_t & oDatalen)
{
	BeginRead();
	unsigned int _MsgLen = 0;
	if( ReadByte(m_LDAPTag) )
	{
		if( CheckTagContruct() )
		{
			unsigned char _SizeOfLen;
			if( ReadByte(_SizeOfLen) )
			{		
				if (_SizeOfLen > INDEFINITE_LENGTH )	 
				{
					//long form
					size_t _len_size = (_SizeOfLen & 0x7F); //erase bit 8
					while( _len_size > 0 )
					{
						--_len_size;
						unsigned char _bS;
						if( !ReadByte( _bS ) )
						{
							return 0;
						}
						_MsgLen |= (_bS) << (8 * (_len_size));
					}
					_len_size += 1;//own
				}
				else if (_SizeOfLen == INDEFINITE_LENGTH )
				{
					Exception("not support indefinite form.");
				}
				else
				{
					//short form
					_MsgLen = _SizeOfLen;
					_SizeOfLen = 1;//re-correct short form length
				}

				if( _MsgLen > (m_payload_length - m_position) )
				{
					return 0; //not completed data
				}
				else
				{
					oDatalen = _MsgLen;
					return 1;
				}
			}//read byte Len
		}//head
	}//read byte head
	return 0;
}

int CLDAPMessage::CheckTagContruct()
{
	m_Form = (m_LDAPTag & 0x20) >> 5;
	m_Class = (m_LDAPTag >> 6);
	m_ID = (m_LDAPTag & 0x1F);

	if( m_Form == E_PC_CONTRUCT )
	{
		return 1;
	}
	return 0;
}


//!---------------------------------------------Signal handler----------------------------------------------------
int CLDAPMessage::MapFNTag( unsigned char TagType ,const OnFnTagType & slot )
{
	if( m_MapFnTag.find(TagType) == m_MapFnTag.end() )
	{
		m_MapFnTag[TagType] = new OnFnTagType(slot);
	}
	return 1;
}

int CLDAPMessage::CallFNTag( unsigned char TagType , CLDAPMessage * parentData )
{
	if( m_MapFnTag.find(TagType) != m_MapFnTag.end() )
	{
		OnFnTag _sig;
		_sig.connect( *(m_MapFnTag[TagType]) );
		return *_sig(parentData);
	}
	else
	{
		return OnCheckOpTag(parentData);
	}
	return MSG_PARSE_ERROR;
}


//!---------------------------------------------End Signal handler-------------------------------------------------

int CLDAPMessage::ParseBER(CLDAPMessage* parentData)
{
	if( IsEncoder() )
		return 0;

	Parent(parentData);
	if( Parent() != nullptr )
	{
		Dept( Parent()->Dept() + 1 );
		ChildIdx( Parent()->Childs().size() + 1 );
	}

	Seek(0);
	unsigned int _MsgLen = 0;
	if( ReadByte(m_LDAPTag) )
	{
		int _result = CallFNTag( m_LDAPTag , parentData );
		if( _result == MSG_PARSE_NEXT )
		{
			if( CheckTagContruct() )
			{
				return ProcessContruct( parentData );
			}
			else
			{
				return ProcessPrimitive( parentData );
			}
		}
		else if( _result == MSG_PARSE_ERROR )
		{
			std::auto_ptr<BYTE_AUTOPTR> _e(new BYTE_AUTOPTR(64));
			_e->sprintf_s("not handler op/tag [%02X] length %d.",m_LDAPTag,m_payload_length);
			Exception( (const char*)_e->Buffer() );
		}
	}//read byte head
	return 0;
}

int CLDAPMessage::ParseBERInDefinite(CLDAPMessage* parentData)
{
	if( IsEncoder() )
		return 0;

	Parent(parentData);
	if( Parent() != nullptr )
	{
		Dept( Parent()->Dept() + 1 );
		ChildIdx( Parent()->Childs().size() + 1 );
	}

	Seek(0);
	unsigned int _MsgLen = 0;
	if( ReadByte(m_LDAPTag) )
	{
		int _result = CallFNTag( m_LDAPTag , parentData );
		if( _result == MSG_PARSE_NEXT )
		{
			if( CheckTagContruct() )
			{
				return ProcessContruct( parentData );
			}
			else
			{
				return ProcessPrimitive( parentData );
			}
		}
		else if( _result == MSG_PARSE_ERROR )
		{
			std::auto_ptr<BYTE_AUTOPTR> _e(new BYTE_AUTOPTR(64));
			_e->sprintf_s("not handler op/tag [%02X] length %d.",m_LDAPTag,m_payload_length);
			Exception( (const char*)_e->Buffer() );
		}
	}//read byte head
	return 0;
}

void CLDAPMessage::AddChild( CLDAPMessage* pChild )
{
	if( pChild != nullptr )
		pChild->Parent(this);
	m_childs.push_back(pChild);
}

int CLDAPMessage::ProcessContruct( CLDAPMessage * parentData )
{
	//get length
	size_t _MsgLen = 0;
	unsigned char _SizeOfLenRoot;
	ReadByte(_SizeOfLenRoot);
	if (_SizeOfLenRoot > INDEFINITE_LENGTH )	 
	{
		//long form
		_SizeOfLenRoot = (_SizeOfLenRoot & 0x7F); //erase bit 8
		int _l = _SizeOfLenRoot;
		while( _l > 0 )
		{
			--_l;
			unsigned char _bS;
			ReadByte( _bS );
			_MsgLen |= (_bS) << (8 * (_l));
		}
		_SizeOfLenRoot += 1;//own
	}
	else if (_SizeOfLenRoot == INDEFINITE_LENGTH )
	{
		Exception("not support indefinite form.");

		//size_t _newMsgSize = Length() - 1 - 1  ; //CLASS + LENGTH 
		//CLDAPMessage * _checkIndef = newObject(  _newMsgSize + 1 );
		//_checkIndef->Write( CurrentPayloadPtr() , _newMsgSize ); 
		//_checkIndef->ParseBERInDefinite( nullptr );
		//_MsgLen = _checkIndef->Length();
		//_SizeOfLenRoot = 1;
		//delete _checkIndef;
	}
	else
	{
		//short form
		_MsgLen = _SizeOfLenRoot;
		_SizeOfLenRoot = 1; //short form re-correct size-of-length
	}

	unsigned char * _firstPtrOfChild = CurrentPayloadPtr();
	//get all child
	unsigned char _Tag = 0;
	unsigned char * _firstPtrOfTag = nullptr;
	while( ReadByte(_Tag) )
	{
		_firstPtrOfTag = CurrentPayloadPtr() - 1;

		size_t _DataLen = 0;
		unsigned char _SizeOfLen;
		ReadByte(_SizeOfLen);
		if (_SizeOfLen > INDEFINITE_LENGTH )	 
		{
			//long form
			_SizeOfLen = (_SizeOfLen & 0x7F); //erase bit 8
			int _l = _SizeOfLen;
			while( _l > 0 )
			{
				--_l;
				unsigned char _bS;
				ReadByte( _bS );
				_DataLen |= (_bS) << (8 * (_l));
			}
			_SizeOfLen += 1; //own
		}
		else if (_SizeOfLen == INDEFINITE_LENGTH )
		{
			Exception("not support indefinite form.");

			//
			//size_t _newMsgSize = Length() - m_position  ; //CLASS + LENGTH 
			//while( _newMsgSize > 0 )
			//{
			//	CLDAPMessage * _checkIndef = newObject(  _newMsgSize + 1 );
			//	_checkIndef->Write( CurrentPayloadPtr() + _DataLen , _newMsgSize ); 
			//	_checkIndef->ParseBERInDefinite( nullptr );
			//	int _L;
			//	unsigned char _Idf;
			//	_checkIndef->Seek(0);
			//	_checkIndef->ReadByte(_Idf);
			//	if( _Idf == E_TAG_EOC)
			//	{
			//		_DataLen += _checkIndef->m_position + 1;
			//		_newMsgSize = 0;
			//		break;
			//	}
			//	else
			//	{
			//		_checkIndef->DecodeBERLength(_L,_Idf);
			//		if( _Idf == 0 )
			//		{
			//			_DataLen += _checkIndef->m_position + _L;
			//			_newMsgSize -=  _checkIndef->m_position + _L;
			//			_SizeOfLen = 1;
			//		}
			//		else
			//		{

			//		}
			//	}
			//	delete _checkIndef;
			//}

		}
		else
		{
			//short form
			_DataLen = _SizeOfLen;
			_SizeOfLen = 1; //short form re-correct size-of-length
		}

		size_t _remainSize = RemainPayloadSize();
		if( _DataLen <= _remainSize   )
		{
			size_t _newMsgSize = 1 + _SizeOfLen + _DataLen  ; //1ByteOfTag + SizeOfLen + DataLen
			CLDAPMessage * _newChild = newObject(  _newMsgSize + 1 );
			_newChild->Write( _firstPtrOfTag , _newMsgSize ); 
			Seek( m_position + _DataLen );
			AddChild( _newChild );
			_newChild->ParseBER(this);
		}
		else
		{
			Exception( "tag length overflow." );
		}
	}//while read tag

	return 1;
}

int CLDAPMessage::ProcessPrimitive( CLDAPMessage * parentData )
{
	if( Class() == E_CO_CONTEXT_SPECIFIC )
	{
		int _Length;
		unsigned char _Idf;
		DecodeBERLength(_Length,_Idf); //skip contextual

		CLDAPMessage * _newChild = newObject(  _Length + 1 );
		_newChild->Write( CurrentPayloadPtr() , _Length ); 
		Seek( m_position + _Length );
		AddChild( _newChild );
		_newChild->ParseBER(this);
	}
	return 1;
}

//!---------------------------------------------Tag handler-------------------------------------------------
int CLDAPMessage::OnTagEOC( CLDAPMessage * parentData )
{
	TagName("EOC");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagBoolean( CLDAPMessage * parentData )
{
	TagName("BOOLEAN");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagInteger( CLDAPMessage * parentData )
{
	if( IsEncoder() )
	{
		Seek(0);
		std::istringstream _iss(m_RawValue);
		int _int;
		if( _iss >> _int )
		{
			EncodeBERInteger( _int );
		}
		else
		{
			EncodeBERInteger( 0 );
		}
	}
	else
	{
		int _val = 0;
		DecodeBERInteger( _val );
		std::stringstream _out;
		_out << _val;
		m_RawValue = _out.str();
	}

	TagName("INTEGER");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagBitString( CLDAPMessage * parentData )
{
	TagName("BITSTRING");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagOctString( CLDAPMessage * parentData )
{
	if( IsEncoder() )
	{
		Seek(0);
		EncodeBEROctetString( (char *)m_RawValue.c_str() );
	}
	else
	{
		char * _val = new char[Length()];
		DecodeBEROctetString( _val , Length() );
		m_RawValue = _val;
		delete [] _val;
	}
	TagName("OCTET STRING");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagNull( CLDAPMessage * parentData )
{
	TagName("NULL");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagObjectID( CLDAPMessage * parentData )
{
	TagName("OBJECT ID");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagObjectDesc( CLDAPMessage * parentData )
{
	TagName("OBJECT DESC");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagExternal( CLDAPMessage * parentData )
{
	TagName("EXTERNAL");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagReal( CLDAPMessage * parentData )
{
	TagName("REAL");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagEnumerated( CLDAPMessage * parentData )
{
	TagName("ENUMERATED");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagEmbeddedPDV( CLDAPMessage * parentData )
{
	TagName("EMBEDDED PDV");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagUTF8String( CLDAPMessage * parentData )
{
	TagName("OBJECT DESC");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagNumericString( CLDAPMessage * parentData )
{
	TagName("NUMERIC STRING");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagPrintableString( CLDAPMessage * parentData )
{
	TagName("PRINTABLE STRING");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagTeletextString( CLDAPMessage * parentData )
{
	TagName("TELETEXT STRING");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagIA5String( CLDAPMessage * parentData )
{
	TagName("IA5 STRING");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagUTCTime( CLDAPMessage * parentData )
{
	TagName("UTC TIME");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagGeneralizeTime( CLDAPMessage * parentData )
{
	TagName("GENERALIZE TIME");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagGraphicString( CLDAPMessage * parentData )
{
	TagName("GRAPHIC STRING");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagVisibleString( CLDAPMessage * parentData )
{
	TagName("VISIBLE STRING");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagGeneralString( CLDAPMessage * parentData )
{
	TagName("GENERAL STRING");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagUniversalString( CLDAPMessage * parentData )
{
	TagName("UNIVERSAL STRING");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagBMPString( CLDAPMessage * parentData )
{
	TagName("BMP STRING");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagSequence( CLDAPMessage * parentData )
{
	TagName("SEQUENCE");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagSet( CLDAPMessage * parentData )
{
	TagName("SET");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnTagContextual( CLDAPMessage * parentData )
{
	TagName("CONTEXTUAL");
	return MSG_PARSE_NEXT;
}

int CLDAPMessage::OnCheckOpTag( CLDAPMessage * parentData )
{
	if( IsEncoder()  )
		return MSG_PARSE_ERROR;

	if( IsOpHandled() == 1  )
	{
		return MSG_PARSE_NEXT;
	}
	else
	{
		if( m_Callback )
		{
			if( m_Callback->CallFNResponse( m_LDAPTag , *this ) )
			{
				return MSG_PARSE_STOP;
			}
		}

		//show default op name
		{
			std::auto_ptr<BYTE_AUTOPTR> _n(new BYTE_AUTOPTR(64));
			int CLASS = (m_LDAPTag >> 6);
			int ID = (m_LDAPTag & 0x1F);
			if( CLASS == 0 )
			{
				_n->sprintf_s("UNIVERSAL_%d",ID);
			}
			else if( CLASS == 1 )
			{
				_n->sprintf_s("APPLICATION_%d",ID);
			}
			else if( CLASS == 2 )
			{
				_n->sprintf_s("CONTEXT_SPECIFIC_%d",ID);
			}
			else if( CLASS == 4 )
			{
				_n->sprintf_s("PRIVATE_%d",ID);
			}
			TagName((const char*)_n->Buffer());
		}
	}

	return MSG_PARSE_NEXT;
}

//!---------------------------------------------End Tag handler-------------------------------------------------