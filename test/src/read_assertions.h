#ifndef TEST_SRC_READ_ASSERTIONS_H_
#define TEST_SRC_READ_ASSERTIONS_H_

#include <any>
#include <map>
#include <string>

#include <vrt/vrt_types.h>

void assert_header(const vrt_header& h, const std::map<std::string, std::any>& values);
void assert_fields(const vrt_fields& f, const std::map<std::string, std::any>& values);
void assert_trailer(const vrt_trailer& t, const std::map<std::string, std::any>& values);
void assert_if_context(const vrt_if_context& c, const std::map<std::string, std::any>& values);
void assert_packet(const vrt_packet& packet, const std::map<std::string, std::any>& values);

#endif
