/*
 * Copyright (c) 2021 ASPEED
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Hash shell commands.
 */

#include <shell/shell.h>
#include <drivers/jtag.h>
#include <stdlib.h>
#include <crypto/rsa_structs.h>
#include <crypto/rsa.h>
#include <device.h>

#ifdef CONFIG_RSA_ASPEED
#define RSA_DRV_NAME DT_LABEL(DT_INST(0, aspeed_rsa))
#endif

struct rsa_testvec {
	struct rsa_key k;
	char *p;
	char *c;
	unsigned int p_size;
	unsigned int c_size;
};

static struct rsa_testvec rsa_tv[] = {
	{
		.k = {
			.m =
			"\xDB\x10\x1A\xC2\xA3\xF1\xDC\xFF\x13\x6B\xED\x44\xDF\xF0\x02\x6D"
			"\x13\xC7\x88\xDA\x70\x6B\x54\xF1\xE8\x27\xDC\xC3\x0F\x99\x6A\xFA"
			"\xC6\x67\xFF\x1D\x1E\x3C\x1D\xC1\xB5\x5F\x6C\xC0\xB2\x07\x3A\x6D"
			"\x41\xE4\x25\x99\xAC\xFC\xD2\x0F\x02\xD3\xD1\x54\x06\x1A\x51\x77"
			"\xBD\xB6\xBF\xEA\xA7\x5C\x06\xA9\x5D\x69\x84\x45\xD7\xF5\x05\xBA"
			"\x47\xF0\x1B\xD7\x2B\x24\xEC\xCB\x9B\x1B\x10\x8D\x81\xA0\xBE\xB1"
			"\x8C\x33\xE4\x36\xB8\x43\xEB\x19\x2A\x81\x8D\xDE\x81\x0A\x99\x48"
			"\xB6\xF6\xBC\xCD\x49\x34\x3A\x8F\x26\x94\xE3\x28\x82\x1A\x7C\x8F"
			"\x59\x9F\x45\xE8\x5D\x1A\x45\x76\x04\x56\x05\xA1\xD0\x1B\x8C\x77"
			"\x6D\xAF\x53\xFA\x71\xE2\x67\xE0\x9A\xFE\x03\xA9\x85\xD2\xC9\xAA"
			"\xBA\x2A\xBC\xF4\xA0\x08\xF5\x13\x98\x13\x5D\xF0\xD9\x33\x34\x2A"
			"\x61\xC3\x89\x55\xF0\xAE\x1A\x9C\x22\xEE\x19\x05\x8D\x32\xFE\xEC"
			"\x9C\x84\xBA\xB7\xF9\x6C\x3A\x4F\x07\xFC\x45\xEB\x12\xE5\x7B\xFD"
			"\x55\xE6\x29\x69\xD1\xC2\xE8\xB9\x78\x59\xF6\x79\x10\xC6\x4E\xEB"
			"\x6A\x5E\xB9\x9A\xC7\xC4\x5B\x63\xDA\xA3\x3F\x5E\x92\x7A\x81\x5E"
			"\xD6\xB0\xE2\x62\x8F\x74\x26\xC2\x0C\xD3\x9A\x17\x47\xE6\x8E\xAB",
			.d =
			"\x52\x41\xF4\xDA\x7B\xB7\x59\x55\xCA\xD4\x2F\x0F\x3A\xCB\xA4\x0D"
			"\x93\x6C\xCC\x9D\xC1\xB2\xFB\xFD\xAE\x40\x31\xAC\x69\x52\x21\x92"
			"\xB3\x27\xDF\xEA\xEE\x2C\x82\xBB\xF7\x40\x32\xD5\x14\xC4\x94\x12"
			"\xEC\xB8\x1F\xCA\x59\xE3\xC1\x78\xF3\x85\xD8\x47\xA5\xD7\x02\x1A"
			"\x65\x79\x97\x0D\x24\xF4\xF0\x67\x6E\x75\x2D\xBF\x10\x3D\xA8\x7D"
			"\xEF\x7F\x60\xE4\xE6\x05\x82\x89\x5D\xDF\xC6\xD2\x6C\x07\x91\x33"
			"\x98\x42\xF0\x02\x00\x25\x38\xC5\x85\x69\x8A\x7D\x2F\x95\x6C\x43"
			"\x9A\xB8\x81\xE2\xD0\x07\x35\xAA\x05\x41\xC9\x1E\xAF\xE4\x04\x3B"
			"\x19\xB8\x73\xA2\xAC\x4B\x1E\x66\x48\xD8\x72\x1F\xAC\xF6\xCB\xBC"
			"\x90\x09\xCA\xEC\x0C\xDC\xF9\x2C\xD7\xEB\xAE\xA3\xA4\x47\xD7\x33"
			"\x2F\x8A\xCA\xBC\x5E\xF0\x77\xE4\x97\x98\x97\xC7\x10\x91\x7D\x2A"
			"\xA6\xFF\x46\x83\x97\xDE\xE9\xE2\x17\x03\x06\x14\xE2\xD7\xB1\x1D"
			"\x77\xAF\x51\x27\x5B\x5E\x69\xB8\x81\xE6\x11\xC5\x43\x23\x81\x04"
			"\x62\xFF\xE9\x46\xB8\xD8\x44\xDB\xA5\xCC\x31\x54\x34\xCE\x3E\x82"
			"\xD6\xBF\x7A\x0B\x64\x21\x6D\x88\x7E\x5B\x45\x12\x1E\x63\x8D\x49"
			"\xA7\x1D\xD9\x1E\x06\xCD\xE8\xBA\x2C\x8C\x69\x32\xEA\xBE\x60\x71",
			.e = "\x01\x00\x01",
			.m_bits = 2048,
			.d_bits = 2048,
			.e_bits = 24
		},
		.p = "\x54\x85\x9b\x34\x2c\x49\xea\x2a",
		.c =
		"\xb2\x97\x76\xb4\xae\x3e\x38\x3c\x7e\x64\x1f\xcc\xa2\x7f\xf6\xbe"
		"\xcf\x49\xbc\x48\xd3\x6c\x8f\x0a\x0e\xc1\x73\xbd\x7b\x55\x79\x36"
		"\x0e\xa1\x87\x88\xb9\x2c\x90\xa6\x53\x5e\xe9\xef\xc4\xe2\x4d\xdd"
		"\xf7\xa6\x69\x82\x3f\x56\xa4\x7b\xfb\x62\xe0\xae\xb8\xd3\x04\xb3"
		"\xac\x5a\x15\x2a\xe3\x19\x9b\x03\x9a\x0b\x41\xda\x64\xec\x0a\x69"
		"\xfc\xf2\x10\x92\xf3\xc1\xbf\x84\x7f\xfd\x2c\xae\xc8\xb5\xf6\x41"
		"\x70\xc5\x47\x03\x8a\xf8\xff\x6f\x3f\xd2\x6f\x09\xb4\x22\xf3\x30"
		"\xbe\xa9\x85\xcb\x9c\x8d\xf9\x8f\xeb\x32\x91\xa2\x25\x84\x8f\xf5"
		"\xdc\xc7\x06\x9c\x2d\xe5\x11\x2c\x09\x09\x87\x09\xa9\xf6\x33\x73"
		"\x90\xf1\x60\xf2\x65\xdd\x30\xa5\x66\xce\x62\x7b\xd0\xf8\x2d\x3d"
		"\x19\x82\x77\xe3\x0a\x5f\x75\x2f\x8e\xb1\xe5\xe8\x91\x35\x1b\x3b"
		"\x33\xb7\x66\x92\xd1\xf2\x8e\x6f\xe5\x75\x0c\xad\x36\xfb\x4e\xd0"
		"\x66\x61\xbd\x49\xfe\xf4\x1a\xa2\x2b\x49\xfe\x03\x4c\x74\x47\x8d"
		"\x9a\x66\xb2\x49\x46\x4d\x77\xea\x33\x4d\x6b\x3c\xb4\x49\x4a\xc6"
		"\x7d\x3d\xb5\xb9\x56\x41\x15\x67\x0f\x94\x3c\x93\x65\x27\xe0\x21"
		"\x5d\x59\xc3\x62\xd5\xa6\xda\x38\x26\x22\x5e\x34\x1c\x94\xaf\x98",
		.p_size = 8,
		.c_size = 256,
	},
	{
		.k = {
			.m =
			"\x00\xAA\x36\xAB\xCE\x88\xAC\xFD\xFF\x55\x52\x3C\x7F\xC4\x52\x3F"
			"\x90\xEF\xA0\x0D\xF3\x77\x4A\x25\x9F\x2E\x62\xB4\xC5\xD9\x9C\xB5"
			"\xAD\xB3\x00\xA0\x28\x5E\x53\x01\x93\x0E\x0C\x70\xFB\x68\x76\x93"
			"\x9C\xE6\x16\xCE\x62\x4A\x11\xE0\x08\x6D\x34\x1E\xBC\xAC\xA0\xA1"
			"\xF5",
			.d =
			"\x0A\x03\x37\x48\x62\x64\x87\x69\x5F\x5F\x30\xBC\x38\xB9\x8B\x44"
			"\xC2\xCD\x2D\xFF\x43\x40\x98\xCD\x20\xD8\xA1\x38\xD0\x90\xBF\x64"
			"\x79\x7C\x3F\xA7\xA2\xCD\xCB\x3C\xD1\xE0\xBD\xBA\x26\x54\xB4\xF9"
			"\xDF\x8E\x8A\xE5\x9D\x73\x3D\x9F\x33\xB3\x01\x62\x4A\xFD\x1D\x51",
			.e = "\x11",
			.m_bits = 520,
			.d_bits = 512,
			.e_bits = 8
		},
		.p = "\x54\x85\x9b\x34\x2c\x49\xea\x2a",
		.c =
		"\x63\x1c\xcd\x7b\xe1\x7e\xe4\xde\xc9\xa8\x89\xa1\x74\xcb\x3c\x63"
		"\x7d\x24\xec\x83\xc3\x15\xe4\x7f\x73\x05\x34\xd1\xec\x22\xbb\x8a"
		"\x5e\x32\x39\x6d\xc1\x1d\x7d\x50\x3b\x9f\x7a\xad\xf0\x2e\x25\x53"
		"\x9f\x6e\xbd\x4c\x55\x84\x0c\x9b\xcf\x1a\x4b\x51\x1e\x9e\x0c\x06",
		.p_size = 8,
		.c_size = 64,
	},
	{
		.k = {
			.m =
			"\xC3\x8B\x55\x7B\x73\x4D\xFF\xE9\x9B\xC6\xDC\x67\x3C\xB4\x8E"
			"\xA0\x86\xED\xF2\xB9\x50\x5C\x54\x5C\xBA\xE4\xA1\xB2\xA7\xAE\x2F"
			"\x1B\x7D\xF1\xFB\xAC\x79\xC5\xDF\x1A\x00\xC9\xB2\xC1\x61\x25\x33"
			"\xE6\x9C\xE9\xCF\xD6\x27\xC4\x4E\x44\x30\x44\x5E\x08\xA1\x87\x52"
			"\xCC\x6B\x97\x70\x8C\xBC\xA5\x06\x31\x0C\xD4\x2F\xD5\x7D\x26\x24"
			"\xA2\xE2\xAC\x78\xF4\x53\x14\xCE\xF7\x19\x2E\xD7\xF7\xE6\x0C\xB9"
			"\x56\x7F\x0B\xF1\xB1\xE2\x43\x70\xBD\x86\x1D\xA1\xCC\x2B\x19\x08"
			"\x76\xEF\x91\xAC\xBF\x20\x24\x0D\x38\xC0\x89\xB8\x9A\x70\xB3\x64"
			"\xD9\x8F\x80\x41\x10\x5B\x9F\xB1\xCB\x76\x43\x00\x21\x25\x36\xD4"
			"\x19\xFC\x55\x95\x10\xE4\x26\x74\x98\x2C\xD9\xBD\x0B\x2B\x04\xC2"
			"\xAC\x82\x38\xB4\xDD\x4C\x04\x7E\x51\x36\x40\x1E\x0B\xC4\x7C\x25"
			"\xDD\x4B\xB2\xE7\x20\x0A\x57\xF9\xB4\x94\xC3\x08\x33\x22\x6F\x8B"
			"\x48\xDB\x03\x68\x5A\x5B\xBA\xAE\xF3\xAD\xCF\xC3\x6D\xBA\xF1\x28"
			"\x67\x7E\x6C\x79\x07\xDE\xFC\xED\xE7\x96\xE3\x6C\xE0\x2C\x87\xF8"
			"\x02\x01\x28\x38\x43\x21\x53\x84\x69\x75\x78\x15\x7E\xEE\xD2\x1B"
			"\xB9\x23\x40\xA8\x86\x1E\x38\x83\xB2\x73\x1D\x53\xFB\x9E\x2A\x8A"
			"\xB2\x75\x35\x01\xC3\xC3\xC4\x94\xE8\x84\x86\x64\x81\xF4\x42\xAA"
			"\x3C\x0E\xD6\x4F\xBC\x0A\x09\x2D\xE7\x1B\xD4\x10\xA8\x54\xEA\x89"
			"\x84\x8A\xCB\xF7\x5A\x3C\xCA\x76\x08\x29\x62\xB4\x6A\x22\xDF\x14"
			"\x95\x71\xFD\xB6\x86\x39\xB8\x8B\xF8\x91\x7F\x38\xAA\x14\xCD\xE5"
			"\xF5\x1D\xC2\x6D\x53\x69\x52\x84\x7F\xA3\x1A\x5E\x26\x04\x83\x06"
			"\x73\x52\x56\xCF\x76\x26\xC9\xDD\x75\xD7\xFC\xF4\x69\xD8\x7B\x55"
			"\xB7\x68\x13\x53\xB9\xE7\x89\xC3\xE8\xD6\x6E\xA7\x6D\xEA\x81\xFD"
			"\xC4\xB7\x05\x5A\xB7\x41\x0A\x23\x8E\x03\x8A\x1C\xAE\xD3\x1E\xCE"
			"\xE3\x5E\xFC\x19\x4A\xEE\x61\x9B\x8E\xE5\xE5\xDD\x85\xF9\x41\xEC"
			"\x14\x53\x92\xF7\xDD\x06\x85\x02\x91\xE3\xEB\x6C\x43\x03\xB1\x36"
			"\x7B\x89\x5A\xA8\xEB\xFC\xD5\xA8\x35\xDC\x81\xD9\x5C\xBD\xCA\xDC"
			"\x9B\x98\x0B\x06\x5D\x0C\x5B\xEE\xF3\xD5\xCC\x57\xC9\x71\x2F\x90"
			"\x3B\x3C\xF0\x8E\x4E\x35\x48\xAE\x63\x74\xA9\xFC\x72\x75\x8E\x34"
			"\xA8\xF2\x1F\xEA\xDF\x3A\x37\x2D\xE5\x39\x39\xF8\x57\x58\x3C\x04"
			"\xFE\x87\x06\x98\xBC\x7B\xD3\x21\x36\x60\x25\x54\xA7\x3D\xFA\x91"
			"\xCC\xA8\x0B\x92\x8E\xB4\xF7\x06\xFF\x1E\x95\xCB\x07\x76\x97\x3B"
			"\x9D",
			.d =
			"\x74\xA9\xE0\x6A\x32\xB4\xCA\x85\xD9\x86\x9F\x60\x88\x7B\x40\xCC"
			"\xCD\x33\x91\xA8\xB6\x25\x1F\xBF\xE3\x51\x1C\x97\xB6\x2A\xD9\xB8"
			"\x11\x40\x19\xE3\x21\x13\xC8\xB3\x7E\xDC\xD7\x65\x40\x4C\x2D\xD6"
			"\xDC\xAF\x32\x6C\x96\x75\x2C\x2C\xCA\x8F\x3F\x7A\xEE\xC4\x09\xC6"
			"\x24\x3A\xC9\xCF\x6D\x8D\x17\x50\x94\x52\xD3\xE7\x0F\x2F\x7E\x94"
			"\x1F\xA0\xBE\xD9\x25\xE8\x38\x42\x7C\x27\xD2\x79\xF8\x2A\x87\x38"
			"\xEF\xBB\x74\x8B\xA8\x6E\x8C\x08\xC6\xC7\x4F\x0C\xBC\x79\xC6\xEF"
			"\x0E\xA7\x5E\xE4\xF8\x8C\x09\xC7\x5E\x37\xCC\x87\x77\xCD\xCF\xD1"
			"\x6D\x28\x1B\xA9\x62\xC0\xB8\x16\xA7\x8B\xF9\xBB\xCC\xB4\x15\x7F"
			"\x1B\x69\x03\xF2\x7B\xEB\xE5\x8C\x14\xD6\x23\x4F\x52\x6F\x18\xA6"
			"\x4B\x5B\x01\xAD\x35\xF9\x48\x53\xB3\x86\x35\x66\xD7\xE7\x29\xC0"
			"\x09\xB5\xC6\xE6\xFA\xC4\xDA\x19\xBE\xD7\x4D\x41\x14\xBE\x6F\xDF"
			"\x1B\xAB\xC0\xCA\x88\x07\xAC\xF1\x7D\x35\x83\x67\x28\x2D\x50\xE9"
			"\xCE\x27\x71\x5E\x1C\xCF\xD2\x30\x65\x79\x72\x2F\x9C\xE1\xD2\x39"
			"\x7F\xEF\x3B\x01\xF2\x14\x1D\xDF\xBD\x51\xD3\xA1\x53\x62\xCF\x5F"
			"\x79\x84\xCE\x06\x96\x69\x29\x49\x82\x1C\x71\x4A\xA1\x66\xC8\x2F"
			"\xFD\x7B\x96\x7B\xFC\xC4\x26\x58\xC4\xFC\x7C\xAF\xB5\xE8\x95\x83"
			"\x87\xCB\x46\xDE\x97\xA7\xB3\xA2\x54\x5B\xD7\xAF\xAB\xEB\xC8\xF3"
			"\x55\x9D\x48\x2B\x30\x9C\xDC\x26\x4B\xC2\x89\x45\x13\xB2\x01\x9A"
			"\xA4\x65\xC3\xEC\x24\x2D\x26\x97\xEB\x80\x8A\x9D\x03\xBC\x59\x66"
			"\x9E\xE2\xBB\xBB\x63\x19\x64\x93\x11\x7B\x25\x65\x30\xCD\x5B\x4B"
			"\x2C\xFF\xDC\x2D\x30\x87\x1F\x3C\x88\x07\xD0\xFC\x48\xCC\x05\x8A"
			"\xA2\xC8\x39\x3E\xD5\x51\xBC\x0A\xBE\x6D\xA8\xA0\xF6\x88\x06\x79"
			"\x13\xFF\x1B\x45\xDA\x54\xC9\x24\x25\x8A\x75\x0A\x26\xD1\x69\x81"
			"\x14\x14\xD1\x79\x7D\x8E\x76\xF2\xE0\xEB\xDD\x0F\xDE\xC2\xEC\x80"
			"\xD7\xDC\x16\x99\x92\xBE\xCB\x40\x0C\xCE\x7C\x3B\x46\xA2\x5B\x5D"
			"\x0C\x45\xEB\xE1\x00\xDE\x72\x50\xB1\xA6\x0B\x76\xC5\x8D\xFC\x82"
			"\x38\x6D\x99\x14\x1D\x1A\x4A\xD3\x7C\x53\xB8\x12\x46\xA2\x30\x38"
			"\x82\xF4\x96\x6E\x8C\xCE\x47\x0D\xAF\x0A\x3B\x45\xB7\x43\x95\x43"
			"\x9E\x02\x2C\x44\x07\x6D\x1F\x3C\x66\x89\x09\xB6\x1F\x06\x30\xCC"
			"\xAD\xCE\x7D\x9A\xDE\x3E\xFB\x6C\xE4\x58\x43\xD2\x4F\xA5\x9E\x5E"
			"\xA7\x7B\xAE\x3A\xF6\x7E\xD9\xDB\xD3\xF5\xC5\x41\xAF\xE6\x9C\x91",
			.e = "\x01\x00\x01",
			.m_bits = 4096,
			.d_bits = 4096,
			.e_bits = 24
		},
		.p = "\x54\x85\x9b\x34\x2c\x49\xea\x2a",
		.c =
		"\x5c\xce\x9c\xd7\x9a\x9e\xa1\xfe\x7a\x82\x3c\x68\x27\x98\xe3\x5d"
		"\xd5\xd7\x07\x29\xf5\xfb\xc3\x1a\x7f\x63\x1e\x62\x31\x3b\x19\x87"
		"\x79\x4f\xec\x7b\xf3\xcb\xea\x9b\x95\x52\x3a\x40\xe5\x87\x7b\x72"
		"\xd1\x72\xc9\xfb\x54\x63\xd8\xc9\xd7\x2c\xfc\x7b\xc3\x14\x1e\xbc"
		"\x18\xb4\x34\xa1\xbf\x14\xb1\x37\x31\x6e\xf0\x1b\x35\x19\x54\x07"
		"\xf7\x99\xec\x3e\x63\xe2\xcd\x61\x28\x65\xc3\xcd\xb1\x38\x36\xa5"
		"\xb2\xd7\xb0\xdc\x1f\xf5\xef\x19\xc7\x53\x32\x2d\x1c\x26\xda\xe4"
		"\x0d\xd6\x90\x7e\x28\xd8\xdc\xe4\x61\x05\xd2\x25\x90\x01\xd3\x96"
		"\x6d\xa6\xcf\x58\x20\xbb\x03\xf4\x01\xbc\x79\xb9\x18\xd8\xb8\xba"
		"\xbd\x93\xfc\xf2\x62\x5d\x8c\x66\x1e\x0e\x84\x59\x93\xdd\xe2\x93"
		"\xa2\x62\x7d\x08\x82\x7a\xdd\xfc\xb8\xbc\xc5\x4f\x9c\x4e\xbf\xb4"
		"\xfc\xf4\xc5\x01\xe8\x00\x70\x4d\x28\x26\xcc\x2e\xfe\x0e\x58\x41"
		"\x8b\xec\xaf\x7c\x4b\x54\xd0\xa0\x64\xf9\x32\xf4\x2e\x47\x65\x0a"
		"\x67\x88\x39\x3a\xdb\xb2\xdb\x7b\xb5\xf6\x17\xa8\xd9\xc6\x5e\x28"
		"\x13\x82\x8a\x99\xdb\x60\x08\xa5\x23\x37\xfa\x88\x90\x31\xc8\x9d"
		"\x8f\xec\xfb\x85\x9f\xb1\xce\xa6\x24\x50\x46\x44\x47\xcb\x65\xd1"
		"\xdf\xc0\xb1\x6c\x90\x1f\x99\x8e\x4d\xd5\x9e\x31\x07\x66\x87\xdf"
		"\x01\xaa\x56\x3c\x71\xe0\x2b\x6f\x67\x3b\x23\xed\xc2\xbd\x03\x30"
		"\x79\x76\x02\x10\x10\x98\x85\x8a\xff\xfd\x0b\xda\xa5\xd9\x32\x48"
		"\x02\xa0\x0b\xb9\x2a\x8a\x18\xca\xc6\x8f\x3f\xbb\x16\xb2\xaa\x98"
		"\x27\xe3\x60\x43\xed\x15\x70\xd4\x57\x15\xfe\x19\xd4\x9b\x13\x78"
		"\x8a\xf7\x21\xf1\xa2\xa2\x2d\xb3\x09\xcf\x44\x91\x6e\x08\x3a\x30"
		"\x81\x3e\x90\x93\x8a\x67\x33\x00\x59\x54\x9a\x25\xd3\x49\x8e\x9f"
		"\xc1\x4b\xe5\x86\xf3\x50\x4c\xbc\xc5\xd3\xf5\x3a\x54\xe1\x36\x3f"
		"\xe2\x5a\xb4\x37\xc0\xeb\x70\x35\xec\xf6\xb7\xe8\x44\x3b\x7b\xf3"
		"\xf1\xf2\x1e\xdb\x60\x7d\xd5\xbe\xf0\x71\x34\x90\x4c\xcb\xd4\x35"
		"\x51\xc7\xdd\xd8\xc9\x81\xf5\x5d\x57\x46\x2c\xb1\x7b\x9b\xaa\xcb"
		"\xd1\x22\x25\x49\x44\xa3\xd4\x6b\x29\x7b\xd8\xb2\x07\x93\xbf\x3d"
		"\x52\x49\x84\x79\xef\xb8\xe5\xc4\xad\xca\xa8\xc6\xf6\xa6\x76\x70"
		"\x5b\x0b\xe5\x83\xc6\x0e\xef\x55\xf2\xe7\xff\x04\xea\xe6\x13\xbe"
		"\x40\xe1\x40\x45\x48\x66\x75\x31\xae\x35\x64\x91\x11\x6f\xda\xee"
		"\x26\x86\x45\x6f\x0b\xd5\x9f\x03\xb1\x65\x5b\xdb\xa4\xe4\xf9\x45",
		.p_size = 8,
		.c_size = 512,
	},
};

