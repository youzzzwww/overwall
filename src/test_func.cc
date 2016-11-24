#include <string>
#include <iostream>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <functional>
#include <unistd.h>

#include "socks5.h"

bool test_socks5_resolve(void) {
	char init_frame[] = {0x05, 0x01, 0x00};
	if (socks5_init_size(init_frame, sizeof(init_frame))) {
		return true;
	}
	return false;
}

bool test_init_reply(void) {
	char reply[] = {0x05, 0x00};
	if (sizeof(reply) == 2)
		return true;
	return false;
}

bool test_host_resolve(void) {
	int soc = get_sock_with_name("www.baidu.com", "443");
	if (soc > 0) {
		close(soc);
		return true;
	}
	return false;
}

bool test_socks5_host(void) {
	char host_name[] = "www.baidu.com";
	char header[] = {0x05, 0x01, 0x00, 0x03};

	char frame[100];
	int offset = 0;
	memcpy(frame, header, sizeof(header));
	offset += sizeof(header);
	uint8_t host_len = (uint8_t)strlen(host_name);
	memcpy(frame+offset, &host_len, 1);
	offset += 1;
	memcpy(frame+offset, host_name, host_len);
	offset += host_len;
	uint16_t port = htons(443);
	memcpy(frame+offset, &port, 2);
	offset += 2;

	int soc = socks5_remote_sock(frame, offset);
	if (soc > 0) {
		close(soc);
		return true;
	}
	return false;
}

bool test_socks5_addr(void) {
	char host_addr[] = "14.215.177.37";
	char header[] = {0x05, 0x01, 0x00, 0x01};

	char frame[100];
	int offset = 0;
	memcpy(frame, header, sizeof(header));
	offset += sizeof(header);

	char ip[4];
	inet_pton(AF_INET, host_addr, ip);
	memcpy(frame+offset, ip, sizeof(ip));
	offset += sizeof(ip);
	uint16_t port = htons(443);
	memcpy(frame+offset, &port, 2);
	offset += 2;

	int soc = socks5_remote_sock(frame, offset);
	if (soc > 0) {
		close(soc);
		return true;
	}
	return false;
}

struct TestStruct {
	std::function<bool(void)> func;
	std::string description;
};

int main(int argc, char** argv) {
	TestStruct test_cases[] = {
		{test_socks5_resolve, "init frame resolve"},
		{test_init_reply, "init reply"},
		{test_host_resolve, "host resolve"},
		{test_socks5_host, "socks5 host resolve"},
		{test_socks5_addr, "socks5 addr resolve"}
	};

	for (unsigned int i=0; i < sizeof(test_cases)/sizeof(TestStruct); ++i) {
		if (test_cases[i].func()) {
			std::cout << test_cases[i].description << " passed" << std::endl;
		} else {
			std::cout << test_cases[i].description << " error" << std::endl;
		}
	}
	return 0;
}
