#pragma once

class HexBlockUI;
class HexData;
class HexViewUI;
class CLDAPMessageViewHex;

class ViewLDAPUI : public QWidget
{
	Q_OBJECT
public:
	ViewLDAPUI(CLDAPMessage* pLdpMsg,QWidget *parent = 0);
	~ViewLDAPUI(void);

	HexViewUI* getHexViewUI() const { return m_HexViewUI; }
	 virtual void closeEvent(QCloseEvent *ev);
public slots:
		void OnShowASN(QString pAsn);
		void OnSelASN(QString pAsn,unsigned int pChildIdx);
private:

	QGridLayout*			m_mainLayout;
	HexViewUI*				m_HexViewUI;	
	QTextEdit*				m_TextEdit;
	CLDAPMessageViewHex*	m_ldpMsg;
};

