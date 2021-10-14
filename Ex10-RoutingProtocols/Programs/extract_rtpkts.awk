
BEGIN{
recvd = 0;
rt_pkts = 0;
rt_size = 0;
}

{
	if ($5 =="rtProtoDV" || $5 =="rtProtoLS" ) {
		rt_pkts++;
		rt_size = rt_size + $6;
		print $2 " " $6 " " $5
	}
}

END{
}
