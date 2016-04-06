#include "stdafx.h"

LDAPClientQT::LDAPClientQT(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

LDAPClientQT::~LDAPClientQT()
{
	Singleton::Instance().IO()->stop();
}

void LDAPClientQT::a()
{
	pcap_if_t *alldevs;
	pcap_if_t *d;
	pcap_t *fp;
	int res;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	u_int netmask;
	struct bpf_program fcode;
	char errbuf[PCAP_ERRBUF_SIZE+1];
	if (pcap_findalldevs_ex("rpcap://192.168.220.129:1113", NULL, &alldevs, errbuf) == -1)
	{
		MessageBoxEvent * _err_msg = new MessageBoxEvent( EMT_ERROR , "Connection failed" , errbuf, QMessageBox::Ok );
		Singleton::Instance().PostMainEvent( _err_msg );
	}
	else
	{
		for(d=alldevs;d;d=d->next)
		{
			OutputDebugStringA(d->name);
			OutputDebugStringA("\n");
			if ( (fp= pcap_open(d->name, 100 /*snaplen*/,
				PCAP_OPENFLAG_PROMISCUOUS /*flags*/,
				20 /*read timeout*/,
				NULL /* remote authentication */,
				errbuf)
				) == NULL)
			{
				return ;
			}
			else
			{
				netmask = 0xffffff00;
				if (pcap_compile(fp, &fcode, "tcp", 1, netmask) <0 )
				{
					return;
				}
				else
				{
					if (pcap_setfilter(fp, &fcode)<0)
					{
						pcap_close(fp);
						return;
					}
				}
			}
		}

		/* Read the packets */
		while((res = pcap_next_ex( fp, &header, &pkt_data)) >= 0)
		{

			if(res == 0)
				/* Timeout elapsed */
					continue;

			/* print pkt timestamp and pkt len */
			char tmp[256];
			sprintf(tmp,"\n %ld:%ld (%ld) \n", header->ts.tv_sec, header->ts.tv_usec, header->len);			
			OutputDebugStringA(tmp);

			/* Print the packet */
			for (size_t i=1; (i < header->caplen + 1 ) ; i++)
			{
				sprintf(tmp,"%.2x ", pkt_data[i-1]);
				OutputDebugStringA(tmp);
				if ( (i % 16) == 0) OutputDebugStringA("\n");;
			}

			printf("\n\n");		
		}

		pcap_close(fp);
		pcap_freealldevs(alldevs);
	}

}

bool LDAPClientQT::event(QEvent* event)
{
	 if (event->type() == (QEvent::Type)ECE_MESSAGEBOX) 
	 {
		 QMessageBox msgBox;
		 MessageBoxEvent *ke = static_cast<MessageBoxEvent *>(event);
		 msgBox.setWindowTitle(ke->caption);
		 msgBox.setText(ke->message);
		 msgBox.setStandardButtons(static_cast<QMessageBox::StandardButtons>(ke->button0 | ke->button1 | ke->button2));
		 switch( ke->kind )
		 {
		 case EMT_WARN:
			 {
				 msgBox.setIcon(QMessageBox::Warning);
				 break;
			 }
		 case EMT_INFO:
			 {
				 msgBox.setIcon(QMessageBox::Information);
				 break;
			 }
		 case EMT_ERROR:
			 {
				 msgBox.setIcon(QMessageBox::Critical);
				 break;
			 }
		 case EMT_QUESTION:
			 {
				 msgBox.setIcon(QMessageBox::Question);
				 break;
			 }
		 }
		 ke->resp = (unsigned int)msgBox.exec();
		 return true;
	 }

	  return QWidget::event(event);
}

void LDAPClientQT::on_pushConnect_clicked()
{
	//Singleton::Instance().IO()->post( boost::bind( &LDAPClientQT::a , this ) );
}

void LDAPClientQT::on_parseButton_clicked()
{
	CLDAPParserASN1 _parser_asn;
	std::string _parse_error;
	QString _asn = ui.txtASN->toPlainText();
	char * _asn_c = new char[_asn.length() + 1];
	for(size_t i = 0 ; i < _asn.length() ; ++i )
	{
		_asn_c[i] = _asn.at(i).toLatin1();
	}
	_asn_c[_asn.length() ] = 0;
	CLDAPMessage* _ret = _parser_asn.Parse( (const char*)_asn_c ,_parse_error);
	delete [] _asn_c;
	if( _ret == nullptr )
	{
		MessageBoxEvent * _err_msg = new MessageBoxEvent( EMT_ERROR , "Parse failed" , _parse_error.c_str(), QMessageBox::Ok );
		Singleton::Instance().PostMainEvent( _err_msg );
	}
	else
	{
		_ret->IsEncoder(1);
		int ret = _ret->EncodeBER(_parse_error);
		if( ret )
		{
			static int _msgid = 0;
			CLDAPMessage * _ldp_bind_req = new CLDAPMessage(nullptr); //send buffer holder
			_ldp_bind_req->BuildMessage( E_TAG_SEQ_OF , ++_msgid ,  *_ret );
			_ldp_bind_req->ParseBER(nullptr);
			CLDAPHelper::ViewHex( _ldp_bind_req );
			delete _ldp_bind_req;
		}
		delete _ret;
	}
}

void LDAPClientQT::on_parseHexButton_clicked()
{
	CLDAPParserASN1 _parser_asn;
	std::string _parse_error;
	QString _hex = ui.txtHex->toPlainText();
	char * _hex_c = new char[_hex.length() + 1];
	for(size_t i = 0 ; i < _hex.length() ; ++i )
	{
		_hex_c[i] = _hex.at(i).toLatin1();
	}
	_hex_c[_hex.length() ] = 0;
	CLDAPMessage* _ret = new CLDAPMessage(nullptr);
	if( _ret->WriteFromHex( _hex_c ) )
	{
		_ret->ParseBER(nullptr);
		CLDAPHelper::ViewHex( _ret );
		delete _ret;
	}
	else
	{
		MessageBoxEvent * _err_msg = new MessageBoxEvent( EMT_ERROR , "Parse failed" , "Error hex string" , QMessageBox::Ok );
		Singleton::Instance().PostMainEvent( _err_msg );
	}
	delete [] _hex_c;
}
