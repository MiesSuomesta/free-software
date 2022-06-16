# free-software
C++: 
	template_helpers.h:

		Template classes: LessThan, MoreThan & WithinRange

			USAGE: LessThan<double> less_than_50 {50}; MoreThan<double> more_than_50 {50}; 
				WithinRange<double> is_valid_percent {-100 , 100};

				bool pa = is_valid_percent(50); bool pb = is_valid_percent(0.35); bool pc 
				= is_valid_percent(101); bool la = less_than_50(50); bool lb = 
				less_than_50(0.35); bool lc = less_than_50(101); bool ma = 
				more_than_50(50); bool mb = more_than_50(0.35); bool mc = 
				more_than_50(101);


Python3:

	email/
		checkEmail.py

			USAGE: python3 checkEmail.py <email> [<email2> <email3> ..... <emailN>]

				Output:

					One argument:

						exit value 0 if email address is ok.

						exit value 1 if email address is not ok.

					two or more arguments:

						list of OK or NOK


Happy coding....
