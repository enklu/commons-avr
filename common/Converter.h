#ifndef CONVERTER_h
#define CONVERTER_h

union ShortUnion
{
	short shortValue;
	char charValue[2];
};

union IntUnion
{
	int intValue;
	char charValue[4];
};

union FloatUnion
{
	float floatValue;
	char charValue[4];
};

#endif