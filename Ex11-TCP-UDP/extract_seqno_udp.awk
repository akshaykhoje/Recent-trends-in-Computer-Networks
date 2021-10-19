
BEGIN{
}

{
	if ($5 =="tcp" ) {
		print $2 " " $11
	}
}

END{
}
