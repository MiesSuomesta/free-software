<?php

include_once("checkEmail.php");

function lja_mailer_raw_check_email($targetaddress, $verbose = 0)
{
	return lja_email_checker_raw_check_email($targetaddress, $verbose); 
}

function lja_mailer_check_email($targetaddress, $ver)
{
	try {
		return lja_mailer_raw_check_email($targetaddress, $ver);
	} catch (Exception $e)
	{
		print($e);
		return false;
	}
}


function lja_is_email_valid($email)
{
        $rv = lja_mailer_check_email($email, true);
	error_log("LJA email check: " . $email .": $rv");
        return $rv;
}

lja_is_email_valid("aergaerg@argaerg.gi");
lja_is_email_valid("aergaerg@lja.fi");
?>
