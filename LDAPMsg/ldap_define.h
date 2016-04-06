	enum _E_CLASS_OR //Offset 8-7
	{
		E_CO_UNIVERSAL				= 0,
		E_CO_APLICATION				= 1,
		E_CO_CONTEXT_SPECIFIC		= 2,
		E_CO_PRIVATE				= 3,
	};
	enum _E_PC //Offset 6
	{
		E_PC_PRIMATIVE				= 0,
		E_PC_CONTRUCT				= 1,
	};
	enum _E_LDAP_TAG
	{
		E_TAG_EOC					= 0x00,
		E_TAG_BOOLEAN				= 0x01,
		E_TAG_INTEGER				= 0x02,
		E_TAG_BIT_STRING			= 0x03,
		E_TAG_OCTET_STRING			= 0x04,
		E_TAG_NULL					= 0x05,
		E_TAG_OBJECT_ID				= 0x06,
		E_TAG_OBJECT_DESC			= 0x07,
		E_TAG_EXTERNAL				= 0x08,
		E_TAG_REAL					= 0x09,
		E_TAG_ENUMERATED			= 0x0A,
		E_TAG_EMBEDDED_PDV			= 0x0B,
		E_TAG_UTF8_STRING			= 0x0C,
		E_TAG_NUMERIC_STRING		= 0x12,
		E_TAG_PRINTABLE_STRING		= 0x13,
		E_TAG_TELETEXTSTRING		= 0x14,
		E_TAG_IA5_STRING			= 0x16,
		E_TAG_UTCTIME				= 0x17,
		E_TAG_GENERALIZE_TIME		= 0x18,
		E_TAG_GRAPHIC_STRING		= 0x19,
		E_TAG_VISIBLE_STRING		= 0x1A,
		E_TAG_GENERAL_STRING		= 0x1B,
		E_TAG_UNIVERSAL_STRING		= 0x1C,
		E_TAG_BMP_STRING			= 0x1E,
		E_TAG_SEQ_OF				= 0x30,
		E_TAG_SET					= 0x31,

		E_TAG_CHOICE				= 0xFF, //!My Specific 
		E_TAG_CONTEXTUAL			= 0x80, //!My Specific 
	};
	enum _E_PROTOCOL_OP
	{
		UNBIND_REQUEST			= 0x42, //0x00
		DEL_REQUEST				= 0x4A, //abcd
		APP_0_BIND_REQUEST		= 0x60,
		APP_1_BIND_RESPONSE		= 0x61,
		APP_2_UNBIND_REQUEST	= 0x62,
		APP_3_SEARCH_REQUEST	= 0x63,  //* If doing search without bind first,  * x500.arc.nasa.gov returns tag [APPLICATION 3] * in Search Response. Gee.
		APP_4_SEARCH_RESPONSE	= 0x64,
		APP_5_SEARCH_RESULT		= 0x65,
		APP_6_MOD_REQUEST		= 0x66,
		APP_7_MOD_RESPONSE		= 0x67,
		APP_8_ADD_REQUEST		= 0x68,
		APP_9_ADD_RESPONSE		= 0x69,
		APP_10_DEL_REQUEST		= 0x6A,
		APP_11_DEL_RESPONSE		= 0x6B,
		APP_11_MOD_RDN_REQUEST	= 0x6C,
		APP_13_MOD_RDN_RESPONSE = 0x6D,
		APP_14_COMP_REQUEST		= 0x6E,
		APP_15_COMP_RESPONSE	= 0x6F,
		APP_19_SEARCH_RESULT_REF = 0x73, 

		APP_ABANDON_REQUEST		= 0x50, //0x0[1..4][0..(2^31) - 1] 
		APP_EXTEND_REQUEST		= 0x77, 
		APP_EXTEND_RESPONSE		= 0x78, 
		// ^above are Ber sequence
		APP_16					 = 0x80,  //64 + 16BER Integer
		//MS SPEC
		CONTEXT_SPEC_5			= 0x85,  //64 + 16BER Integer - (a) Handle Microsoft v3 referral bugs! (Response) -  (b) Handle Microsoft v3 supportedVersion in Bind response
		CONTEXT_SPEC_7			= 0x87, //BER OctetString - Handle Microsoft Filter "present" in search request.
		CONTEXT_SPEC_10			= 0x8A, //BER OctetString - Handle extended response
		CONTEXT_SPEC_11			= 0x8B, //BER OctetString - Handle extended response
		CONTEXT_SPEC_CON_3		= 0xA3, //BER sequence - Handle Microsoft v3 sasl bind request
		CONTEXT_SPEC_CON_7		= 0xA7, //BER sequence - Handle Microsoft v3 serverCred in bind response. MS encodes it as SEQUENCE OF while it should be CHOICE OF.
		CONTEXT_SPEC_CON_0_0	= 0xA0, //BER sequence - v3 Server Control. SEQUENCE of SEQUENCE of {OID  [critical] [value]} THIS IS WARNING FROM PASSWORD CONTROL
		CONTEXT_SPEC_CON_0_1	= 0x81, //BER Integer - v3 Server Control. SEQUENCE of SEQUENCE of {OID  [critical] [value]} THIS IS WARNING FROM PASSWORD CONTROL
		CONTEXT_SPEC_CON_0_2	= 0xA1, //BER sequence - v3 Server Control. SEQUENCE of SEQUENCE of {OID  [critical] [value]}
	};
	enum { 
		INITIAL_DATA_BUFFER = 64,
		GROWTH_SIZE = 64,

		MAX_DEFINITE_LENGTH = 0x7F,
		INDEFINITE_LENGTH	= 0x80,
	};
	enum { 
		MSG_PARSE_ERROR = 0,
		MSG_PARSE_NEXT = 1,
		MSG_PARSE_STOP = 2,
	};
	typedef enum _E_RECV_MESSAGE_STATE
	{
		ERMS_NONE,
		ERMS_MSG_TYPE,
		ERMS_WAIT_BODY,
		ERMS_COMPLETED
	}E_PROCESS_MESSAGE_STATE;