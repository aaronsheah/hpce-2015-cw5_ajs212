__kernel void kernelBrute(
    __global const uint *input_key, 
    const uint input_rounds, 
    const ulong input_cipherText,
    __global int *hit
)
{

	uint i = get_global_id(0);

	ulong tmp=TEA_hash(i, input_key, input_rounds);

	//uint i0=i&0xFFFFFFFFul, i1=i>>32, sum=0, j;           /* set up */
	//uint delta=0x9e3779b9;                     /* a key schedule constant */
	//uint input_key0=input_key[0], input_key1=input_key[1], input_key2=input_key[2], input_key3=input_key[3];   /* cache key */

	//ulong res=1234567801234567ul;
	//for (j=0; j < input_rounds; j++) {                       /* basic cycle start */
	//	sum += delta;
	//	i0 += ((i1<<4) + input_key0) ^ (i1 + sum) ^ ((i1>>5) + input_key1);
	//	i1 += ((i0<<4) + input_key2) ^ (i0 + sum) ^ ((i0>>5) + input_key3);
	//	res=((res << 7) ^ (res>>57)) + (i0&i1); // Non-linear, oh-noes! 
	//}     /* end cycle */

	hit[i] = tmp==input_cipherText;
}
