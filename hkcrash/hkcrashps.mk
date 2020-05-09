
hkcrashps.dll: dlldata.obj hkcrash_p.obj hkcrash_i.obj
	link /dll /out:hkcrashps.dll /def:hkcrashps.def /entry:DllMain dlldata.obj hkcrash_p.obj hkcrash_i.obj \
		kernel32.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
.c.obj:
	cl /c /Ox /DREGISTER_PROXY_DLL \
		$<

clean:
	@del hkcrashps.dll
	@del hkcrashps.lib
	@del hkcrashps.exp
	@del dlldata.obj
	@del hkcrash_p.obj
	@del hkcrash_i.obj
