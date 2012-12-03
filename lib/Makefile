include ../Makefile.in

all: libqcd.a

CFLAGS += -I../include
SOURCES=\
	qcd_rng\
	qcd_blas\
	qcd_communication\
	qcd_gamma\
	qcd_gaugeFixing\
	qcd_init\
	qcd_io\
	qcd_observables\
	qcd_smearing\
	qcd_wilson\
	qcd_stochastic

include $(wildcard *.d)

libqcd.a: ${addsuffix .o, $(SOURCES)}
	$(AR) cr $@  ${addsuffix .o, $(SOURCES)} &&\
	$(RANLIB) $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) -MM $(CFLAGS) $< > $*.d

clean:
	$(RM) *.o *.d

cleanall: clean
	$(RM) libqcd.a