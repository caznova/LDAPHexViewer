#include "stdafx.h"


CTagDefClass::CTagDefClass(const char * pName,unsigned char pTagID)
	:m_LDAPTag(pTagID),
	m_Name(pName),
	m_TypeRef(nullptr)
{
	m_Form = (m_LDAPTag & 0x20) >> 5;
	m_Class = (m_LDAPTag >> 6);
	m_ID = (m_LDAPTag & 0x1F);
}

CTagDefClass::CTagDefClass(const char * pName,CTagDefClass * pTypeDef)
	:m_LDAPTag(0),
	m_Name(pName),
	m_TypeRef(pTypeDef)
{
	m_Form = E_PC_PRIMATIVE;
}

CTagDefClass::~CTagDefClass(void)
{
	m_Tag.clear();
	m_TagOptionFlag.clear();
}

void CTagDefClass::AddTagDefClass(  const char * pTagName, CTagDefClass * pTagDef,unsigned char pOptional )
{
	if( m_Tag.find(pTagName) == m_Tag.end() )
	{
		m_Tag[pTagName] = (pTagDef);
		m_TagOptionFlag[pTagName] = pOptional;
	}
	return;
}

CTagDefClass* CTagDefClass::FindTagDefClass( const char * pTagName )
{
	if( m_Tag.find(pTagName) != m_Tag.end() )
	{
		return (m_Tag[pTagName]);
	}
	return nullptr;
}

unsigned char CTagDefClass::FindTagDefOptional( const char * pTagName )
{
	if( m_TagOptionFlag.find(pTagName) != m_TagOptionFlag.end() )
	{
		return (m_TagOptionFlag[pTagName]);
	}
	return 1;
}

