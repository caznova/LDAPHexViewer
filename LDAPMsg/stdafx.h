// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <boost/unordered_map.hpp>
#include <boost/aligned_storage.hpp>
#include <boost/array.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
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
#include<boost/algorithm/string/split.hpp>  
#include <boost/algorithm/string/compare.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include "type_define.h"
#include "ldap_define.h"
#include "TagDefClass.h"
#include "LDAPParserASN1.h"
#include "LDAPMessage.h"
#include "LDAPMsgCallback.h"

#ifndef _DEBUG
#pragma comment(lib,"boost_system-mt.lib")
#pragma comment(lib,"boost_thread-mt.lib")
#pragma comment(lib,"boost_date_time-mt.lib")
#pragma comment(lib,"boost_iostreams-mt.lib")
#else
#pragma comment(lib,"boost_system-mt-gd.lib")
#pragma comment(lib,"boost_thread-mt-gd.lib")
#pragma comment(lib,"boost_date_time-mt-gd.lib")
#pragma comment(lib,"boost_iostreams-mt-gd.lib")
#endif // DEBUG 
