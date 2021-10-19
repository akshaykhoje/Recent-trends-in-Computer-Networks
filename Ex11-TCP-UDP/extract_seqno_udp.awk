
BEGIN{
}

{
	if ($5=="cbr" && $1=="+") {
		print $2 " " $11
	}
}

END{
}
