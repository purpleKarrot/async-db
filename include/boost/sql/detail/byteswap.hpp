#include <boost/config.hpp>
#include <boost/cstdint.hpp>
//#include <boost/detail/endian.hpp>

namespace boost
{
namespace detail
{

inline uint8_t byteswap(uint8_t value)
{
	return value;
}

inline int8_t byteswap(int8_t value)
{
	return value;
}

#ifdef BOOST_WINDOWS

#include "intrin.h"

inline uint16_t byteswap(uint16_t value)
{
	return _byteswap_ushort(value);
}

inline uint32_t byteswap(uint32_t value)
{
	return _byteswap_ulong(value);
}

inline uint64_t byteswap(uint64_t value)
{
	return _byteswap_uint64(value);
}

#else

#include <byteswap.h>

inline uint16_t byteswap(uint16_t value)
{
	return bswap_16(value);
}

inline uint32_t byteswap(uint32_t value)
{
	return bswap_32(value);
}

inline uint64_t byteswap(uint64_t value)
{
	return bswap_64(value);
}

#endif //BOOST_WINDOWS

inline int16_t byteswap(int16_t value)
{
	uint16_t tmp = byteswap(*reinterpret_cast<uint16_t*> (&value));
	return *reinterpret_cast<int16_t*> (&tmp);
}

inline int32_t byteswap(int32_t value)
{
	uint32_t tmp = byteswap(*reinterpret_cast<uint32_t*> (&value));
	return *reinterpret_cast<int32_t*> (&tmp);
}

inline int64_t byteswap(int64_t value)
{
	uint64_t tmp = byteswap(*reinterpret_cast<uint64_t*> (&value));
	return *reinterpret_cast<int64_t*> (&tmp);
}

inline float byteswap(float value)
{
	uint32_t tmp = byteswap(*reinterpret_cast<uint32_t*> (&value));
	return *reinterpret_cast<float*> (&tmp);
}

inline double byteswap(double value)
{
	uint64_t tmp = byteswap(*reinterpret_cast<uint64_t*> (&value));
	return *reinterpret_cast<double*> (&tmp);
}

} // end namespace detail
} // end namespace boost
