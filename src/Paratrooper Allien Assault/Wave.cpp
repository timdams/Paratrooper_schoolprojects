//-----------------------------------------------------------------
// Wave Object
// C++ Source - Wave.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Inclusions
//-----------------------------------------------------------------
#include "StdAfx.h"
//-----------------------------------------------------------------
#include "Wave.h"
#include "MMIO.h"
#include "fstream.h"

//-----------------------------------------------------------------
// MFC Debugging Support
//-----------------------------------------------------------------
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//-----------------------------------------------------------------
// CWave Public Constructor(s)/Destructor
//-----------------------------------------------------------------
CWave::CWave() : m_dwImageLen(0), m_bResource(FALSE),
    m_pImageData(NULL)
{
}

CWave::CWave(const char* pszFileName) : m_dwImageLen(0),
    m_bResource(FALSE), m_pImageData(NULL)
{
    Create(pszFileName);
}

CWave::CWave(UINT uiResID, HMODULE hmod) : m_dwImageLen(0),
    m_bResource(TRUE), m_pImageData(NULL)
{
    Create(uiResID, hmod);
}

CWave::~CWave() {
    // Free the wave image data
    Free();
}

//-----------------------------------------------------------------
// CWave Public Methods
//-----------------------------------------------------------------
BOOL CWave::Create(const char* pszFileName)
{
    // Free any previous wave image data
    Free();

    // Flag as regular memory
    m_bResource = FALSE;

    // Open the wave file
    ifstream fileWave(pszFileName);

    // Get the file length
    fileWave.seekg(0, ios::end);
    m_dwImageLen = (DWORD)fileWave.tellg();

    // Allocate and lock memory for the image data
    m_pImageData = (BYTE*)GlobalLock(GlobalAlloc(GMEM_MOVEABLE |
        GMEM_SHARE, m_dwImageLen));
    if (!m_pImageData)
        return FALSE;

    // Read the image data from the file
    fileWave.seekg(0, ios::beg);
    fileWave.read(m_pImageData, m_dwImageLen);

    return TRUE;
}

BOOL CWave::Create(UINT uiResID, HMODULE hmod)
{
    // Free any previous wave image data
    Free();

    // Flag as resource memory
    m_bResource = TRUE;

    // Find the wave resource
    HRSRC hresInfo;
    hresInfo = FindResource(hmod, MAKEINTRESOURCE(uiResID),
        "WAVE");
    if (!hresInfo)
        return FALSE;

    // Load the wave resource
    HGLOBAL hgmemWave = LoadResource(hmod, hresInfo);

    if (hgmemWave)
    {
        // Get pointer to and length of the wave image data
        m_pImageData= (BYTE*)LockResource(hgmemWave);
        m_dwImageLen = SizeofResource(hmod, hresInfo);
    }

    return (m_pImageData ? TRUE : FALSE);
}

BOOL CWave::Play(BOOL bAsync, BOOL bLooped) const
{
    // Check validity
    if (!IsValid())
        return FALSE;

    // Play the wave
    return PlaySound((LPCSTR)m_pImageData, NULL, SND_MEMORY |
        SND_NODEFAULT | (bAsync ? SND_ASYNC : SND_SYNC) |
        (bLooped ? (SND_LOOP | SND_ASYNC) : 0));
}

BOOL CWave::GetFormat(WAVEFORMATEX& wfFormat) const
{
    // Check validity
    if (!IsValid())
        return FALSE;

    // Setup and open the MMINFO structure
    CMMMemoryIOInfo mmioInfo((HPSTR)m_pImageData, m_dwImageLen);
    CMMIO           mmio(mmioInfo);

    // Find the WAVE chunk
    CMMTypeChunk mmckParent('W','A','V','E');
    mmio.Descend(mmckParent, MMIO_FINDRIFF);

    // Find and read the format subchunk
    CMMIdChunk mmckSubchunk('f','m','t',' ');
    mmio.Descend(mmckSubchunk, mmckParent, MMIO_FINDCHUNK);
    mmio.Read((HPSTR)&wfFormat, sizeof(WAVEFORMATEX));
    mmio.Ascend(mmckSubchunk);

    return TRUE;
}

DWORD CWave::GetDataLen() const
{
    // Check validity
    if (!IsValid())
        return (DWORD)0;

    // Setup and open the MMINFO structure
    CMMMemoryIOInfo mmioInfo((HPSTR)m_pImageData, m_dwImageLen);
    CMMIO           mmio(mmioInfo);

    // Find the WAVE chunk
    CMMTypeChunk mmckParent('W','A','V','E');
    mmio.Descend(mmckParent, MMIO_FINDRIFF);

    // Find and get the size of the data subchunk
    CMMIdChunk mmckSubchunk('d','a','t','a');
    mmio.Descend(mmckSubchunk, mmckParent, MMIO_FINDCHUNK);
    return mmckSubchunk.cksize;
}

DWORD CWave::GetData(BYTE*& pWaveData, DWORD dwMaxLen) const
{
    // Check validity
    if (!IsValid())
        return (DWORD)0;

    // Setup and open the MMINFO structure
    CMMMemoryIOInfo mmioInfo((HPSTR)m_pImageData, m_dwImageLen);
    CMMIO           mmio(mmioInfo);

    // Find the WAVE chunk
    CMMTypeChunk mmckParent('W','A','V','E');
    mmio.Descend(mmckParent, MMIO_FINDRIFF);

    // Find and get the size of the data subchunk
    CMMIdChunk mmckSubchunk('d','a','t','a');
    mmio.Descend(mmckSubchunk, mmckParent, MMIO_FINDCHUNK);
    DWORD dwLenToCopy = mmckSubchunk.cksize;

    // Allocate memory if the passed in pWaveData was NULL
    if (pWaveData == NULL)
        pWaveData = (BYTE*)GlobalLock(GlobalAlloc(GMEM_MOVEABLE,
            dwLenToCopy));
    else
        // If we didn't allocate our own memory, honor dwMaxLen
        if (dwMaxLen < dwLenToCopy)
            dwLenToCopy = dwMaxLen;
    if (pWaveData)
        // Read waveform data into the buffer
        mmio.Read((HPSTR)pWaveData, dwLenToCopy);

    return dwLenToCopy;
}

//-----------------------------------------------------------------
// CWave Protected Methods
//-----------------------------------------------------------------
BOOL CWave::Free()
{
    // Free any previous wave data
    if (m_pImageData) {
        HGLOBAL  hgmemWave = GlobalHandle(m_pImageData);

        if (hgmemWave) {
            if (m_bResource)
                // Free resource
                FreeResource(hgmemWave);
            else {
                // Unlock and free memory
                GlobalUnlock(hgmemWave);
                GlobalFree(hgmemWave);
            }

            m_pImageData = NULL;
            m_dwImageLen = 0;
            return TRUE;
        }
    }
    return FALSE;
}
