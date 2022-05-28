#include "cvd/internal/io/jpeg.h"
#include <iostream>

using namespace CVD;

class CVD::JPEG::ReadPimpl
{
};

namespace
{
[[noreturn]] void error()
{
	throw Exceptions::Image_IO::MissingImageType("JPEG");
}
}

CVD::JPEG::Reader::Reader(std::istream&)
{
	error();
}

CVD::JPEG::Reader::~Reader()
{
}

std::string CVD::JPEG::Reader::datatype()
{
	error();
}

std::string CVD::JPEG::Reader::name()
{
	error();
}

bool CVD::JPEG::Reader::top_row_first()
{
	error();
};

ImageRef CVD::JPEG::Reader::size()
{
	error();
};

void CVD::JPEG::Reader::get_raw_pixel_line(unsigned char*)
{
	error();
}

void CVD::JPEG::Reader::get_raw_pixel_line(Rgb<unsigned char>*)
{
	error();
}

class CVD::JPEG::WritePimpl
{
};

CVD::JPEG::Writer::Writer(std::ostream&, ImageRef, const std::string&, const std::map<std::string, Parameter<>>&)
{
	error();
}

CVD::JPEG::Writer::~Writer()
{
}

void CVD::JPEG::Writer::write_raw_pixel_line(const unsigned char*)
{
	error();
}

void CVD::JPEG::Writer::write_raw_pixel_line(const Rgb<unsigned char>*)
{
	error();
}
