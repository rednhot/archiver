//---------------------------------------------------------------------------

#include <vcl.h>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <vector>


#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	///must archive .bmp and .txt files with rle algorithm

	if(OpenDialog1 -> Execute())
	{
		std::string path = AnsiString(OpenDialog1 -> FileName).c_str();
		std::string ext = "";
		{
			int ext_pos = find(path, '.').back();
			ext = path.substr(ext_pos, path.size() - ext_pos);
		}

		if(ext == ".bmp")
			archive_bmp(path);
		else
			archive_txt(path);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
	if(OpenDialog1 -> Execute())
	{

		std::string path = AnsiString(OpenDialog1 -> FileName).c_str();
		std::string ext = "";
		{
			auto dots = find(path, '.');
			if(dots.size() > 1)
				ext = path.substr(*(dots.rbegin()+1), dots.back() - *(dots.rbegin()+1));
		}

		if(ext == ".bmp")
			dearchive_bmp(path);
		else
			dearchive_txt(path);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	//debug = std::ofstream("___debug___.txt");
}
//---------------------------------------------------------------------------
std::vector<size_t> TForm1::find(const std::string &str, char ch)
{
	std::vector<size_t> res;
	res.reserve(str.size());

	int pos = str.size();
	for(size_t i = 0; i < str.size(); ++i)
		if(str[i] == ch)
			res.push_back(i);

	return res;
}
//---------------------------------------------------------------------------
void TForm1::archive_txt(const std::string &path)
{
	std::ifstream input_file(path, std::ios::binary);
	std::ofstream archived_file(path + ".rle", std::ios::binary);

	std::vector<uint8_t> raw_data;
	raw_data.reserve(1 << 15);

	while(input_file)
	{
		uint8_t ch = input_file.get();
		if(ch == 0xff)
			break;
		raw_data.push_back(ch);
	}


	auto archived_data = archive_bytes(raw_data);

	for(auto &i : archived_data)
		archived_file << i;
}
//---------------------------------------------------------------------------
void TForm1::dearchive_txt(const std::string &path)
{
	std::ifstream archived_file(path, std::ios::binary);
	std::ofstream dearchived_file;
	{
		auto dots = find(path, '.');
		dearchived_file.open(path.substr(0, dots.back()-1), std::ios::binary);
	}


	std::vector<uint8_t> raw_data;
	raw_data.reserve(1 << 15);

	while(archived_file)
	{
		uint8_t ch = archived_file.get();

		if(ch == 0xff)
			break;
		raw_data.push_back(ch);
	}

	auto dearchived_bytes = dearchive_bytes(raw_data);

	for(auto &i : dearchived_bytes)
		dearchived_file << i;
}
//---------------------------------------------------------------------------
void TForm1::archive_bmp(const std::string &path)
{
	std::ofstream output_file(path + ".rle", std::ios::binary);

	Image1 -> Picture -> LoadFromFile(path.c_str());

	auto bmp = new Graphics::TBitmap;
	bmp -> LoadFromFile(path.c_str());
	Image1 -> Picture -> LoadFromFile(path.c_str());

	std::vector<uint8_t> data;
	data.reserve(bmp -> Height * bmp -> Width);

	for(int color = 0; color < 3; ++color)
		for(size_t i = 0; i < bmp -> Height; ++i)
			for(size_t j = 0; j < bmp -> Width; ++j)
				data.push_back(bmp -> Canvas -> Pixels[j][i] >> 8 * color
								& (1u << 8) - 1);
	auto archived_data = archive_bytes(data);

	for(auto &i : archived_data)
		output_file << i;


	output_file << uint8_t(bmp -> Width >> 8 & (1u << 8)-1) << uint8_t(bmp -> Width & (1u << 8)-1);
	output_file << uint8_t(bmp -> Height >> 8 & (1u << 8)-1) << uint8_t(bmp -> Height & (1u << 8)-1);
	delete bmp;
}
//---------------------------------------------------------------------------
void TForm1::dearchive_bmp(const std::string &path)
{
	std::ifstream input_file(path, std::ios::binary);

	std::vector<uint8_t> data;
	data.reserve(1 << 15);

	while(input_file)
	{
		uint8_t ch = input_file.get();

		data.push_back(ch);
	}
	data.pop_back();

	auto result = new Graphics::TBitmap;


	size_t bmp_width = (*(data.rbegin()+3) << 8u) + *(data.rbegin()+2);
	size_t bmp_height = (*(data.rbegin()+1) << 8u) + *data.rbegin();
	result -> Width = bmp_width;
	result -> Height = bmp_height;

	for(int i = 0; i < 4; ++i)
		data.pop_back();

	auto dearchived_bytes = dearchive_bytes(data);

	for(int i = 0; i < bmp_height; ++i)
		for(int j = 0; j < bmp_width; ++j)
			result -> Canvas -> Pixels[j][i] = TColor(0);

	for(int color = 0; color < 3; ++color)
		for(int i = 0; i < bmp_height; ++i)
			for(int j = 0; j < bmp_width; ++j)
			{
				uint32_t tmp = result -> Canvas -> Pixels[j][i];
				tmp |= dearchived_bytes[
					color*bmp_width*bmp_height + i*bmp_width + j] << 8*color;

				result -> Canvas -> Pixels[j][i] = TColor(tmp);
            }

	std::string save_path = path.substr(0, find(path, '.').back());
	result -> SaveToFile(save_path.c_str());
	Image1 -> Picture -> LoadFromFile(save_path.c_str());
	delete result;
}
//---------------------------------------------------------------------------
std::vector<uint8_t> TForm1::archive_bytes(const std::vector<uint8_t> &input)
{
	if(input.empty())
		return {};

	std::vector<uint8_t> result;
	result.reserve(input.size());

	struct Block
	{
		uint8_t _val;
		size_t _n;
	};

	std::vector<Block> blocks;
	{
		Block cur{input[0], 1};

		for(size_t i = 1; i < input.size(); ++i)
		{
			if(input[i] == cur._val)
				++cur._n;
			else
			{
				blocks.push_back(cur);
				cur._val = input[i];
				cur._n = 1;
			}
		}
		blocks.push_back(cur);
	}

	std::string buf;
	buf.reserve(blocks.size());

	auto flush_buf = [&]() -> void
	{
		int ptr = 0;
		while(ptr != buf.size())
		{
			uint8_t times = std::min(128u, buf.size() - ptr);

			uint8_t code_symbol = (1u << 7) + (times - 1);
			result.push_back(code_symbol);
			for(uint8_t i = 0; i < times; ++i)
				result.push_back(buf[ptr++]);
		}

		buf = "";
	};

	for(size_t i = 0; i < blocks.size(); ++i)
	{
		if(blocks[i]._n > 1)
		{
			///sign bit is set to
			///0 -> character is repeated n times
			///1 -> series of single characters, consisting of n characters

			///?0x0 -> 1
			///?0x7f -> 128

			flush_buf();

			while(blocks[i]._n)
			{
				uint8_t times = std::min(128u, blocks[i]._n);
				blocks[i]._n -= times;

				uint8_t code_symbol = times - 1;
				result.push_back(code_symbol);
				result.push_back(blocks[i]._val);
			}
		}
		else
			buf += blocks[i]._val;

	}
	flush_buf();

	return result;
}
//---------------------------------------------------------------------------
std::vector<uint8_t> TForm1::dearchive_bytes(const std::vector<uint8_t> &archived_bytes)
{
	std::vector<uint8_t> result;
	result.reserve(1 << 15);


	size_t ptr = 0;
	while(archived_bytes.size() != ptr)
	{
		uint8_t code_symbol = archived_bytes[ptr++];

		uint8_t times = (code_symbol & (1u << 7) - 1) + 1;

		uint8_t character = -1;
		if(code_symbol & 1u << 7)
		{
			for(uint8_t i = 0; i < times; ++i)
			{
				character = archived_bytes[ptr++];
				result.push_back(character);
			}
		}
		else
		{
			character = archived_bytes[ptr++];
			for(uint8_t i = 0; i < times; ++i)
				result.push_back(character);
		}
	}

	return result;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
	Memo2 -> Clear();

	std::vector<uint8_t> raw_data;
	raw_data.reserve(1 << 15);

	for(size_t i = 0; i < Memo1 -> Lines -> Count; ++i)
	{
		std::string buf = AnsiString(Memo1 -> Lines -> Strings[i]).c_str();
		buf += '\n';

		for(auto &ch : buf)
			raw_data.push_back(ch);
	}
	raw_data.pop_back();


	auto archived_text = archive_bytes(raw_data);

	std::string all;
	all.resize(archived_text.size());

	for(size_t i = 0; i < archived_text.size(); ++i)
		all[i] = archived_text[i];

	Memo2 -> Lines -> Add(all.c_str());

	Label1 -> Caption = FloatToStr(double(raw_data.size())/archived_text.size());
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
	Memo1 -> Clear();

	std::vector<uint8_t> archived_text;
	archived_text.reserve(1 << 15);

	for(size_t i = 0; i < Memo2 -> Lines -> Count; ++i)
	{
		std::string buf = AnsiString(Memo2 -> Lines -> Strings[i]).c_str();
		buf += '\n';

		for(auto &ch : buf)
			archived_text.push_back(ch);
	}
	archived_text.pop_back();

	auto dearchived_text = dearchive_bytes(archived_text);

	std::string buf = "";
	auto flush_buf = [&]() -> void
	{
		Memo1 -> Lines -> Add(buf.c_str());
		buf = "";
	};

	for(size_t i = 0; i < dearchived_text.size(); ++i)
	{
		if(dearchived_text[i] == '\n')
			flush_buf();
		else
			buf += dearchived_text[i];
	}
	flush_buf();

	Label1 -> Caption =  FloatToStr(double(dearchived_text.size())
									/ archived_text.size());

}
//---------------------------------------------------------------------------
//        .--.       .--.
//    _  `    \     /    `  _
//     `\.===. \.^./ .===./`
//            \/`"`\/
//         ,  | y2k |  ,
//        / `\|;-.-'|/` \
//       /    |::\  |    \
//    .-' ,-'`|:::; |`'-, '-.
//        |   |::::\|   |
//        |   |::::;|   |
//        |   \:::://   |
//        |    `.://'   |
//       .'             `.
//    _,'                 `,_
