# wireshark 过滤出子机没有dns响应的query

        dns && (dns.flags.response == 0) && ! dns.response_in
