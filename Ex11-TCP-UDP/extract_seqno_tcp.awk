
BEGIN{
}

{
	if ($5=="tcp" && $1=="+") {
		print $2 " " $11
	}
}

END{
}
