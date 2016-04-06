#include "stdafx.h"


CLDAPParserASN1::CLDAPParserASN1(void)
{
	//!!------------------------------------------------------------------------BASE
	//OCTET STRING
	CTagDefClass * _ldaOctetString = CreateTagDef("octet string",E_TAG_OCTET_STRING);

	//INTEGER
	CTagDefClass * _ldapInteger = CreateTagDef("integer",E_TAG_INTEGER);
	//!!------------------------------------------------------------------------TYPE DEF
	//LDAPString ::= OCTET STRING
	CTagDefClass * _ldapString = CreateTagDef("ldapstring",_ldaOctetString);

	//LDAPDN ::= LDAPString
	CTagDefClass * _ldapPdn = CreateTagDef("ldapdn",_ldapString);

	//attributeValue ::= OCTET STRING
	CTagDefClass * _ldapAttributeValue = CreateTagDef("attributevalue",_ldaOctetString);

	//AttributeDescription ::= LDAPString
	CTagDefClass * _ldapAttributeDescription = CreateTagDef("attributedescription",_ldapString);

	//!!------------------------------------------------------------------------TAG DEF
	//SaslCredentials ::= SEQUENCE {
	//	mechanism    LDAPString,
	//	credentials  OCTET STRING OPTIONAL }
	CTagDefClass * _ldapSaslCredentials = CreateTagDef("sasl",E_TAG_SEQ_OF);
	_ldapSaslCredentials->AddTagDefClass("mechanism",_ldapString);
	_ldapSaslCredentials->AddTagDefClass("credentials",_ldaOctetString , 1 ); //optional

	//AuthenticationChoice ::= CHOICE {
	//	simple  [0] OCTET STRING,
	//		-- 1 and 2 reserved
	//		sasl    [3] SaslCredentials }
	CTagDefClass * _ldapAuthenticationChoice = CreateTagDef("authentication",E_TAG_CHOICE);
	_ldapAuthenticationChoice->AddTagDefClass("simple",_ldaOctetString);
	_ldapAuthenticationChoice->AddTagDefClass("sasl",_ldapSaslCredentials);


	//AttributeList ::= SEQUENCE OF SEQUENCE {
	//	type    AttributeDescription,
	//		vals    SET OF AttributeValue }
	CTagDefClass * _ldapAttributeList = CreateTagDef("attributelist",E_TAG_SEQ_OF);
	_ldapAttributeList->AddTagDefClass("type",_ldapAttributeDescription);
	_ldapAttributeList->AddTagDefClass("vals",_ldapAttributeValue);


	//!!------------------------------------------------------------------------OP DEF
	//BindRequest ::= [DIRxSBOX:APPLICATION 0] SEQUENCE {
	//	version         INTEGER (1 .. 127),
	//		name            LDAPDN,
	//		authentication  AuthenticationChoice }
	CTagDefClass * _TagBindRequest = CreateTagDef("bindrequest",APP_0_BIND_REQUEST);
	_TagBindRequest->AddTagDefClass("version",_ldapInteger);
	_TagBindRequest->AddTagDefClass("name",_ldapPdn);
	_TagBindRequest->AddTagDefClass("authentication",_ldapAuthenticationChoice);

	//	AddRequest ::= [APPLICATION 8] SEQUENCE {
	//		entry           LDAPDN,
	//			attributes      AttributeList }
	CTagDefClass * _TagAddRequest = CreateTagDef("addrequest",APP_8_ADD_REQUEST);
	_TagBindRequest->AddTagDefClass("entry",_ldapPdn);
	_TagBindRequest->AddTagDefClass("attributes",_ldapAttributeList);


	//ROOT
	m_TagLDAPSchema = CreateTagDef("LDAP",E_TAG_SEQ_OF);
	m_TagLDAPSchema->AddTagDefClass("bindrequest",_TagBindRequest);
}


CLDAPParserASN1::~CLDAPParserASN1(void)
{
	std::vector<CTagDefClass*>::iterator _it_del ;                                                                                                             
	for(_it_del = m_DefClasses.begin() ; _it_del != m_DefClasses.end(); _it_del++) 
	{
		delete (*_it_del);
	}
}

CTagDefClass* CLDAPParserASN1::CreateTagDef( const char * pName,unsigned char pTagID )
{
	CTagDefClass* _c = new CTagDefClass(pName,pTagID);
	m_DefClasses.push_back( _c );
	return _c;
}

CTagDefClass* CLDAPParserASN1::CreateTagDef( const char * pName,CTagDefClass * pTypeDef )
{
	CTagDefClass* _c = new CTagDefClass(pName,pTypeDef);
	m_DefClasses.push_back( _c );
	return _c;
}

