/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#include <cstdint>
#include <iostream>
#include <fstream>
#include <plusone/compiler.hpp>

struct pcap_hdr_t
{
    std::uint32_t magic_number;   /* magic number */
    std::uint16_t version_major;  /* major version number */
    std::uint16_t version_minor;  /* minor version number */
    std::int32_t  thiszone;       /* GMT to local correction */
    std::uint32_t sigfigs;        /* accuracy of timestamps */
    std::uint32_t snaplen;        /* max length of captured packets, in octets */
    std::uint32_t network;        /* data link type */
} __packed;

struct pcaprec_hdr_t
{
    std::uint32_t ts_sec;         /* timestamp seconds */
    std::uint32_t ts_usec;        /* timestamp microseconds */
    std::uint32_t incl_len;       /* number of octets of packet saved in file */
    std::uint32_t orig_len;       /* actual length of packet */
} __packed;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "pcap_read <path to file>\n";
        return 1;
    }

    const char* pcap_path = argv[1];

    try {
        std::ifstream is;
        is.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        is.open(pcap_path, std::ifstream::binary);

        pcap_hdr_t header;
        is.read(reinterpret_cast< char* >(&header), sizeof(header));

        std::cout
            << "magic_number: 0x" << std::hex << header.magic_number << std::dec << '\n';
        std::cout
            << "version_major: " << header.version_major << '\n'
            << "version_minor: " << header.version_minor << '\n'
            << "thiszone: " << header.thiszone << '\n'
            << "network: " << header.network << '\n';

        char buffer[4096];
        pcaprec_hdr_t record;

        while (__likely(!is.eof())) {
            is.read(reinterpret_cast< char* >(&record), sizeof(record));

            std::cout
                << "--\n"
                << "timestamp: " << record.ts_sec << '.' << record.ts_usec << '\n'
                << "incl_len: " << record.incl_len << '\n'
                << "orig_len: " << record.orig_len << '\n';

            /* actual packet data */
            is.read(buffer, record.incl_len);

            ether_header* header = reinterpret_cast< ether_header* >(buffer);
            if (__likely(header->ether_type == ntohs(ETHERTYPE_IP))) {
                iphdr* ip = reinterpret_cast< iphdr* >(header + 1);
                const unsigned char* saddr = reinterpret_cast< const unsigned char* >(&ip->saddr);
                const unsigned char* daddr = reinterpret_cast< const unsigned char* >(&ip->daddr);
                std::cout << "[packet] "
                    << int(saddr[0]) << '.' << int(saddr[1]) << '.' << int(saddr[2]) << '.' << int(saddr[3]) << " -> "
                    << int(daddr[0]) << '.' << int(daddr[1]) << '.' << int(daddr[2]) << '.' << int(daddr[3]) << '\n';
            }
            //std::cout << "ether_type: " << header->ether_type << '\n';
            std::cout << "processed\n";
        }


    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
