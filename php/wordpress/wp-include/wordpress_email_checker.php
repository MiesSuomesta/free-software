<?php

namespace PHPMailer\PHPMailer;

require_once __DIR__ . '/PHPMailer/SMTP.php';

include_once("checkEmail.php");

function lja_getip($target)
{
	return lja_email_checker_getip($target);
}

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

?>
