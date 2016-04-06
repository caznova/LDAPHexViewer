#include "stdafx.h"

CLDAPMsgCallback::CLDAPMsgCallback(void)
{

}

CLDAPMsgCallback::~CLDAPMsgCallback(void)
{
	boost::unordered::unordered_map<unsigned int,OnFnResponseType*>::iterator  _it=   m_MapFnOps.begin();
	while( _it != m_MapFnOps.end() )
	{
		delete _it->second;
		++_it;
	}
	m_MapFnOps.clear();
}

boost::signals2::connection CLDAPMsgCallback::SetOnBindAccept( const OnBindAcceptType & slot )
{
	return DoBindAccept.connect(slot);
}

boost::signals2::connection CLDAPMsgCallback::SetOnException( const OnExceptionType & slot )
{
	return DoException.connect(slot);
}

boost::signals2::connection CLDAPMsgCallback::SetOnNoHandle( const OnNoHandleType & slot )
{
	return DoNoHandle.connect(slot);
}

boost::signals2::connection CLDAPMsgCallback::SetOnMsg( const OnMsgType & slot )
{
	return DoMsg.connect(slot);
}

int CLDAPMsgCallback::CallFNResponse( unsigned int OP_ID, CLDAPMessage & data )
{
	if( m_MapFnOps.find(OP_ID) != m_MapFnOps.end() )
	{
		OnFnResponse _sig;
		_sig.connect( *(m_MapFnOps[OP_ID]) );
		return *_sig(data);
	}
	else
	{
		DoNoHandle(data);
	}
	return 0;
}

int CLDAPMsgCallback::MapFNResponse( unsigned int OP_ID,const OnFnResponseType & slot )
{
	if( m_MapFnOps.find(OP_ID) == m_MapFnOps.end() )
	{
		m_MapFnOps[OP_ID] = new OnFnResponseType(slot);
	}
	return 1;
}
