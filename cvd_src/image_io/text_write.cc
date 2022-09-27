#include "cvd/image_io.h"
#include "cvd/internal/io/text.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>

using namespace CVD;
using namespace CVD::TEXT;
using namespace CVD::Exceptions::Image_IO;
using namespace std;

class CVD::TEXT::WritePimpl
{
	public:
	WritePimpl(ostream& oo, ImageRef size, const string& t)
	    : o(oo)
	    , my_size(size)
	    , type(t)
	    , row(0)
	{
		if(t != "double" && t != "float")
			throw UnsupportedImageSubType("TEXT", t);
	}
	WritePimpl(const WritePimpl&) = delete;
	WritePimpl(WritePimpl&&) = delete;
	WritePimpl& operator=(const WritePimpl&) = delete;
	WritePimpl& operator=(WritePimpl&&) = delete;

	void write_raw_pixel_line(const double* d)
	{
		if(type != "double")
			throw WriteTypeMismatch(type, "double");

		//Do some sanity checking
		if(row >= (unsigned long)my_size.y)
			throw InternalLibraryError("CVD", "Write past end of image.");
		o << scientific << setw(24) << setprecision(16);

		copy(d, d + my_size.x - 1, ostream_iterator<double>(o, " "));
		o << d[my_size.x - 1] << endl;
		row++;
	}

	void write_raw_pixel_line(const float* d)
	{
		if(type != "float")
			throw WriteTypeMismatch(type, "float");

		//Do some sanity checking
		if(row >= (unsigned long)my_size.y)
			throw InternalLibraryError("CVD", "Write past end of image.");
		o << scientific << setw(15) << setprecision(8);

		copy(d, d + my_size.x - 1, ostream_iterator<float>(o, " "));
		o << d[my_size.x - 1] << endl;
		row++;
	}

	private:
	ostream& o;
	ImageRef my_size;
	string type;
	unsigned long row;
};

TEXT::Writer::Writer(ostream& o, ImageRef size, const string& type, const std::map<std::string, Parameter<>>&)
    : t(new WritePimpl(o, size, type))
{
}

TEXT::Writer::~Writer()
{
}

void TEXT::Writer::write_raw_pixel_line(const double* d)
{
	t->write_raw_pixel_line(d);
}

void TEXT::Writer::write_raw_pixel_line(const float* d)
{
	t->write_raw_pixel_line(d);
}
