#pragma once

class HexViewUI;
class HexBlockUI;
class CLDAPMessageViewHex;

#define DEFAULT_BLOCK_COLOR_R 230
#define DEFAULT_BLOCK_COLOR_G 230
#define DEFAULT_BLOCK_COLOR_B 230

class HexData
{
public:
	HexData(void) : m_BlockRef(nullptr),
		m_hexGroup(0),
		m_hexSubGroup(0),
		m_Hex(0),
		m_CustomColor(DEFAULT_BLOCK_COLOR_R,DEFAULT_BLOCK_COLOR_G,DEFAULT_BLOCK_COLOR_B),
		m_LdapMsgInst(nullptr)
	{

	}
	~HexData(void)
	{

	}

	HexBlockUI* BlockRef() const { return m_BlockRef; }
	void BlockRef(HexBlockUI* val) { m_BlockRef = val; }
	size_t HexGroup() const { return m_hexGroup; }
	void HexGroup(size_t val) { m_hexGroup = val; }
	size_t HexSubGroup() const { return m_hexSubGroup; }
	void HexSubGroup(size_t val) { m_hexSubGroup = val; }
	QColor CustomColor() const { return m_CustomColor; }
	void CustomColor(QColor val) { m_CustomColor = val; }
	unsigned char Hex() const { return m_Hex; }
	void Hex(unsigned char val) { m_Hex = val; }
	CLDAPMessageViewHex* LDAPMsgInst() const { return m_LdapMsgInst; }
	void LDAPMsgInst(CLDAPMessageViewHex* val) { m_LdapMsgInst = val; }

	void Show(HexBlockUI* val);
	void Hide();
	void Active();
	void InActive();
private:
	HexBlockUI*					m_BlockRef;
	size_t						m_hexGroup;
	size_t						m_hexSubGroup;
	unsigned char				m_Hex;
	QColor						m_CustomColor;
	CLDAPMessageViewHex*		m_LdapMsgInst;
};

class HexBlockUI :
	public QWidget
{
	Q_OBJECT
public:
	HexBlockUI(QWidget * pParent);
	~HexBlockUI(void);

	HexData* getHexData() const { return m_HexData; }
	void setHexData(HexData* val);

	int ActiveState() const { return m_ActiveState; }
	void ActiveState(int val) { m_ActiveState = val; }

	void ActiveWithColor1() { m_bColor = m_bActiveColor1; }
	void ActiveWithColor2() { m_bColor = m_bActiveColor2; }
	void ResetActiveColor() { m_bColor = m_bActiveDefult; }
signals:
	void MouseEnter( HexBlockUI* );
	void MouseLeave( HexBlockUI* );
	void LeftClick( HexBlockUI* );
	void RightClick( HexBlockUI* );
protected:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void paintEvent(QPaintEvent *event);

	void mousePressEvent(QMouseEvent * ev);
private:
	HexData*	m_HexData;
	QColor      m_DefaultBg;
	QString		m_DrawHex;
	QFont		m_fDefult;
	QBrush		m_bDefult;
	QBrush		m_bActiveDefult;
	QBrush		m_bActiveColor1;
	QBrush		m_bActiveColor2;
	QBrush		m_bColor;
	int			m_MouseOver;
	int			m_ActiveState;
};

