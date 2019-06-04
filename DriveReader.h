//---------------------------------------------------------------------------

#ifndef DriveReaderH
#define DriveReaderH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
#include "FileSystemReader.h"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TListBox *ListBoxDrive;
	TButton *ButtonDriveInfo;
	TButton *ButtonReadCluster;
	TButton *ButtonSave;
	TMemo *Memo1;
	TSaveTextFileDialog *SaveTextFileDialog1;
	TEdit *EditFrom;
	TLabel *Label1;
	TLabel *Label2;
	TEdit *EditTo;
	TButton *ButtonSearchTxt;
	void __fastcall ShowDriveInfo(TObject *Sender);
	void __fastcall ClustersRead(TObject *Sender);
	void __fastcall ListBoxClick(TObject *Sender);
	void __fastcall SaveAs(TObject *Sender);
	void __fastcall SearchTxt(TObject *Sender);
private:	// User declarations
public:		// User declarations
	void GetLetterDrive();
	HANDLE OpenDriveMethod();
	void SetFilePointerMethod(HANDLE fileHandle, ULONGLONG startOffset);
	AnsiString DriveReadMethod(HANDLE fileHandle, int size);
	__fastcall TForm1(TComponent* Owner);
    void ShowTxtCluster(int i, AnsiString c);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
