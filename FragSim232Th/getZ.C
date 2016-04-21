

Double_t
getZ(const Double_t x, const Double_t y)
{
//(0.569282,-0.642788,0.512584)
	Double_t a = 0.569282;
	Double_t b = -0.642788;
	Double_t c = 0.512584;

	return -(a*x + b*y)/c;
}
