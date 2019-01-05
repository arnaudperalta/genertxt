tar:
	$(MAKE) -C genertxt clean
	tar -zcf "$(CURDIR).tar.gz" genertxt/* holdall/* hashtable/* keylist/* valuelist/* makefile
