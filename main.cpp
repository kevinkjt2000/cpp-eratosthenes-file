#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <fstream>
#include <limits>

namespace primes_file {
	using std::ios;
	const char primes_file[] = "my-primes";

	void erase_contents() {
		std::fstream file(primes_file, ios::out | ios::trunc);
	}

	std::streamsize get_size() {
		std::fstream file(primes_file, ios::in | ios::binary);
		file.ignore(std::numeric_limits<std::streamsize>::max());
		std::streamsize fsize = file.gcount();
		return fsize;
	}

	std::fstream get_eof_read_write_handle() {
		return std::fstream(primes_file, ios::out | ios::in | ios::binary | ios::ate);
	}

	std::fstream get_read_handle() {
		return std::fstream(primes_file, ios::in | ios::binary);
	}
}

bool is_prime(uint64_t x) {
	std::streamsize fsize = primes_file::get_size();
	std::streamsize location_of_prime = (x / 8) + 1;

	if(location_of_prime > fsize) {
		//need to gen some more primes because we don't have it yet
		std::fstream file = primes_file::get_eof_read_write_handle();
		char assume_prime = 0xF;
		for(std::streamsize num_bytes_to_add = location_of_prime - fsize; num_bytes_to_add > 0; --num_bytes_to_add) {
			file.write(&assume_prime, 1);
		}
	}

	//the prime number is less than the file size so we should be able to seek to it's bit
	std::fstream file = primes_file::get_read_handle();
	file.seekg(location_of_prime - 1, std::ios::beg);
	char byte = file.get();
	bool primality = byte & (1 << (x % 8));

	return primality;
}

TEST_CASE( "these numbers are prime", "[is_prime]" ) {
	primes_file::erase_contents();
	REQUIRE( is_prime(2) );
	REQUIRE( is_prime(3) );
}

TEST_CASE( "file size does not grow when a previous number is queried", "is_prime" ) {
	primes_file::erase_contents();

	is_prime(42);

	std::streampos fsize_start = primes_file::get_size();

	is_prime(42);

	std::streampos fsize_end = primes_file::get_size();
	REQUIRE( fsize_end == fsize_start );
}

TEST_CASE( "file size grows when a number larger than the sieve is queried", "is_prime" ) {
	primes_file::erase_contents();
	std::streampos fsize_start = primes_file::get_size();

	is_prime(2);

	std::streampos fsize_end = primes_file::get_size();
	REQUIRE( fsize_end > fsize_start );

	is_prime(42);

	std::streampos fsize_final = primes_file::get_size();
	REQUIRE( fsize_final > fsize_end );
}