CLDAPMessage* CTagDefClass::ParseFromVector( std::vector<std::string> & pVec,std::string & oError )
{
	if( Form() == E_PC_PRIMATIVE )
	{
		//TODO GetKey , GetValue -> Erase Key ::= Value,[,]
		if( m_TypeRef  != nullptr )
		{
			//try get typedef
			//next until standard type
			CLDAPMessage * _child_msg = m_TypeRef->ParseFromVector( pVec , oError );
			return _child_msg;
		}
		else
		{
			CLDAPMessage * _msg = new CLDAPMessage(nullptr);
			_msg->TagName( Name() );
			_msg->LDAPTag( LDAPTag() );
			_msg->Form( Form() );
			_msg->Class( Class() );
			_msg->ID( ID() );

			pVec.erase( pVec.begin() ); //skip name
			std::string _value = (*(pVec.begin())).c_str();
			_msg->RawValue(_value);
			return _msg;
		}
	}
	else if( Form() == E_PC_CONTRUCT )
	{
		CLDAPMessage * _msg = new CLDAPMessage(nullptr);
		_msg->TagName( Name() );
		_msg->LDAPTag( LDAPTag() );
		_msg->Form( Form() );
		_msg->Class( Class() );
		_msg->ID( ID() );

		if( LDAPTag()  != E_TAG_CHOICE )
		{
			//check bracket bracket pair
			pVec.erase( pVec.begin() ); //skip name
			pVec.erase( pVec.begin() ); //skip ::=
			if( (*(pVec.begin())) == "{" && (*(pVec.end() - 1 ) ) == "}" )
			{
				pVec.erase( pVec.begin() );
				pVec.erase( pVec.end() - 1 );

				while( pVec.size() > 0 )
				{
					std::string _tagname = (*(pVec.begin())).c_str();
					CTagDefClass * _DefClass = FindTagDefClass( _tagname.c_str() );
					if( _DefClass != nullptr )
					{
						//crop vars
						std::vector<std::string> _var;
						std::vector<std::string>::iterator _it_vec_bracket = pVec.begin();     
						std::vector<std::string>::iterator _it_vec_comma = pVec.begin();     
						int _count_open_bracket = 0; //count pair of bracket
						int _cut_size = 0; //remove size form pVec
						if( _DefClass->Form() == E_PC_CONTRUCT )
						{
							//case : when found CONTRUCT (CHOICE,SET,SEQUENCE,...) 
							//-------find first bracket and comma 
							for(_it_vec_bracket = pVec.begin() ; _it_vec_bracket != pVec.end(); _it_vec_bracket++) 
							{
								_var.push_back((*_it_vec_bracket));
								//candidate comma and bracket
								if( (*_it_vec_bracket) == "{" )
								{//bracket prior found and break
									_count_open_bracket++;
									_it_vec_bracket++;
									break;
								}
								else if( (*_it_vec_bracket) == "," && _it_vec_comma == pVec.begin() )
								{//if not found comma its means we founded CONSTRUCT first
									_it_vec_comma = _it_vec_bracket;
								}
							}
						}
						if( _it_vec_comma < _it_vec_bracket && _it_vec_comma != pVec.begin() )
						{
							//if we founded PRIMITIVE first
							//reach only comma position
							//case : when found CHOICE 
							//-------and found ref vars type primitive before construct
							_count_open_bracket = 0;
							int _backward_size = _it_vec_bracket - _it_vec_comma;
							while( --_backward_size > 0 )
								_var.pop_back();
						}
						else
						{
							//if type CONSTRUCT,crop all
							for(_it_vec_bracket = _it_vec_bracket ; _it_vec_bracket != pVec.end(); _it_vec_bracket++) 
							{
								if( (*_it_vec_bracket) == "{" )
									_count_open_bracket++;
								else if( (*_it_vec_bracket) == "}" )
									_count_open_bracket--;

								if( _count_open_bracket == 0  )
								{
									if( (*_it_vec_bracket) != "," ) //not end of construct
									{
										_var.push_back((*_it_vec_bracket));
									}
									else
									{//end of construct
										_cut_size++;
										break;
									}
								}
								else
								{
									_var.push_back((*_it_vec_bracket));
								}
							}
						}

						_cut_size += _var.size();
						pVec.erase( pVec.begin() ,  pVec.begin() + _cut_size  );
						CLDAPMessage * _child_msg = _DefClass->ParseFromVector( _var , oError );
						_msg->AddChild( _child_msg );
					}
					else
					{
						oError += "Syntax error : invalid ASN.1 tag name ";
						oError += "\n";
						oError += _tagname .c_str();
						oError += "\n";
						break;
					}
				}//while vector > 0
			}//wrap by bracket
			else
			{
				oError += "Syntax error : not validate construct type, of ";
				oError += "\n";
				oError += Name();
				oError += "\n";
			}
		}
		else
		{
			pVec.erase( pVec.begin() ); //skip name
			//try get choice from list
			std::string _tagname = (*(pVec.begin())).c_str();
			CTagDefClass * _DefClass = FindTagDefClass( _tagname.c_str() );
			if( _DefClass != nullptr )
			{
				//split vars
				std::vector<std::string> _var;
				std::vector<std::string>::iterator _it_vec_bracket = pVec.begin();     
				std::vector<std::string>::iterator _it_vec_comma = pVec.begin();     
				int _count_open_bracket = 0; //count pair of bracket
				int _cut_size = 0; //remove size form pVec
				if( _DefClass->Form() == E_PC_CONTRUCT )
				{
					//case : when found CONTRUCT (CHOICE,SET,SEQUENCE,...) 
					//-------find first bracket and comma 
					for(_it_vec_bracket = pVec.begin() ; _it_vec_bracket != pVec.end(); _it_vec_bracket++) 
					{
						_var.push_back((*_it_vec_bracket));
						//candidate comma and bracket
						if( (*_it_vec_bracket) == "{" )
						{//bracket prior found and break
							_count_open_bracket++;
							_it_vec_bracket++;
							break;
						}
						else if( (*_it_vec_bracket) == "," && _it_vec_comma == pVec.begin() )
						{//if not found comma its means we founded CONSTRUCT first
							_it_vec_comma = _it_vec_bracket;
						}
					}
				}
				if( _it_vec_comma < _it_vec_bracket && _it_vec_comma != pVec.begin() )
				{
					//if we founded PRIMITIVE first
					//reach only reach comma position
					//case : when found CHOICE 
					//-------and found ref vars type primitive before construct
					_count_open_bracket = 0;
					int _backward_size = _it_vec_bracket - _it_vec_comma;
					while( --_backward_size > 0 )
						_var.pop_back();
				}
				else
				{
					//if type CONSTRUCT,crop all
					for(_it_vec_bracket = _it_vec_bracket ; _it_vec_bracket != pVec.end(); _it_vec_bracket++) 
					{
						if( (*_it_vec_bracket) == "{" )
							_count_open_bracket++;
						else if( (*_it_vec_bracket) == "}" )
							_count_open_bracket--;

						if( _count_open_bracket == 0  )
						{
							if( (*_it_vec_bracket) != "," ) //not end of construct
							{
								_var.push_back((*_it_vec_bracket));
							}
							else
							{//end of construct
								_cut_size++; 
								break;
							}
						}
						else
						{
							_var.push_back((*_it_vec_bracket));
						}
					}
				}

				_cut_size += _var.size();
				pVec.erase( pVec.begin() ,  pVec.begin() + _var.size()  );
				CLDAPMessage * _child_msg = _DefClass->ParseFromVector( _var , oError );
				_msg->AddChild( _child_msg );

				//replace CHOICE to CONTEXTUAL
				_msg->LDAPTag(E_TAG_CONTEXTUAL);
			}
			else
			{
				oError += "Syntax error : not specific ASN.1 tag name ";
				oError += "\n";
				oError +=  _tagname.c_str();
				oError += "\n";
			}
		}

		return _msg;
	}
	return nullptr;
}

