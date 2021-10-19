
BEGIN{
}

{
	if ($5 =="cbr" ) {
		print $2 " " $11
	}
}

END{
}
