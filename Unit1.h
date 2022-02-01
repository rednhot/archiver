//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include <fstream>
#include <string>
#include <vector>


#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TButton *Button2;
	TOpenDialog *OpenDialog1;
	TImage *Image1;
	TMemo *Memo1;
	TMemo *Memo2;
	TButton *Button4;
	TButton *Button5;
	TLabel *Label1;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
private:	// User declarations
	std::ofstream debug;

	std::vector<size_t> find(const std::string&, char);
	void archive_txt(const std::string&);
	void archive_bmp(const std::string&);
	void dearchive_txt(const std::string&);
	void dearchive_bmp(const std::string&);
	std::vector<uint8_t> archive_bytes(const std::vector<uint8_t>&);
    std::vector<uint8_t> dearchive_bytes(const std::vector<uint8_t>&);

public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
