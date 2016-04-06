#pragma once

class  CLDAPParserASN1
{
public:
	CLDAPParserASN1(void);
	~CLDAPParserASN1(void);


	CTagDefClass* CreateTagDef(const char * pName,unsigned char pTagID);
	CTagDefClass* CreateTagDef(const char * pName,CTagDefClass * pTypeDef);
	CLDAPMessage* Parse(const char * pASN,std::string & oError);
private:
	CTagDefClass*					m_TagLDAPSchema;
	std::vector<CTagDefClass*>		m_DefClasses;
};

