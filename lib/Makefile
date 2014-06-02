include ../Makefile.in

all: libqcd.a

CFLAGS += -I../include
SOURCES=\
	qcd_blas		\
	qcd_communication	\
	qcd_contract_2pt	\
	qcd_contract_2pt_pr	\
	qcd_contract_3pt_all	\
	qcd_currents_udsc	\
	qcd_gamma		\
	qcd_gaugeFixing		\
	qcd_init		\
	qcd_io			\
	qcd_observables		\
	qcd_particle_data	\
	qcd_rng			\
	qcd_smearing		\
	qcd_smearing_opt	\
	qcd_stochastic		\
	qcd_wilson

include $(wildcard *.d)

libqcd.a: ${addsuffix .o, $(SOURCES)}
	$(AR) cr $@  ${addsuffix .o, $(SOURCES)} &&\
	$(RANLIB) $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) -MM $(CFLAGS) -c $< > $*.d

clean:
	$(RM) *.o *.d

cleanall: clean
	$(RM) libqcd.a
