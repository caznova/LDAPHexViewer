#pragma once

class CLDAPMsgCallback
{
public:
	CLDAPMsgCallback(void);
	virtual ~CLDAPMsgCallback(void);

	//--signal/slot
	typedef boost::signals2::signal<void (CLDAPMessage & data)> OnBindAccept;
	typedef OnBindAccept::slot_type OnBindAcceptType;
	virtual boost::signals2::connection SetOnBindAccept(const OnBindAcceptType & slot);

	typedef boost::signals2::signal<void (std::exception & e)> OnException;
	typedef OnException::slot_type OnExceptionType;
	virtual boost::signals2::connection SetOnException(const OnExceptionType & slot);

	typedef boost::signals2::signal<void (CLDAPMessage & data)> OnNoHandle;
	typedef OnNoHandle::slot_type OnNoHandleType;
	virtual boost::signals2::connection SetOnNoHandle(const OnNoHandleType & slot);

	typedef boost::signals2::signal<void ( const char * msg)> OnMsg;
	typedef OnMsg::slot_type OnMsgType;
	virtual boost::signals2::connection SetOnMsg(const OnMsgType & slot);

	//Base : signal on function
	typedef boost::signals2::signal<int (CLDAPMessage & data)> OnFnResponse;
	typedef OnFnResponse::slot_type OnFnResponseType;
	boost::unordered::unordered_map<unsigned int,OnFnResponseType*> m_MapFnOps;
public:
	int CallFNResponse(unsigned int OP_ID, CLDAPMessage & data );
	int MapFNResponse(unsigned int OP_ID,const OnFnResponseType & slot);

	OnBindAccept			DoBindAccept;
	OnException				DoException;
	OnNoHandle				DoNoHandle;
	OnMsg					DoMsg;
};

