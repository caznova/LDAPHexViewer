#pragma once

class HexBlockUI;
class HexData;

class HexViewUI  : 
	public QAbstractScrollArea
{
	Q_OBJECT
public:
	enum _E_CONFIG
	{
		DEFAULT_COL = 16,
		DEFAULT_ROW = 20,
	};
public:
	HexViewUI(QWidget * pParent);
	~HexViewUI(void);

	std::vector<HexData*> getHexData() const { return m_hexData; }
	void setHexData(std::vector<HexData*> val) { m_hexData = val; }
	void ClearHexData();
	void AddHexData(HexData * pHd);
	void PopBack(int pLen);

	void ReArrange();
	void Refresh();
	void RefreshBlock(size_t nRow);
	
	HexData* createHexData();
	void destroyHexData(HexData* pHd);

	virtual void resizeEvent(QResizeEvent * ev);
protected:
	void scrollContentsBy(int dx, int dy);

public slots:
	void OnBlockActive(HexBlockUI* hBlk);
	void OnBlockInactive(HexBlockUI* hBlk);
	void OnBlockLeftClick(HexBlockUI* hBlk);
	void OnBlockRightClick(HexBlockUI* hBlk);
signals:
	void ShowASN(QString pAsn);
	void SelASN(QString pAsn,unsigned int pChildIdx);
private:
	std::vector<HexBlockUI*> m_hexBlocks;
	std::vector<HexData*> m_hexData;
	size_t m_Row;
	size_t m_Col;

	size_t m_Left;
	size_t m_Top;
	size_t m_Height;
	size_t m_Width;
	int m_viewRow;

private:
	//!---Hex data factory
	std::queue<HexData*>	m_recycle_hxdata;
};

