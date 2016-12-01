This project is writen for exercise. It's purpose is to over the great fire wall using socks5 protocol.

How to use it?
1. install and compile socks5 server.

	After the project has been download, get into src directory, using "make".

2. run the server.

	To run it, using command like "./bin/overwall -P 1080 -T 30 &", the -P means which port you are listening
	and -T is how much thread you want use.

3. cofigure the ngnix to provide pac file.

	The pac file is under the serverconfig directory, just find how to run a nginx through the internet. The 
	Proxy in pac file should using the same ip and port as the socks5 server listened.

4. web browser configuration.

	Configuration => using the automatic proxy => "http://your server ip/pacfiles/proxy.pac" and choose
	using socks5 for dns resolving if there is.

5. improve the connection quality by adjust tcp congestion module.

	The default tcp congestion module is cubic which is not suitable for long run and high packet loss link.
	change it to hybla, you can find the configuration on internet.

