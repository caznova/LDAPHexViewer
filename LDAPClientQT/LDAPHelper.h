#pragma once

class CLDAPMessage;
class HexViewUI;

class CLDAPHelper
{
public:
	CLDAPHelper(void);
	~CLDAPHelper(void);

	static void ViewHex( CLDAPMessage* pLdapMsg );
};

