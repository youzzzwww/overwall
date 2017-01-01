function FindProxyForURL(url, host) {
	var	DIRECT = "DIRECT";
	var	PROXY = "SOCKS5 yourip:1080";

	var	rules = [
		"google.com",
		"facebook.com",
		"twitter.com",
		"youtube.com",
		"googlevideo.com",
		"gstatic.com",
		"googleapis.com",
		"lang-8.com",
		"twitch.tv",
		"ttvnw.net"
	];


	for (var i=0; i<rules.length; ++i) {
		if (dnsDomainIs(host, rules[i])) {
			return PROXY;
		}
	};
	return DIRECT;
}
