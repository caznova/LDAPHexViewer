#pragma once

class CLDAPMessageViewHex : public CLDAPMessage
{
public:
	CLDAPMessageViewHex(CLDAPMsgCallback * pLdapCallback);
	CLDAPMessageViewHex(CLDAPMsgCallback * pLdapCallback,size_t pCap);
	~CLDAPMessageViewHex(void);

	HexViewUI* HexviewUI() const { return m_HexviewUI; }
	void HexviewUI(HexViewUI* val) { m_HexviewUI = val; }
private:
	virtual CLDAPMessage* newObject();
	virtual CLDAPMessage* newObject(size_t pCap);
public:
	virtual int ParseBER(CLDAPMessage* pParent);
	void ToHexView();
	int ToText(QString & oASN);
	int ToHex(QString & oASN);
private:
	int WriteHexViewByte(unsigned char pByte);
private:
	HexViewUI*				m_HexviewUI;
};

