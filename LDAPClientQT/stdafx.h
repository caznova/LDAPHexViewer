#pragma once 

#define WIN32_LEAN_AND_MEAN
#include <QtWidgets/QDialog>
#include <QtGui>
#include <QtWidgets>
#include <QtWidgets/QApplication>
#include <QEvent>

#include <boost/unordered_map.hpp>
#include <boost/aligned_storage.hpp>
#include <boost/array.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/signals2.hpp>
#include <boost/signals2/connection.hpp>
#include <boost/signals2/deconstruct.hpp>
#include <boost/signals2/dummy_mutex.hpp>
#include <boost/signals2/last_value.hpp>
#include <boost/signals2/mutex.hpp>
#include <boost/signals2/optional_last_value.hpp>
#include <boost/signals2/shared_connection_block.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/signals2/signal_base.hpp>
#include <boost/signals2/signal_type.hpp>
#include <boost/signals2/slot.hpp>
#include <boost/signals2/slot_base.hpp>
#include <boost/signals2/trackable.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>  
#include <boost/algorithm/string/compare.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <queue>

#define YY_NEVER_INTERACTIVE
#define YY_NO_UNISTD_H
#define yylval pcap_lval
#define _USRDLL
#define LIBPCAP_EXPORTS
#define HAVE_STRERROR
#define INET6
#define SIZEOF_CHAR 1
#define SIZEOF_SHORT 2
#define SIZEOF_INT 4
#define HAVE_ADDRINFO
#define _U_
#define HAVE_SNPRINTF
#define HAVE_VSNPRINTF
#define HAVE_REMOTE
#define HAVE_TC_API
#define _WINDOWS
#include <pcap.h>

#include "Singleton.h"
//LDAP msg
#include "type_define.h"
#include "ldap_define.h"
#include "TagDefClass.h"
#include "LDAPParserASN1.h"
#include "LDAPMessage.h"
#include "LDAPMsgCallback.h"

#include "typedef.h"
#include "LDAPHelper.h"
#include "LDAPMessageViewHex.h"

//UI
#include "MessageBoxEvent.h"
#include "ldapclientqt.h"
#include "HexBlockUI.h"
#include "HexViewUI.h"
#include "ViewLDAPUI.h"

#ifndef _DEBUG
#pragma comment(lib,"boost_system-mt.lib")
#pragma comment(lib,"boost_thread-mt.lib")
#pragma comment(lib,"boost_date_time-mt.lib")
#pragma comment(lib,"boost_iostreams-mt.lib")
#pragma comment(lib,"LDAPMsg.lib")
#pragma comment(lib,"wpcap.lib")
#pragma comment(lib,"packet.lib")
#else
#pragma comment(lib,"boost_system-mt-gd.lib")
#pragma comment(lib,"boost_thread-mt-gd.lib")
#pragma comment(lib,"boost_date_time-mt-gd.lib")
#pragma comment(lib,"boost_iostreams-mt-gd.lib")
#pragma comment(lib,"LDAPMsg_d.lib")
#pragma comment(lib,"wpcap_d.lib")
#pragma comment(lib,"packet.lib")
#endif // DEBUG 