void memset(void *dest, char val, unsigned int count)
{
    unsigned char *temp = (unsigned char *)dest;
	while(count != 0)
	{
		temp[count] = val;
		count--;
	}
}