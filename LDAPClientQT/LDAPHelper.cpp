#include "stdafx.h"


CLDAPHelper::CLDAPHelper(void) 
{
}


CLDAPHelper::~CLDAPHelper(void)
{

}

void CLDAPHelper::ViewHex( CLDAPMessage* pLdapMsg )
{
	ViewLDAPUI * _hDlg = new ViewLDAPUI( pLdapMsg , nullptr);
	_hDlg->show();
}
