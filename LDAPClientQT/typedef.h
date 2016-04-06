#pragma once

enum _E_CUSTOM_EVENT
{
	ECE_MESSAGEBOX = QEvent::User + 10,
	ECE_COUNT,
};

typedef enum _E_MSG_TYPE
{
	EMT_WARN,
	EMT_INFO,
	EMT_ERROR,
	EMT_QUESTION
}E_MSG_TYPE;