SUBDIRS= usermanual lpc-userguide

%: force
	+@for d in $(SUBDIRS); do \
		cd $$d; make $(*); cd ..; \
	done
force: ;
