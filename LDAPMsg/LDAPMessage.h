#pragma once

class CLDAPMsgCallback;

class CLDAPMessage
{
public:

	CLDAPMessage(CLDAPMsgCallback * pLdapCallback);
	CLDAPMessage(CLDAPMsgCallback * pLdapCallback,size_t pCap);
	~CLDAPMessage(void);

	void Init();
	size_t Length() const;
	void Length(size_t pLen);
	const unsigned char*  Payload() const;
	unsigned char*  Payload();
	size_t CheckAllocSize() const;
	int AllocSize(size_t val );
	int CheckAllocSize(size_t val);
private:
	int Clean();
	virtual CLDAPMessage* newObject();
	virtual CLDAPMessage* newObject(size_t pCap);
public:
	CLDAPMessage & operator=(const CLDAPMessage &rhs);
	CLDAPMessage & operator+(const CLDAPMessage &rhs);
	CLDAPMessage & operator+=(const CLDAPMessage &rhs);

	unsigned char LDAPTag() const { return m_LDAPTag; }
	void LDAPTag(unsigned char val) { m_LDAPTag = val; }

	std::string TagName() const { return m_TagName; }
	void TagName(std::string val) { m_TagName = val; }

	unsigned char Class() const { return m_Class; }
	void Class(unsigned char val) { m_Class = val; }

	unsigned char ID() const { return m_ID; }
	void ID(unsigned char val) { m_ID = val; }

	unsigned char Form() const { return m_Form; }
	void Form(unsigned char val) { m_Form = val; }

	CLDAPMessage* Parent() const { return m_Parent; }
	void Parent(CLDAPMessage* val) { m_Parent = val; }

	unsigned char Dept() const { return m_Dept; }
	void Dept(unsigned char val) { m_Dept = val; }

	unsigned int ChildIdx() const { return m_ChildIdx; }
	void ChildIdx(unsigned int val) { m_ChildIdx = val; }

	unsigned char IsOpHandled() const { return m_IsOpHandled; }
	void IsOpHandled(unsigned char val) { m_IsOpHandled = val; }

	void Msg(const char * pMsg);
	void Exception(const char * pException);

	std::string RawValue() const { return m_RawValue; }
	void RawValue(std::string val) { m_RawValue = val; }

	unsigned char IsEncoder() const { return m_IsEncoder; }
	void IsEncoder(unsigned char val) { m_IsEncoder = val; }

	//!--------------------------------------------------------------------BINARY HELPER
	int BeginRead();
	int Seek(size_t pPosition);
	int SeekToEnd();
	//!--------------------------------------------------------------------READER
	int ReadByte(unsigned char & oByte);
	int ReadWord( unsigned short & oWord);
	int ReadDword( unsigned int & oDword);
	int ReadLongLong( unsigned long long & oLongLong);
	int ReadFloat( float & oFloat);
	int ReadStringA( size_t & oStrSize, char * oString);
	int Read( unsigned char * oBuf , size_t pReadSize );
	//!--------------------------------------------------------------------WRITER
	virtual int WriteByte(unsigned char pByte);
	virtual int WriteWord(unsigned short pWord);
	virtual int WriteDword(unsigned int pDword);
	virtual int WriteLongLong(unsigned long long pLongLong);
	virtual int WriteFloat( float pFloat );
	virtual int WriteStringA( size_t & StrSize, char * String );
	virtual int Write(unsigned char * oBuf, size_t pWriteSize );
	virtual int WriteFromHex(const char * pHexStr);

	//!--------------------------------------------------------------------ENCODER
	int EncodeBEROctetString( char * String );
	int EncodeBERContextual( char * Ctx , size_t pLen );
	int EncodeBERLength(size_t pLen , unsigned char pIndefinite = 0);
	int EncodeBERInteger(int pInteger);
	int EncodeBERNull();
	int EncodeBERIA5String( char * String );
	int EncodeBERUTF8String( char * String );
	int EncodeBERBITString( char * String );
	int EncodeBERReal( double Real );
	int EncodeBERObjectID( char * ID );
	int EncodeBERObjectDesc( char * Desc );
	int EncodeBERBoolean( unsigned char Bool );
	int EncodeBEREOC();

