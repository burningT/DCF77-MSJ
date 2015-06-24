//	x_n_envelope = IIR_DF1(num_envelope, &T_IIR, x_n_DC_and_FIR_MOD_filtered);
float IIR_DF1(float b1, float *T_IIR, float x_n)
{
/*
float y_n	= num_envelope[0]*x_n			// a0
				+ num_envelope[1]*T_IIR[0]		// a1
				- num_envelope[2] * T_IIR[1];		// -b1
				//- num_envelope[3]*T_IIR[1];		// -b1
	T_IIR[0] = x_n;
	T_IIR[1] = y_n;
	return y_n;
*/
	//float y_n = x_n + b1 * *T_IIR;
	float y_n = x_n * b1 + *T_IIR;
	*T_IIR = y_n;
	return *T_IIR;
}
