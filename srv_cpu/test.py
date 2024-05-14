import re

pattern = r"http://www.baidu.com/123"
string = "hhhhttp://www.baidu.com/abd"

url_re0 = r"^https?://(www\.)?[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&//=]*)$"

test_re = r"hhh"
protocol_re = r"hhh(?P<protocol>https?://)"
domain_re = r"(?P<domain>(www\.)?[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6})"
tld_re = r"(?P<tld>\b([-a-zA-Z0-9()@:%_\+.~#?&//=]*)$)"

#url_re = test_re + protocol_re + domain_re + tld_re
url_re = protocol_re + domain_re + tld_re

match = re.search(pattern, string)
if match:
    print("000 Match found:", match.group())
else:
    print("No match found")


match = re.search(url_re, string)
if match:
    protocol = match.group(1)
    domain = match.group(2)
    tlb = match.group(3)
    print(protocol, domain, tlb)
    print("111 Match found:", match.group())
else:
    print("No match found")