	//!--------------------------------------------------------------------DECODER
	int DecodeBERLength( int & oLength , unsigned char & pIndefinite );
	int DecodeBEROctetString( char * pBuffer , size_t pBufLen );
	int DecodeBERContextual( char * pBuffer , size_t pBufLen );
	int DecodeBERInteger(int & oInteger);
	int DecodeBERNull();
	int DecodeBERIA5String( char * pBuffer , size_t pBufLen );
	int DecodeBERUTF8String( char * pBuffer , size_t pBufLen );
	int DecodeBERBITString( char * pBuffer , size_t pBufLen );
	int DecodeBERReal( double & Real );
	int DecodeBERObjectID( char * pBuffer , size_t pBufLen );
	int DecodeBERObjectDesc( char * pBuffer , size_t pBufLen );
	int DecodeBERBoolean( unsigned char & oBool );
	int DecodeBEREOC();

	//!--------------------------------------------------------------------HELPER
	virtual void BuildMessage(unsigned char pType , unsigned int pMsgId , CLDAPMessage & pTagMsg);
	virtual int EncodeBER(std::string & oError);
	virtual int WriteTag(unsigned char pType , CLDAPMessage & pTagMsg );
	virtual int WriteOp( unsigned char OpCode , CLDAPMessage & pOpMsg );
	unsigned char * CurrentPayloadPtr();
	size_t RemainPayloadSize();
	int IsCompletedData(unsigned char & oOpCode,size_t & oDatalen);
	int CheckTagContruct();

	virtual int ParseBER(CLDAPMessage* pParent);
	virtual int ParseBERInDefinite(CLDAPMessage* pParent);
public:
	std::vector<CLDAPMessage*> Childs() const { return m_childs; }
	void AddChild( CLDAPMessage* pChild );

	//Base : signal on function
	typedef boost::signals2::signal<int (CLDAPMessage * parentData)> OnFnTag;
	typedef OnFnTag::slot_type OnFnTagType;
	boost::unordered::unordered_map<unsigned char,OnFnTagType*> m_MapFnTag;
	int CallFNTag(unsigned char TagType, CLDAPMessage * parentData );
	int MapFNTag(unsigned char TagType,const OnFnTagType & slot);

	int ProcessContruct(CLDAPMessage * parentData);
	int ProcessPrimitive(CLDAPMessage * parentData);
	int OnTagEOC(CLDAPMessage * parentData);
	int OnTagBoolean(CLDAPMessage * parentData);
	int OnTagInteger(CLDAPMessage * parentData);
	int OnTagBitString(CLDAPMessage * parentData);
	int OnTagOctString(CLDAPMessage * parentData);
	int OnTagNull(CLDAPMessage * parentData);
	int OnTagObjectID(CLDAPMessage * parentData);
	int OnTagObjectDesc(CLDAPMessage * parentData);
	int OnTagExternal(CLDAPMessage * parentData);
	int OnTagReal(CLDAPMessage * parentData);
	int OnTagEnumerated(CLDAPMessage * parentData);
	int OnTagEmbeddedPDV(CLDAPMessage * parentData);
	int OnTagUTF8String(CLDAPMessage * parentData);
	int OnTagNumericString(CLDAPMessage * parentData);
	int OnTagPrintableString(CLDAPMessage * parentData);
	int OnTagTeletextString(CLDAPMessage * parentData);
	int OnTagIA5String(CLDAPMessage * parentData);
	int OnTagUTCTime(CLDAPMessage * parentData);
	int OnTagGeneralizeTime(CLDAPMessage * parentData);
	int OnTagGraphicString(CLDAPMessage * parentData);
	int OnTagVisibleString(CLDAPMessage * parentData);
	int OnTagGeneralString(CLDAPMessage * parentData);
	int OnTagUniversalString(CLDAPMessage * parentData);
	int OnTagBMPString(CLDAPMessage * parentData);
	int OnTagSequence(CLDAPMessage * parentData);
	int OnTagSet(CLDAPMessage * parentData);
	int OnTagContextual(CLDAPMessage * parentData);
	int OnCheckOpTag(CLDAPMessage * parentData);
private:
	//LDAP DATA
	unsigned char				m_LDAPTag;
	unsigned char				m_Form;
	unsigned char				m_Class;
	unsigned char				m_ID;
	CLDAPMessage*				m_Parent;
	unsigned char				m_Dept;
	unsigned int				m_ChildIdx;
	std::string					m_RawValue;
	unsigned char				m_IsOpHandled; //only use for decode BER
	unsigned char				m_IsEncoder; //only use for encode to BER
	std::string					m_TagName;

private:
	std::vector<unsigned char>*	m_data;
	size_t						m_payload_length;
	size_t						m_position;
protected:
	CLDAPMsgCallback*			m_Callback;
	std::vector<CLDAPMessage*>	m_childs;
};
