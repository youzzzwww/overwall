#! /bin/bash

run_command="/root/project/overwall/bin/overwall -P 1080 -T 100 &"

case "$(pidof overwall | wc -w)" in

0)	$run_command
	;;
1)	;;
*)	kill $(pidof overwall | awk '{print $1}')
	;;
esac