static void rsa_test(const struct shell *shell, size_t argc, char **argv)
{
	const struct device *dev = device_get_binding(RSA_DRV_NAME);
	struct rsa_ctx ini;
	struct rsa_pkt pkt;
	struct rsa_key *rk;
	char plain_text[512];
	char cipher_text[512];
	int i, j;
	int ret;

	for (i = 0; i < ARRAY_SIZE(rsa_tv); i++) {
		shell_fprintf(shell, SHELL_NORMAL, "rsa test vector[%d]:\n", i);
		rk = &rsa_tv[i].k;
		pkt.in_buf = rsa_tv[i].p;
		pkt.in_len = rsa_tv[i].p_size;
		pkt.out_buf = cipher_text;
		pkt.out_buf_max = 512;
		ret = rsa_begin_session(dev, &ini, rk);
		if (ret)
			shell_print(shell, "rsa_begin_session fail: %d", ret);
		rsa_encrypt(&ini, &pkt);
		rsa_free_session(dev, &ini);
		ret = memcmp(cipher_text, rsa_tv[i].c, rsa_tv[i].c_size);
		if (ret != 0) {
			shell_fprintf(shell, SHELL_NORMAL, "enc fail: %d\n", ret);
			shell_fprintf(shell, SHELL_NORMAL, "input text:\n");
			for (j = 0; j < rsa_tv[i].p_size; j++) {
				shell_fprintf(shell, SHELL_NORMAL, "%02x", rsa_tv[i].p[j]);
				if ((j + 1) % 32 == 0)
					shell_fprintf(shell, SHELL_NORMAL, "\n");
			}
			shell_fprintf(shell, SHELL_NORMAL, "\n");
			shell_fprintf(shell, SHELL_NORMAL, "result text:\n");
			for (j = 0; j < rsa_tv[i].c_size; j++) {
				shell_fprintf(shell, SHELL_NORMAL, "%02x", cipher_text[j]);
				if ((j + 1) % 32 == 0)
					shell_fprintf(shell, SHELL_NORMAL, "\n");
			}
			shell_fprintf(shell, SHELL_NORMAL, "\n");
		} else {
			shell_fprintf(shell, SHELL_NORMAL, "enc pass\n");
		}

		pkt.in_buf = cipher_text;
		pkt.in_len = rsa_tv[i].c_size;
		pkt.out_buf = plain_text;
		pkt.out_buf_max = 512;
		ret = rsa_begin_session(dev, &ini, rk);
		if (ret)
			shell_print(shell, "rsa_begin_session fail: %d", ret);
		rsa_decrypt(&ini, &pkt);
		rsa_free_session(dev, &ini);
		ret = memcmp(plain_text, rsa_tv[i].p, rsa_tv[i].p_size);
		if (ret != 0) {
			shell_fprintf(shell, SHELL_NORMAL, "dec fail: %d\n", ret);
			shell_fprintf(shell, SHELL_NORMAL, "input text:\n");
			for (j = 0; j < rsa_tv[i].c_size; j++) {
				shell_fprintf(shell, SHELL_NORMAL, "%02x", cipher_text[j]);
				if ((j + 1) % 32 == 0)
					shell_fprintf(shell, SHELL_NORMAL, "\n");
			}
			shell_fprintf(shell, SHELL_NORMAL, "result text:\n");
			for (j = 0; j < rsa_tv[i].p_size; j++) {
				shell_fprintf(shell, SHELL_NORMAL, "%02x", plain_text[j]);
				if ((j + 1) % 32 == 0)
					shell_fprintf(shell, SHELL_NORMAL, "\n");
			}
			shell_fprintf(shell, SHELL_NORMAL, "\n");
		} else {
			shell_fprintf(shell, SHELL_NORMAL, "dec pass\n");
		}
	}
}

SHELL_STATIC_SUBCMD_SET_CREATE(rsa_cmds,
	SHELL_CMD_ARG(test, NULL, "", rsa_test, 1, 0),
	SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(rsa, &rsa_cmds, "RSA shell commands", NULL);
