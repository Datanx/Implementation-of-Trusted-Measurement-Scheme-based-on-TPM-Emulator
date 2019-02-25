int MyFunc_SealFile(char *file,TSS_HKEY hKey,UINT32 *pcrs,UINT32 num_pcrs)
{
	FILE *fin,*fout;
	UINT32 inSize,outSize,encDesKeySize = DES_KEY_SZ;
	BYTE in[DES_KEY_SZ],out[DES_KEY_SZ],encDesKey[DES_KEY_SZ];
	TSS_HPCRS hPcrs;
	BYTE *outFileName;
	DES_key_schedule schedule;
	
	DES_random_key(&key);
	DES_set_key_checked(&key,&schedule);
	
	inSize = outSize = DES_KEY_SZ;
	outFileName = malloc(strlen(file)+5);
	sprintf(outFileName,"%s.enc",file);
	
	fin = fopen(file,"r");
	fout = fopen(outFileName,"w");
	/*根据所选择使用的对称加密算法，需要依据该算法已知的弱密钥数据库检查
	*返回的随机数是否是弱密钥。对于使用弱密钥加密的数据，攻击者通过分析密
	*文可以找出其秘钥*/
	while((inSize = read(fileno(fin),in,inSize)== DES_KEY_SZ)){
		/*调用外部库，使用对称秘钥进行大量数据的加密*/
		DES_ecb_encrypt(&in,&out,&schedule,DES_ENCRYPT);
		/*将加密的数据块写回*/
		write(fileno(fout),out,outSize);
	}
	fclose(fin);
	fclose(fout);
	/*创建PCR合成对象，密封对称秘钥*/
	Myfunc_CreatePcrs(num_pcrs,pcrs,&hPcrs);
	/*使用TPM秘钥密封对称秘钥*/
	MyFunc_SealData(hKey,hPcrs,SYM_KEY_SIZE,symKey,&encSymKeySize,&encSymKey);
	/*将加密的对称秘钥数据块写入文件*/
	sprintf(outFileName,"%s.key",file);
	fout = fopen(outFileName,"w");
	write(fileno(fout),encSymKey,encSymKeySize);
	fclose(fout);
	
	return 0;
}