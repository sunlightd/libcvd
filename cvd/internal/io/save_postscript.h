#ifndef PNM_SAVE_POSTSCRIPT_H
#define PNM_SAVE_POSTSCRIPT_H

#include <cvd/byte.h>
#include <cvd/image.h>
#include <cvd/internal/convert_pixel_types.h>
#include <cvd/internal/io/parameter.h>
#include <cvd/rgb.h>
#include <iostream>
#include <memory>
#include <string>

namespace CVD
{
namespace PS
{

	class WritePimpl;

	class Writer
	{
		public:
		Writer(std::ostream&, ImageRef size, const std::string& type, const std::map<std::string, Parameter<>>& p);
		~Writer();
		Writer(const Writer&) = delete;
		Writer(Writer&&) = delete;
		Writer& operator=(const Writer&) = delete;
		Writer& operator=(Writer&&) = delete;

		void write_raw_pixel_line(const byte*);
		void write_raw_pixel_line(const Rgb<byte>*);

		template <class Incoming>
		struct Outgoing
		{
			typedef byte type;
		};

		static const int top_row_first = 0;

		protected:
		std::unique_ptr<WritePimpl> t;
	};

	template <class C>
	struct Writer::Outgoing<Rgb<C>>
	{
		typedef Rgb<byte> type;
	};

	template <class C>
	struct Writer::Outgoing<Rgba<C>>
	{
		typedef Rgb<byte> type;
	};

	template <>
	struct Writer::Outgoing<Rgb8>
	{
		typedef Rgb<byte> type;
	};

	class eps_writer
	{
		public:
		eps_writer(std::ostream&, ImageRef size, const std::string& type, const std::map<std::string, Parameter<>>& p);
		~eps_writer();
		eps_writer(const eps_writer&) = delete;
		eps_writer(eps_writer&&) = delete;
		eps_writer& operator=(const eps_writer&) = delete;
		eps_writer& operator=(eps_writer&&) = delete;

		void write_raw_pixel_line(const byte*);
		void write_raw_pixel_line(const Rgb<byte>*);

		template <class Incoming>
		struct Outgoing
		{
			typedef typename Writer::Outgoing<Incoming>::type type;
		};

		static const int top_row_first = 1;

		protected:
		std::unique_ptr<WritePimpl> t;
	};

}
}
#endif
