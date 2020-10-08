#pragma once

#define BE_NO_DEVICES_TEXT			_T("No device were found on the machine")
#define BE_BURNER_NOT_OPEN_TEXT		_T("The Burner object is not initialized")
#define BE_DEVICE_NOT_SET_TEXT		_T("No device is selected")

enum EBurnerErrors
{
	BE_BURNER_NOT_OPEN = 0,
	BE_DEVICE_NOT_SET = 1,
	BE_NO_DEVICES = 2,
};


// Special Errors
#define DEVICE_ERROR					((uint32_t)-100)
#define DATADISC_ERROR					((uint32_t)-200)
#define VIDEODVD_ERROR					((uint32_t)-300)
#define ENGINE_ERROR					((uint32_t)-400)
#define DEVICE_ENUM_ERROR				((uint32_t)-500)


class BurnerException
{
public:
	BurnerException()
	{
		m_error			= 0;
		m_errorInfo = NULL;
	}

	~BurnerException()
	{
		if(NULL != m_errorInfo)
		{
			m_errorInfo->release();
			m_errorInfo = NULL;
		}
	}

	BurnerException(const BurnerException& other)
	{
		m_message		= other.m_message;
		m_error			= other.m_error;

		if(other.m_errorInfo)
		{
			m_errorInfo	= other.m_errorInfo->clone();
		}
		else
		{
			m_errorInfo	= NULL;
		}
	}

	BurnerException(const uint32_t error, const tstring message)
	{
		m_error = error;
		m_message = message;
		m_errorInfo	= NULL;
	}

	BurnerException(Device *pDevice)
	{
		m_errorInfo	= NULL;

		if (NULL != pDevice)
		{
			m_error = DEVICE_ERROR;
			m_errorInfo = pDevice->error()->clone();
			m_message = BuildErrorMessage(m_errorInfo);
		}
	}

	BurnerException(Engine *pEngine)
	{
		m_errorInfo	= NULL;

		if (NULL != pEngine)
		{
			m_error = ENGINE_ERROR;
			m_errorInfo = pEngine->error()->clone();
			m_message = BuildErrorMessage(m_errorInfo);
		}
	}

	BurnerException(DeviceEnum *pDeviceEnum)
	{
		m_errorInfo	= NULL;

		if (NULL != pDeviceEnum)
		{
			m_error = DEVICE_ENUM_ERROR;
			m_errorInfo = pDeviceEnum->error()->clone();
			m_message = BuildErrorMessage(m_errorInfo);
		}
	}


	BurnerException(DataDisc* pDataDisc)
	{
		m_errorInfo	= NULL;

		if (NULL != pDataDisc)
		{
			m_error = DATADISC_ERROR;
			m_errorInfo = pDataDisc->error()->clone();
			m_message = BuildErrorMessage(m_errorInfo);
		}
	}

	BurnerException(VideoDVD* pVideoDVD)
	{
		m_errorInfo	= NULL;

		if (NULL != pVideoDVD)
		{
			m_error = VIDEODVD_ERROR;
			m_errorInfo = pVideoDVD->error()->clone();
			m_message = BuildErrorMessage(m_errorInfo);
		}
	}

	BurnerException & operator= (const BurnerException & other)
	{
		if (this == &other) 
			return *this;

		if(NULL != m_errorInfo)
		{
			m_errorInfo->release();
			m_errorInfo = NULL;
		}

		m_message		= other.m_message;
		m_error			= other.m_error;

		if(other.m_errorInfo)
		{
			m_errorInfo	= other.m_errorInfo->clone();
		}

		return *this;
	}

	const tstring& get_Message() const
	{
		return m_message;
	}

	const uint32_t get_Error() const
	{
		return m_error;
	}

protected:

	tstring m_message;

	uint32_t m_error;
	const primo::burner::ErrorInfo *m_errorInfo;

protected:
	tstring BuildSystemErrorMessage(uint32_t systemError)
	{
		TCHAR tcsErrorMessage[1024];
		::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, systemError,
	 		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), tcsErrorMessage, 1024, NULL);
		return tcsErrorMessage;
	}

	tstring BuildErrorMessage(const primo::burner::ErrorInfo *errorInfo)
	{
		tstring errMsg;
		const int bufSize = 1024;
		TCHAR tcsErrorMessage[bufSize];

		switch(errorInfo->facility())
		{
		case ErrorFacility::SystemWindows:
			{
				errMsg = BuildSystemErrorMessage(errorInfo->code());
			}
			break;

		case ErrorFacility::Device:
			_sntprintf(tcsErrorMessage, bufSize, _T("Device Error: 0x%06x - %s"), errorInfo->code(), errorInfo->message());
			errMsg = tcsErrorMessage;
			break;

		case ErrorFacility::DeviceEnumerator:
			_sntprintf(tcsErrorMessage, bufSize, _T("DeviceEnumerator Error: 0x%06x - %s"), errorInfo->code(), errorInfo->message());
			errMsg = tcsErrorMessage;
			break;

		case ErrorFacility::DataDisc:
			_sntprintf(tcsErrorMessage, bufSize, _T("DataDisc Error: 0x%06x - %s"), errorInfo->code(), errorInfo->message());
			errMsg = tcsErrorMessage;
			break;

		case ErrorFacility::VideoDVD:
			_sntprintf(tcsErrorMessage, bufSize, _T("VideoDVD Error: 0x%06x - %s"), errorInfo->code(), errorInfo->message());
			errMsg = tcsErrorMessage;
			break;

		case ErrorFacility::Engine:
			_sntprintf(tcsErrorMessage, bufSize, _T("Engine Error: 0x%06x - %s"), errorInfo->code(), errorInfo->message());
			errMsg = tcsErrorMessage;
			break;

		default:
			_sntprintf(tcsErrorMessage, bufSize, _T("Error Facility: 0x%06x   Code: 0x%06x - %s"), errorInfo->facility(), errorInfo->code(), errorInfo->message());
			errMsg = tcsErrorMessage;
			break;
		}

		return errMsg;
	}
};
