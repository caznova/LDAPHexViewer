#pragma once

class CLDAPMessage;

class  CTagDefClass
{
public:
	typedef boost::unordered::unordered_map<std::string,CTagDefClass*> TagNameToClass;
	typedef boost::unordered::unordered_map<std::string,unsigned char> TagNameOptional;
public:
	CTagDefClass(const char * pName,unsigned char pTagID);
	CTagDefClass(const char * pName,CTagDefClass * pTypeDef);
	~CTagDefClass(void);

	unsigned char LDAPTag() const { return m_LDAPTag; }
	void LDAPTag(unsigned char val) { m_LDAPTag = val; }
	unsigned char Form() const { return m_Form; }
	void Form(unsigned char val) { m_Form = val; }
	unsigned char Class() const { return m_Class; }
	void Class(unsigned char val) { m_Class = val; }
	unsigned char ID() const { return m_ID; }
	void ID(unsigned char val) { m_ID = val; }
	std::string Name() const { return m_Name; }
	void Name(std::string val) { m_Name = val; }
	unsigned char Optional() const { return m_Optional; }
	void Optional(unsigned char val) { m_Optional = val; }
	CTagDefClass::TagNameToClass Tag() const { return m_Tag; }
	void AddTagDefClass( const char * pTagName, CTagDefClass * pTagDef ,unsigned char pOptional = 0 );

	CTagDefClass* FindTagDefClass( const char * pTagName );
	unsigned char FindTagDefOptional( const char * pTagName );

	CLDAPMessage* ParseFromVector(std::vector<std::string> & pVec,std::string & oError);
private:
	unsigned char				m_LDAPTag;
	unsigned char				m_Form;
	unsigned char				m_Class;
	unsigned char				m_ID;
	unsigned char				m_Optional;
	std::string					m_Name;
	TagNameToClass				m_Tag;
	TagNameOptional				m_TagOptionFlag;
	CTagDefClass*				m_TypeRef;
};

