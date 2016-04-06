#ifndef LDAPCLIENTQT_H
#define LDAPCLIENTQT_H

#include <QtWidgets/QDialog>
#include "ui_ldapclientqt.h"

class HexViewUI;

class LDAPClientQT : public QDialog
{
	Q_OBJECT

public:
	LDAPClientQT(QWidget *parent = 0);
	~LDAPClientQT();

private:
	Ui::LDAPClientQTClass ui;
	HexViewUI* m_HexView;

	void a();
	public slots: void on_pushConnect_clicked();
	public slots: void on_parseButton_clicked();
	public slots: void on_parseHexButton_clicked();

	bool event(QEvent* event);
};

#endif // LDAPCLIENTQT_H
