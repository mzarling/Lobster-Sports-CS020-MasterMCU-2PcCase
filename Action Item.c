for (i=0; i<=RemoteDataCnt; i++) {
	while(Busy2USART());
	putc2USART(RemoteString[i]);
}