function FindProxyForURL(url, host) {
	var	DIRECT = "DIRECT";
	var	PROXY = "SOCKS5 192.168.40.129:1080";

	var	rules = [
		"google.com",
		"facebook.com",
		"twitter.com",
		"baidu.com"
	];


	for (var i=0; i<rules.length; ++i) {
		if (dnsDomainIs(host, rules[i])) {
			return PROXY;
		}
	};
	return DIRECT;
}
