This project is writen for exercise. It's purpose is to over the great fire wall using socks5 protocol.

How to use it?
1. install and compile socks5 server
	After the project has been download, get into src directory, using "make".  Notice that two another
	empty directory are required which is "bin" and "tmp", just the same subdirectory as "src".

2. run the server
	To run it, using command like "../bin/overwall -P 1080 -T 30 &", the -P means which port you are listening
	and -T is how much thread you want use.

3. cofigure the ngnix to provide pac file
	The pac file is under the serverconfig directory, just find how to run a nginx on the internet. The 
	Proxy in pac file should using the same port as the socks5 server listened.

4. web browser configuration
	Configuration => using the automatic proxy => "http://your server ip/pacfiles/proxy.pac" and choose
	using socks5 for dns if there is.