CLDAPMessage*  CLDAPParserASN1::Parse( const char * pASN,std::string & oError )
{
	std::string _tmp_line;
	std::istringstream is(pASN);
	std::vector<std::string> _all_vec;
	while( std::getline(is, _tmp_line) )
	{
		std::vector<std::string> _line_vec;       
		boost::algorithm::trim(_tmp_line);
		boost::algorithm::to_lower(_tmp_line);
		if( _tmp_line.length() > 0 )
		{
			//skip comment
			boost::iterator_range<std::string::iterator> _word_comment = boost::algorithm::find_first(_tmp_line, "--"); 
			if( _word_comment.begin() != _word_comment.end()  )
			{
				_tmp_line = std::string(_tmp_line.begin() , _word_comment.begin() );
			}
			boost::algorithm::split(_line_vec, _tmp_line, boost::is_any_of("\t "),boost::token_compress_on);  //space or tab
			std::vector<std::string>::iterator _it_wline ;                                                                                                             
			for(_it_wline = _line_vec.begin() ; _it_wline != _line_vec.end(); _it_wline++) 
			{
				std::string _word = *_it_wline;
				boost::algorithm::replace_all(_word,"\'","\""); //replace apostrophe to quote

				boost::iterator_range<std::string::iterator> _word_equ = boost::algorithm::find_first(_word, "::="); 
				boost::iterator_range<std::string::iterator> _word_bst = boost::algorithm::find_first(_word, "{"); 
				boost::iterator_range<std::string::iterator> _word_bse = boost::algorithm::find_first(_word, "}"); 
				boost::iterator_range<std::string::iterator> _word_comma = boost::algorithm::find_first(_word, ","); 
				boost::iterator_range<std::string::iterator> _word_quot = boost::algorithm::find_first(_word, "\""); 
				std::vector<boost::iterator_range<std::string::iterator>*> _candidate_vec;
				_candidate_vec.push_back(&_word_equ);
				_candidate_vec.push_back(&_word_bst);
				_candidate_vec.push_back(&_word_bse);
				_candidate_vec.push_back(&_word_comma);
				_candidate_vec.push_back(&_word_quot);
				while( _word_equ.begin() != _word_equ.end() 
					|| _word_bst.begin() != _word_bst.end() 
					|| _word_bse.begin() != _word_bse.end()
					|| _word_comma.begin() != _word_comma.end()
					|| _word_quot.begin() != _word_quot.end())
				{
					//find who first
					std::vector<boost::iterator_range<std::string::iterator>*>::iterator _it_candidate ;          
					boost::iterator_range<std::string::iterator>* _it_first = nullptr;
					for(_it_candidate = _candidate_vec.begin() ; _it_candidate != _candidate_vec.end(); _it_candidate++) 
					{
						if( (*_it_candidate)->begin() != (*_it_candidate)->end() )
						{
							if( _it_first != nullptr )
							{
								if( _it_first->begin() > (*_it_candidate)->begin() )
								{
									_it_first = (*_it_candidate);
								}
							}
							else
							{
								_it_first = (*_it_candidate);
							}
						}
					}
					if( _it_first != nullptr )
					{
						std::string _f( _it_first->begin() ,_it_first->end() );
						std::string::iterator _liit = _word.begin();
						if( _liit < _it_first->begin() )
						{
							//found word before
							std::string _fb( _liit ,_it_first->begin() );
							boost::algorithm::erase_first(_word,_fb);
							_all_vec.push_back(_fb);
						}

						if( _f == "\""  )
						{
							boost::algorithm::erase_first(_word,_f);
							_word_quot = boost::algorithm::find_first(_word, "\""); 
							if( _word_quot.begin() == _word_quot.end() )
							{
								oError += "Syntax error : not found string quote near ";
								oError += "\n";
								oError += _word;
								oError += "\n";
								return nullptr;// not found close quote in same line
							}
							_all_vec.push_back( std::string( _word.begin() , _word_quot.begin() )  );
							_word = std::string( _word_quot.end() , _word.end() );
						}
						else
						{
							_all_vec.push_back(_f);
							boost::algorithm::erase_first(_word,_f);
						}
					}

					_word_equ = boost::algorithm::find_first(_word, "::="); 
					_word_bst = boost::algorithm::find_first(_word, "{"); 
					_word_bse = boost::algorithm::find_first(_word, "}"); 
					_word_comma = boost::algorithm::find_first(_word, ","); 
					_word_quot = boost::algorithm::find_first(_word, "\""); 
				}

				if( _word.length() > 0 )
				{
					_all_vec.push_back(_word);
				}
			}               
		}
	}

	size_t _count_bracket_open = 0;
	size_t _count_bracket_close = 0;
	//simple check syntax
	if( _all_vec.size() > 0 )
	{
		std::vector<std::string>::iterator _it_word ;                                                                                                             
		for(_it_word = _all_vec.begin() ; _it_word != _all_vec.end(); _it_word++) 
		{
			if( *_it_word == "{" )
			{
				_count_bracket_open++;
			}
			else if( *_it_word == "}" )
			{
				_count_bracket_close++;
			}
		}
		if( _count_bracket_open != _count_bracket_close )
		{
			oError += "Syntax error : not complete close bracket bracket \n";
			return nullptr; //invalid bracket 
		}

		std::string _tagname = (*(_all_vec.begin())).c_str();
		CTagDefClass * _DefClass = m_TagLDAPSchema->FindTagDefClass( _tagname.c_str() );
		if( _DefClass != nullptr )
		{
			CLDAPMessage * _msg = _DefClass->ParseFromVector( _all_vec , oError );
			return _msg;
		}
		else
		{
			oError += "Syntax error : invalid LDAP op ";
			oError += "\n";
			oError += _tagname;
			oError += "\n";
		}
	}

	return nullptr;
}
