#ifndef CVD_INTERNAL_IO_PNG_H
#define CVD_INTERNAL_IO_PNG_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <cvd/image.h>
#include <cvd/internal/convert_pixel_types.h>
#include <cvd/internal/load_and_save.h>

namespace CVD
{
namespace PNG
{

	using CVD::Internal::Head;
	using CVD::Internal::TypeList;

	class PNGPimpl;
	class WriterPimpl;

	class Reader
	{
		public:
		Reader(std::istream&);
		~Reader();
		Reader(const Reader&) = delete;
		Reader(Reader&&) = delete;
		Reader& operator=(const Reader&) = delete;
		Reader& operator=(Reader&&) = delete;

		ImageRef size();
		bool top_row_first();

		void get_raw_pixel_line(bool*);
		void get_raw_pixel_line(unsigned char*);
		void get_raw_pixel_line(unsigned short*);
		void get_raw_pixel_line(Rgb<unsigned char>*);
		void get_raw_pixel_line(Rgb<unsigned short>*);
		void get_raw_pixel_line(Rgba<unsigned char>*);
		void get_raw_pixel_line(Rgba<unsigned short>*);

		std::string datatype();
		std::string name();

		typedef TypeList<bool,
		    TypeList<byte,
		        TypeList<unsigned short,
		            TypeList<Rgb<byte>,
		                TypeList<Rgb<unsigned short>,
		                    TypeList<Rgba<byte>,
		                        TypeList<Rgba<unsigned short>,
		                            Head>>>>>>>
		    Types;
		static bool first_byte_matches(const int b)
		{
			return b == 0x89;
		}

		private:
		std::unique_ptr<PNGPimpl> p;
	};

	////////////////////////////////////////////////////////////////////////////////
	//
	// How to convert Misc types in to PNG compatible types
	//

	//The range is encoded un unary notation. The range is on some integer, x.
	//g1 is set if x > 1. g8 is set if x > 8 and so on.
	//This allows us to choose a type with a reasonable number of bits.
	template <int g1, int g8>
	struct IntMapper
	{
		typedef unsigned short type;
	};
	template <>
	struct IntMapper<1, 0>
	{
		typedef unsigned char type;
	};
	template <>
	struct IntMapper<0, 0>
	{
		typedef bool type;
	};

	//Mapping for integral types
	template <class ComponentIn, int is_integral>
	struct ComponentMapper_
	{
		typedef typename IntMapper<
		    (Pixel::traits<ComponentIn>::bits_used > 1),
		    (Pixel::traits<ComponentIn>::bits_used > 8)>::type type;
	};

	//Mapping for non integral types
	template <class ComponentIn>
	struct ComponentMapper_<ComponentIn, 0>
	{
		typedef unsigned short type;
	};

	template <class ComponentIn>
	struct ComponentMapper
	{
		typedef typename ComponentMapper_<ComponentIn, Pixel::traits<ComponentIn>::integral>::type type;
	};

	//Mapping for Rgbish types
	template <class ComponentIn>
	struct ComponentMapper<Rgb<ComponentIn>>
	{
		typedef Rgb<typename ComponentMapper_<ComponentIn, Pixel::traits<ComponentIn>::integral>::type> type;
	};

	template <class ComponentIn>
	struct ComponentMapper<Rgba<ComponentIn>>
	{
		typedef Rgba<typename ComponentMapper_<ComponentIn, Pixel::traits<ComponentIn>::integral>::type> type;
	};

	template <>
	struct ComponentMapper<Rgb8>
	{
		typedef Rgb8 type;
	};

	class Writer
	{
		public:
		Writer(std::ostream&, ImageRef size, const std::string& type, const std::map<std::string, Parameter<>>& p);
		~Writer();
		Writer(const Writer&) = delete;
		Writer(Writer&&) = delete;
		Writer& operator=(const Writer&) = delete;
		Writer& operator=(Writer&&) = delete;

		void write_raw_pixel_line(const bool*);
		void write_raw_pixel_line(const unsigned char*);
		void write_raw_pixel_line(const unsigned short*);
		void write_raw_pixel_line(const Rgb<unsigned char>*);
		void write_raw_pixel_line(const Rgb8*);
		void write_raw_pixel_line(const Rgb<unsigned short>*);
		void write_raw_pixel_line(const Rgba<unsigned char>*);
		void write_raw_pixel_line(const Rgba<unsigned short>*);

		template <class Incoming>
		struct Outgoing
		{
			typedef typename ComponentMapper<Incoming>::type type;
		};
		static const int top_row_first = 1;

		private:
		std::unique_ptr<WriterPimpl> p;
	};

}
}
#endif
