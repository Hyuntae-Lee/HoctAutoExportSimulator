
// HoctAutoExportSimulatorDoc.h : interface of the CHoctAutoExportSimulatorDoc class
//


#pragma once


class CHoctAutoExportSimulatorDoc : public CDocument
{
protected: // create from serialization only
	CHoctAutoExportSimulatorDoc();
	DECLARE_DYNCREATE(CHoctAutoExportSimulatorDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CHoctAutoExportSimulatorDoc();

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
