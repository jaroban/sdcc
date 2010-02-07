/* lkmain.c

   Copyright (C) 1989-1998 Alan R. Baldwin
   721 Berkeley St., Kent, Ohio 44240

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

/*
 * 31-Oct-97 JLH:
 *			 - add jflag and jfp to control NoICE output file generation
 *	3-Nov-97 JLH:
 *			 - use a_type == 0 as "virgin area" flag: set == 1 if -b
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sdld.h"
#include "aslink.h"

/*)Module	lkmain.c
 *
 *	The module lkmain.c contains the functions which
 *	(1) input the linker options, parameters, and specifications
 *	(2) perform a two pass link
 *	(3) produce the appropriate linked data output and/or
 *	    link map file and/or relocated listing files.
 *
 *	lkmain.c contains the following functions:
 *		FILE *	afile()
 *		VOID	bassav()
 *		VOID	gblsav()
 *		VOID	link_main()
 *		VOID	lkexit()
 *		VOID	main()
 *		VOID	map()
 *		int	parse()
 *		VOID	setbas()
 *		VOID	setgbl()
 *		VOID	usage()
 *
 *	lkmain.c contains the following local variables:
 *		char *	usetext[]	array of pointers to the
 *					command option tect lines
 *
 */

/* sdld 8051 & 6808 specific */
/*JCF:	Creates some of the default areas so they are allocated in the right order.*/
void Areas51 (void)
{
	char * rel[] = {
		"XH",
		"H 7 areas 0 global symbols",
		"A _CODE size 0 flags 0",		/*Each .rel has one, so...*/
		"A REG_BANK_0 size 0 flags 4",	/*Register banks are overlayable*/
		"A REG_BANK_1 size 0 flags 4",
		"A REG_BANK_2 size 0 flags 4",
		"A REG_BANK_3 size 0 flags 4",
		"A BSEG size 0 flags 80",		/*BSEG must be just before BITS*/
		"A BSEG_BYTES size 0 flags 0",	/*Size will be obtained from BSEG in lnkarea()*/
		""
	};

	char * rel2[] = {
		"XH",
		"H C areas 0 global symbols",
		"A _CODE size 0 flags 0",		/*Each .rel has one, so...*/
		"A REG_BANK_0 size 0 flags 4",	/*Register banks are overlayable*/
		"A REG_BANK_1 size 0 flags 4",
		"A REG_BANK_2 size 0 flags 4",
		"A REG_BANK_3 size 0 flags 4",
		"A BSEG size 0 flags 80",		/*BSEG must be just before BITS*/
		"A BSEG_BYTES size 0 flags 0",	/*Size will be obtained from BSEG in lnkarea()*/
		"A BIT_BANK size 0 flags 4",	/*Bit register bank is overlayable*/
		"A DSEG size 0 flags 0",
		"A OSEG size 0 flags 4",
		"A ISEG size 0 flags 0",
		"A SSEG size 0 flags 4",
		""
	};
	int j;
	struct sym * sp;

	if (packflag) {
		for (j = 0; rel2[j][0] != 0; j++) {
			ip = rel2[j];
			link_main();
		}
	}
	else {
		for (j = 0; rel[j][0] != 0; j++) {
			ip = rel[j];
			link_main();
		}
	}

	/*Set the start address of the default areas:*/
	for (ap = areap; ap; ap = ap->a_ap) {
		/**/ if (!strcmp(ap->a_id, "REG_BANK_0")) { ap->a_addr = 0x00; ap->a_type = 1; }
		else if (!strcmp(ap->a_id, "REG_BANK_1")) { ap->a_addr = 0x08; ap->a_type = 1; }
		else if (!strcmp(ap->a_id, "REG_BANK_2")) { ap->a_addr = 0x10; ap->a_type = 1; }
		else if (!strcmp(ap->a_id, "REG_BANK_3")) { ap->a_addr = 0x18; ap->a_type = 1; }
		else if (!strcmp(ap->a_id, "BSEG_BYTES")) { ap->a_addr = 0x20; ap->a_type = 1; }
		else if (TARGET_IS_8051 && !strcmp(ap->a_id, "SSEG")) {
			if (stacksize) ap->a_axp->a_size = stacksize;
		}
	}

	if (TARGET_IS_8051) {
		sp = lkpsym("l_IRAM", 1);
		sp->s_addr = ((iram_size>0) && (iram_size<=0x100)) ? iram_size : 0x0100;
		sp->s_axp = NULL;
		sp->s_type |= S_DEF;
	}
}
/* end sdld 8051 & 6808 specific */

/* sdld gb specific */
char *default_basep[] = {
  "_CODE=0x0200",
  "_DATA=0xC0A0",
  NULL
};

char *default_globlp[] = {
  /* DMA transfer must start at multiples of 0x100 */
  ".OAM=0xC000",
  ".STACK=0xE000",
  ".refresh_OAM=0xFF80",

  ".init=0x0000",

  NULL
};

int nb_rom_banks;
int nb_ram_banks;
int mbc_type;
char cart_name[16] = "";

patch* patches = NULL;

void
gb_init()
{
	int i;

	nb_rom_banks = 2;
	nb_ram_banks = 0;
	mbc_type = 0;
	symflag = 0;

	for(i = 0; default_basep[i] != NULL; i++) {
		if(basep == NULL) {
			basep = (struct base *)new(sizeof(struct base));
			bsp = basep;
		} else {
			bsp->b_base = (struct base *)new(sizeof(struct base));
			bsp = bsp->b_base;
		}
		bsp->b_strp = default_basep[i];
	}
	for(i = 0; default_globlp[i] != NULL; i++) {
		if(globlp == NULL) {
			globlp = (struct globl *)new(sizeof(struct globl));
			gsp = globlp;
		} else {
			gsp->g_globl = (struct globl *)new(sizeof(struct globl));
			gsp = gsp->g_globl;
		}
		gsp->g_strp = default_globlp[i];
	}
}

void gb_init_banks()
{
	int i;

	for(i = 1; i < nb_rom_banks; i++) {
		bsp->b_base = (struct base *)new(sizeof(struct base));
		bsp = bsp->b_base;
		bsp->b_strp = (char *)malloc(18);
		sprintf(bsp->b_strp, "_CODE_%d=0x4000", i);
	}
	for(i = 0; i < nb_ram_banks; i++) {
		bsp->b_base = (struct base *)new(sizeof(struct base));
		bsp = bsp->b_base;
		bsp->b_strp = (char *)malloc(18);
		sprintf(bsp->b_strp, "_DATA_%d=0xA000", i);
	}
}
/* end sdld gb specific */

/*)Function	VOID	main(argc,argv)
 *
 *		int	argc		number of command line arguments + 1
 *		char *	argv[]		array of pointers to the command line
 *					arguments
 *
 *	The function main() evaluates the command line arguments to
 *	determine if the linker parameters are to input through 'stdin'
 *	or read from a command file.  The functions lk_getline() and parse()
 *	are to input and evaluate the linker parameters.  The linking process
 *	proceeds by making the first pass through each .rel file in the order
 *	presented to the linker.  At the end of the first pass the setbase(),
 *	lnkarea(), setgbl(), and symdef() functions are called to evaluate
 *	the base address terms, link all areas, define global variables,
 *	and look for undefined symbols.  Following these routines a linker
 *	map file may be produced and the linker output files may be opened.
 *	The second pass through the .rel files will output the linked data
 *	in one of the four supported formats.
 *
 *	local variables:
 *		char *	p		pointer to an argument string
 *		int	c		character from argument string
 *		int	i		loop counter
 *
 *	global variables:
 *					text line in ib[]
 *		lfile	*cfp		The pointer *cfp points to the
 *					current lfile structure
 *		char	ctype[]		array of character types, one per
 *					ASCII character
 *		lfile	*filep		The pointer *filep points to the
 *					beginning of a linked list of
 *					lfile structures.
 *		head	*hp		Pointer to the current
 *					head structure
 *		char	ib[NINPUT]	.rel file text line
 *		char	*ip		pointer into the .rel file
 *		lfile	*linkp		pointer to first lfile structure
 *					containing an input .rel file
 *					specification
 *		int	lkerr		error flag
 *		int	mflag		Map output flag
 *		int	oflag		Output file type flag
 *		FILE	*ofp		Output file handle
 *					for word formats
 *		FILE	*ofph		Output file handle
 *					for high byte format
 *		FILE	*ofpl		Output file handle
 *					for low byte format
 *		int	pass		linker pass number
 *		int	pflag		print linker command file flag
 *		int	radix		current number conversion radix
 *		FILE	*sfp		The file handle sfp points to the
 *					currently open file
 *		lfile	*startp		asmlnk startup file structure
 *		FILE *	stdin		c_library
 *		FILE *	stdout		c_library
 *
 *	functions called:
 *		FILE *	afile()		lkmain.c
 *		int	fclose()	c_library
 *		int	fprintf()	c_library
 *		int	lk_getline()	lklex.c
 *		VOID	library()	lklibr.c
 *		VOID	link_main()	lkmain.c
 *		VOID	lkexit()	lkmain.c
 *		VOID	lnkarea()	lkarea.c
 *		VOID	map()		lkmain.c
 *		VOID	new()		lksym.c
 *		int	parse()		lkmain.c
 *		VOID	reloc()		lkreloc.c
 *		VOID	search()	lklibr.c
 *		VOID	setbas()	lkmain.c
 *		VOID	setgbl()	lkmain.c
 *		VOID	symdef()	lksym.c
 *		VOID	usage()		lkmain.c
 *
 *	side effects:
 *		Completion of main() completes the linking process
 *		and may produce a map file (.map) and/or a linked
 *		data files (.ihx or .s19) and/or one or more
 *		relocated listing files (.rst).
 */

int
main(argc, argv)
int argc;
char *argv[];
{
	register char *p;
	register int c, i;

	/* sdas specific */
	/* sdas initialization */
	sdld_init(argv[0]);
	/* end sdas specific */

	if (TARGET_IS_GB)
		gb_init();
	if (!is_sdld())
		fprintf(stdout, "\n");

	startp = (struct lfile *) new (sizeof (struct lfile));

	pflag = 1;
	for (i=1; i<argc; ++i) {
		p = argv[i];
		if (*p == '-') {
			while (ctype[c = *(++p)] & LETTER) {
				switch(c) {

				case 'c':
				case 'C':
					startp->f_type = F_STD;
					break;

				case 'f':
				case 'F':
					startp->f_type = F_LNK;
					break;

				case 'n':
				case 'N':
					pflag = 0;
					break;

				case 'p':
				case 'P':
					pflag = 1;
					break;

				default:
					usage();
				}
			}
		} else {
			if (startp->f_type == F_LNK) {
				startp->f_idp = p;
			}
		}
	}
	if (startp->f_type == 0)
		usage();
	if (startp->f_type == F_LNK && startp->f_idp == NULL)
		usage();

	cfp = NULL;
	sfp = NULL;
	filep = startp;
	while (1) {
		ip = ib;
		if (lk_getline() == 0)
			break;
		if (pflag && sfp != stdin)
			fprintf(stdout, "%s\n", ip);
		if (*ip == '\0' || parse())
			break;
	}

	if (sfp) {
		fclose(sfp);
		sfp = NULL;
	}

	if (linkp == NULL)
		usage();

	/*
	 * If no input file is specified
	 * then assume a single file with
	 * the same name as the output file.
	 */
	if (lfp == linkp) {
		lfp->f_flp = (struct lfile *) new (sizeof (struct lfile));
		lfp = lfp->f_flp;
		lfp->f_idp = strsto(linkp->f_idp);
		lfp->f_type = F_REL;
	}

	if (TARGET_IS_GB)
		gb_init_banks();
	syminit();

	/* sdld specific */
	if (dflag){
		//dfp = afile("temp", "cdb", 1);
		SaveLinkedFilePath(linkp->f_idp);	//Must be the first one...
		dfp = afile(linkp->f_idp, "cdb" ,1);	//JCF: Nov 30, 2002
		if (dfp == NULL)
			lkexit(1);
	}
	/* end sdld specific */

	for (pass=0; pass<2; ++pass) {
		cfp = NULL;
		sfp = NULL;
		filep = linkp->f_flp;
		hp = NULL;
		radix = 10;

		/* sdld specific */
		if (TARGET_IS_8051 || TARGET_IS_6808)
			Areas51(); /*JCF: Create the default 8051 areas in the right order*/
		/* end sdld specific */

		while (lk_getline()) {
			ip = ib;

			/* sdld specific */
			/* pass any "magic comments" to NoICE output */
			if ((ip[0] == ';') && (ip[1] == '!') && jfp) {
				fprintf( jfp, "%s\n", &ip[2] );
			}
			/* end sdld specific */

			link_main();
		}
		if (pass == 0) {
			/*
			 * Search libraries for global symbols
			 */
			search();
			/*
			 * Set area base addresses.
			 */
			setbas();
			/*
			 * Link all area addresses.
			 */
			if (!packflag)
				lnkarea();
			else {
				/* sdld 8051 specific */
				lnkarea2();
				/* end sdld 8051 specific */
			}
			/*
			 * Process global definitions.
			 */
			setgbl();
			/*
			 * Check for undefined globals.
			 */
			symdef(stderr);

			/* sdld specific */
			/* Open NoICE output file if requested */
			if (jflag) {
				jfp = afile(linkp->f_idp, "noi", 1);
				if (jfp == NULL) {
					lkexit(1);
				}
			}
			/* end sdld specific */

			/*
			 * Output Link Map if requested,
			 * or if NoICE output requested (since NoICE
			 * file is generated in part by map() processing)
			 */
			if (mflag || jflag)
				map();

			/* sdld specific */
			if (sflag) {	/*JCF: memory usage summary output*/
				if (!packflag) {
					if (summary(areap)) lkexit(1);
				}
				else {
					/* sdld 8051 specific */
					if (summary2(areap)) lkexit(1);
					/* end sdld 8051 specific */
				}
			}

			if ((iram_size) && (!packflag))
				iramcheck();
			/* end sdld specific */

			/*
			 * Open output file
			 */
			if (oflag == 1) {
				ofp = afile(linkp->f_idp, "ihx", 1);
				if (ofp == NULL) {
					lkexit(1);
				}
				/* sdld specific */
				/* include NoICE command to load hex file */
				if (jfp) fprintf( jfp, "LOAD %s.ihx\n", linkp->f_idp );
				/* end sdld specific */
			} else
			if (oflag == 2) {
				ofp = afile(linkp->f_idp, "s19", 1);
				if (ofp == NULL) {
					lkexit(1);
				}
				/* sdld specific */
				/* include NoICE command to load hex file */
				if (jfp) fprintf( jfp, "LOAD %s.s19\n", linkp->f_idp );
				/* end sdld specific */
			} else
			/* sdld specific */
			if (oflag == 3) {
				ofp = afile(linkp->f_idp, "elf", 2);
				if (ofp == NULL) {
					lkexit(1);
				}
			}
			else
			if (oflag == 4) {
				ofp = afile(linkp->f_idp, "", 2);
				if (ofp == NULL) {
					lkexit(1);
				}
			}
			/* end sdld specific */
		} else {
			/*
			 * Link in library files
			 */
			library();
			reloc('E');
		}
	}
	if (TARGET_IS_8051 || TARGET_IS_6808) {
		//JCF:
		CreateAOMF51();
	}

	lkexit(lkerr);

	/* Never get here. */
	return 0;
}

/*)Function	VOID	lkexit(i)
 *
 *			int	i	exit code
 *
 *	The function lkexit() explicitly closes all open
 *	files and then terminates the program.
 *
 *	local variables:
 *		none
 *
 *	global variables:
 *		FILE *	mfp		file handle for .map
 *		FILE *	ofp		file handle for .ihx/.s19
 *		FILE *	rfp		file hanlde for .rst
 *		FILE *	sfp		file handle for .rel
 *		FILE *	tfp		file handle for .lst
 *
 *	functions called:
 *		int	fclose()	c_library
 *		VOID	exit()		c_library
 *
 *	side effects:
 *		All files closed. Program terminates.
 */

VOID
lkexit(i)
int i;
{
	/* sdld 8051 specific */
	if (jfp != NULL) fclose(jfp);
	if (dfp != NULL) fclose(dfp);
	/* end sdld 8051 specific */
	if (mfp != NULL) fclose(mfp);
	if (ofp != NULL) fclose(ofp);
	if (rfp != NULL) fclose(rfp);
	if (sfp != NULL) fclose(sfp);
	if (tfp != NULL) fclose(tfp);
	exit(i);
}

/*)Function	link_main()
 *
 *	The function link_main() evaluates the directives for each line of
 *	text read from the .rel file(s).  The valid directives processed
 *	are:
 *		X, D, Q, H, M, A, S, T, R, and P.
 *
 *	local variables:
 *		int	c		first non blank character of a line
 *
 *	global variables:
 *		head	*headp		The pointer to the first
 *					head structure of a linked list
 *		head	*hp		Pointer to the current
 *					head structure
 *		int	pass		linker pass number
 *		int	radix		current number conversion radix
 *
 *	functions called:
 *		char	endline()	lklex.c
 *		VOID	module()	lkhead.c
 *		VOID	newarea()	lkarea.c
 *		VOID	newhead()	lkhead.c
 *		sym *	newsym()	lksym.c
 *		VOID	reloc()		lkreloc.c
 *
 *	side effects:
 *		Head, area, and symbol structures are created and
 *		the radix is set as the .rel file(s) are read.
 */

VOID
link_main()
{
	register char c;

	if ((c=endline()) == 0) { return; }
	switch (c) {

	/* sdld specific */
	case 'O': /* For some important sdcc options */
		if (is_sdld() && pass == 0) {
			if (NULL == optsdcc) {
				optsdcc = strsto(&ip[1]);
				optsdcc_module = hp->m_id;
			}
			else {
				if (strcmp(optsdcc, &ip[1]) != 0) {
					fprintf(stderr,
						"?ASlink-Warning-Conflicting sdcc options:\n"
						"   \"%s\" in module \"%s\" and\n"
						"   \"%s\" in module \"%s\".\n",
						optsdcc, optsdcc_module, &ip[1], hp->m_id);
					lkerr++;
				}
			}
		}
		break;
	/* end sdld specific */

	case 'X':
		radix = 16;
		break;

	case 'D':
		radix = 10;
		break;

	case 'Q':
		radix = 8;
		break;

	case 'H':
		if (pass == 0) {
			newhead();
		} else {
			if (hp == 0) {
				hp = headp;
			} else {
				hp = hp->h_hp;
			}
		}
		sdp.s_area = NULL;
		sdp.s_areax = NULL;
		sdp.s_addr = 0;
		break;

	case 'M':
		if (pass == 0) {
			module();
		}
		break;

	case 'A':
		if (pass == 0)
			newarea();
		if (sdp.s_area == NULL) {
			sdp.s_area = areap;
			sdp.s_areax = areap->a_axp;
			sdp.s_addr = 0;
		}
		break;

	case 'S':
		if (pass == 0)
			newsym();
		break;

	case 'T':
	case 'R':
	case 'P':
		if (pass == 0)
			break;
		reloc(c);
		break;

	default:
		break;
	}
	if (c == 'X' || c == 'D' || c == 'Q') {
		if ((c = get()) == 'H') {
			hilo = 1;
		} else
		if (c == 'L') {
			hilo = 0;
		}
	}
}

/*)Function	VOID	map()
 *
 *	The function map() opens the output map file and calls the various
 *	routines to
 *	(1) output the variables in each area,
 *	(2) list the files processed with module names,
 *	(3) list the libraries file processed,
 *	(4) list base address definitions,
 *	(5) list global variable definitions, and
 *	(6) list any undefined variables.
 *
 *	local variables:
 *		int	i		counter
 *		head *	hdp		pointer to head structure
 *		lbfile *lbfh		pointer to library file structure
 *
 *	global variables:
 *		area	*ap		Pointer to the current
 *					area structure
 *		area	*areap		The pointer to the first
 *					area structure of a linked list
 *		base	*basep		The pointer to the first
 *					base structure
 *		base	*bsp		Pointer to the current
 *					base structure
 *		lfile	*filep		The pointer *filep points to the
 *					beginning of a linked list of
 *					lfile structures.
 *		globl	*globlp		The pointer to the first
 *					globl structure
 *		globl	*gsp		Pointer to the current
 *					globl structure
 *		head	*headp		The pointer to the first
 *					head structure of a linked list
 *		lbfile	*lbfhead	The pointer to the first
 *					lbfile structure of a linked list
 *		lfile	*linkp		pointer to first lfile structure
 *					containing an input REL file
 *					specification
 *		int	lop		current line number on page
 *		FILE	*mfp		Map output file handle
 *		int	page		current page number
 *
 *	functions called:
 *		FILE *	afile()		lkmain.c
 *		int	fprintf()	c_library
 *		VOID	lkexit()	lkmain.c
 *		VOID	lstarea()	lklist.c
 *		VOID	newpag()	lklist.c
 *		VOID	symdef()	lksym.c
 *
 *	side effects:
 *		The map file is created.
 */

VOID
map()
{
  /* sdld specific */
  /* if gb output file format, generate gb map file */
  if (oflag == 4) {
        register int i;
        register struct head *hdp;
        register struct lbfile *lbfh;

        /*
         * Open Map File
         */
        mfp = afile(linkp->f_idp, "map", 1);
        if (mfp == NULL) {
                lkexit(1);
        }

        /*
         * Output Map Area Lists
         */
        page = 0;
        lop  = NLPP;
        ap = areap;
        while (ap) {
                lstarea(ap);
                ap = ap->a_ap;
        }
        /*
         * List Linked Files
         */
        newpag(mfp);
        fprintf(mfp, "\nFiles Linked      [ module(s) ]\n\n");
        hdp = headp;
        filep = linkp->f_flp;
        while (filep) {
                fprintf(mfp, "%-16s", filep->f_idp);
                i = 0;
                while ((hdp != NULL) && (hdp->h_lfile == filep)) {
                        if (i % 5) {
                            fprintf(mfp, ", %8.8s", hdp->m_id);
                        } else {
                            if (i) {
                                fprintf(mfp, ",\n%20s%8.8s", "", hdp->m_id);
                            } else {
                                fprintf(mfp, "  [ %8.8s", hdp->m_id);
                            }
                        }
                        hdp = hdp->h_hp;
                        i++;
                }
                if (i)
                        fprintf(mfp, " ]");
                fprintf(mfp, "\n");
                filep = filep->f_flp;
        }
        /*
         * List Linked Libraries
         */
        if (lbfhead != NULL) {
                fprintf(mfp,
        "\nLibraries Linked                    [   object  file   ]\n\n");
                for (lbfh=lbfhead; lbfh; lbfh=lbfh->next) {
                        fprintf(mfp, "%-32s    [ %16.16s ]\n",
                                lbfh->libspc, lbfh->relfil);
                }
                fprintf(mfp, "\n");
        }
        /*
         * List Base Address Definitions
         */
        if (basep) {
                newpag(mfp);
                fprintf(mfp, "\nUser Base Address Definitions\n\n");
                bsp = basep;
                while (bsp) {
                        fprintf(mfp, "%s\n", bsp->b_strp);
                        bsp = bsp->b_base;
                }
        }
        /*
         * List Global Definitions
         */
        if (globlp) {
                newpag(mfp);
                fprintf(mfp, "\nUser Global Definitions\n\n");
                gsp = globlp;
                while (gsp) {
                        fprintf(mfp, "%s\n", gsp->g_strp);
                        gsp = gsp->g_globl;
                }
        }
        fprintf(mfp, "\n\f");
        symdef(mfp);
  } else {
  /* end sdld specific */
	register int i;
	register struct head *hdp;
	register struct lbfile *lbfh;

	/*
	 * Open Map File
	 */
	mfp = afile(linkp->f_idp, "map", 1);
	if (mfp == NULL) {
		lkexit(1);
	}

	/*
	 * Output Map Area Lists
	 */
	page = 0;
	lop  = NLPP;
	ap = areap;
	while (ap) {
		lstarea(ap);
		ap = ap->a_ap;
	}
	/*
	 * List Linked Files
	 */
	newpag(mfp);
	fprintf(mfp,
"\nFiles Linked                              [ module(s) ]\n\n");
	hdp = headp;
	filep = linkp->f_flp;
	while (filep) {
		fprintf(mfp, "%-40.40s  [ ", filep->f_idp);
		i = 0;
		while ((hdp != NULL) && (hdp->h_lfile == filep)) {
			if (i)
				fprintf(mfp, ",\n%44s", "");
			fprintf(mfp, "%-.32s", hdp->m_id);
			hdp = hdp->h_hp;
			i++;
		}
		if (i)
			fprintf(mfp, " ]");
		fprintf(mfp, "\n");
		filep = filep->f_flp;
	}
	fprintf(mfp, "\n");
	/*
	 * List Linked Libraries
	 */
	if (lbfhead != NULL) {
		fprintf(mfp,
"\nLibraries Linked                          [ object file ]\n\n");
		for (lbfh=lbfhead; lbfh; lbfh=lbfh->next) {
			fprintf(mfp, "%-40.40s  [ %-.32s ]\n",
				lbfh->libspc, lbfh->relfil);
		}
		fprintf(mfp, "\n");
	}
	/*
	 * List Base Address Definitions
	 */
	if (basep) {
		newpag(mfp);
		fprintf(mfp, "\nUser Base Address Definitions\n\n");
		bsp = basep;
		while (bsp) {
			fprintf(mfp, "%s\n", bsp->b_strp);
			bsp = bsp->b_base;
		}
	}
	/*
	 * List Global Definitions
	 */
	if (globlp) {
		newpag(mfp);
		fprintf(mfp, "\nUser Global Definitions\n\n");
		gsp = globlp;
		while (gsp) {
			fprintf(mfp, "%s\n", gsp->g_strp);
			gsp = gsp->g_globl;
		}
	}
	fprintf(mfp, "\n\f");
	symdef(mfp);
  /* sdld specific */
  }
  /* end sdld specific */
}

/*)Function	int	parse()
 *
 *	The function parse() evaluates all command line or file input
 *	linker directives and updates the appropriate variables.
 *
 *	local variables:
 *		int	c		character value
 *		char	fid[]		file id string
 *
 *	global variables:
 *		char	ctype[]		array of character types, one per
 *					ASCII character
 *		lfile	*lfp		pointer to current lfile structure
 *					being processed by parse()
 *		lfile	*linkp		pointer to first lfile structure
 *					containing an input REL file
 *					specification
 *		int	mflag		Map output flag
 *		int	oflag		Output file type flag
 *		int	pflag		print linker command file flag
 *		FILE *	stderr		c_library
 *		int	uflag		Relocated listing flag
 *		int	xflag		Map file radix type flag
 *		int	wflag		Wide listing format
 *		int	zflag		Enable symbol case sensitivity
 *
 *	Functions called:
 *		VOID	addlib()	lklibr.c
 *		VOID	addpath()	lklibr.c
 *		VOID	bassav()	lkmain.c
 *		int	fprintf()	c_library
 *		VOID	gblsav()	lkmain.c
 *		VOID	getfid()	lklex.c
 *		char	getnb()		lklex.c
 *		VOID	lkexit()	lkmain.c
 *		char *	strsto()	lksym.c
 *		int	strlen()	c_library
 *
 *	side effects:
 *		Various linker flags are updated and the linked
 *		structure lfile is created.
 */

int
parse()
{
	register int c;
	char fid[NINPUT];

	/* sdld specific */
	wflag = 1;
	zflag = 1;
	/* end sdld specific */

	while ((c = getnb()) != 0) {
		/* sdld specific */
		if ( c == ';')
			return(0);
		/* end sdld specific */
		if ( c == '-') {
			while (ctype[c=get()] & LETTER) {
				switch(c) {

				case 'i':
				case 'I':
					oflag = 1;
					break;

				case 's':
				case 'S':
					oflag = 2;
					break;

				case 't':
				case 'T':
					if (TARGET_IS_6808)
						oflag = 3;
					else
						goto err;
					break;

				case 'm':
				case 'M':
					++mflag;
					break;

				case 'y': /*JCF: memory usage summary output*/
					if (is_sdld()) {
						if (TARGET_IS_GB) {
							c = get();
							if(c == 'O' || c == 'o')
								nb_rom_banks = expr(0);
							else if(c == 'A' || c == 'a')
								nb_ram_banks = expr(0);
							else if(c == 'T' || c == 't')
								mbc_type = expr(0);
							else if(c == 'N' || c == 'n') {
								int i = 0;
								if(getnb() != '=' || getnb() != '"') {
									fprintf(stderr, "Syntax error in -yn=\"name\" flag\n");
									lkexit(1);
								}
								while((c = get()) != '"' && i < 16) {
									cart_name[i++] = c;
								}
								if(i < 16)
									cart_name[i] = 0;
								else
									while(get() != '"')
										;
							} else if(c == 'P' || c == 'p') {
								patch *p = patches;

								patches = (patch *)malloc(sizeof(patch));
								patches->next = p;
								patches->addr = expr(0);
								if(getnb() != '=') {
									fprintf(stderr, "Syntax error in -YHaddr=val flag\n");
									lkexit(1);
								}
								patches->value = expr(0);
							} else {
								fprintf(stderr, "Invalid option\n");
								lkexit(1);
							}
						}
						else
							++sflag;
					}
					else
						goto err;
					break;

				case 'Y':
					if (TARGET_IS_8051) {
						unget(getnb());
						packflag=1;
					}
					else if (TARGET_IS_6808) {
						++sflag;
					}
					else
						goto err;
					break;

				case 'A':
					if (TARGET_IS_8051) {
						unget(getnb());
						if (ip && *ip)
						{
							stacksize=expr(0);
							if(stacksize>256) stacksize=256;
							else if(stacksize<0) stacksize=0;
						}
						return(0);
					}
					else
						goto err;
					break;

				case 'a':
					if (is_sdld() && !(TARGET_IS_Z80 || TARGET_IS_GB)) {
						iramsav();
						return(0);
					}
					else
						goto err;
					break;

				case 'v':
				case 'V':
					if (is_sdld() && !(TARGET_IS_Z80 || TARGET_IS_GB)) {
						xramsav();
						return(0);
					}
					else
						goto err;
					break;

				case 'w':
				case 'W':
					if (is_sdld() && !(TARGET_IS_Z80 || TARGET_IS_GB)) {
						codesav();
						return(0);
					}
					else
						++wflag;
					break;

				case 'Z':
					if (TARGET_IS_Z80 || TARGET_IS_GB) {
						oflag = 4;
						break;
					}
					/* fall through */
				case 'z':
					if (is_sdld()) {
						dflag = 1;
						return(0);
					}
					else
						++zflag;
					break;

				case 'j':
				case 'J':
					if (is_sdld()) {
						if (TARGET_IS_Z80 || TARGET_IS_GB) {
							++symflag;
						}
						else
							jflag = 1;
					}
					else
						goto err;
					break;

				case 'r':
				case 'R':
					if (is_sdld() && !(TARGET_IS_Z80 || TARGET_IS_GB))
						rflag = 1;
					else
						goto err;
					break;

				case 'u':
				case 'U':
					uflag = 1;
					break;

				case 'x':
				case 'X':
					xflag = 0;
					break;

				case 'q':
				case 'Q':
					xflag = 1;
					break;

				case 'd':
				case 'D':
					xflag = 2;
					break;

				case 'e':
				case 'E':
					return(1);

				case 'n':
				case 'N':
					pflag = 0;
					break;

				case 'p':
				case 'P':
					pflag = 1;
					break;

				case 'b':
				case 'B':
					bassav();
					return(0);

				case 'g':
				case 'G':
					gblsav();
					return(0);

				case 'k':
				case 'K':
					addpath();
					return(0);

				case 'l':
				case 'L':
					addlib();
					return(0);

				default:
				err:
					fprintf(stderr,
					    "Unkown option -%c ignored\n", c);
					break;
				}
			}
			/* sdld specific */
			if ( c == ';')
				return(0);
			/* end sdld specific */
		} else
		if (ctype[c] & ILL) {
			fprintf(stderr, "Invalid input\n");
			lkexit(1);
		} else {
			if (linkp == NULL) {
				linkp = (struct lfile *)
					new (sizeof (struct lfile));
				lfp = linkp;
				lfp->f_type = F_OUT;
			} else {
				lfp->f_flp = (struct lfile *)
						new (sizeof (struct lfile));
				lfp = lfp->f_flp;
				lfp->f_type = F_REL;
			}
			getfid(fid, c);
			lfp->f_idp = strsto(fid);
		}
	}
	return(0);
}

/*)Function	VOID	bassav()
 *
 *	The function bassav() creates a linked structure containing
 *	the base address strings input to the linker.
 *
 *	local variables:
 *		none
 *
 *	global variables:
 *		base	*basep		The pointer to the first
 *					base structure
 *		base	*bsp		Pointer to the current
 *					base structure
 *		char	*ip		pointer into the REL file
 *					text line in ib[]
 *
 *	 functions called:
 *		char	getnb()		lklex.c
 *		VOID *	new()		lksym.c
 *		int	strlen()	c_library
 *		char *	strcpy()	c_library
 *		VOID	unget()		lklex.c
 *
 *	side effects:
 *		The basep structure is created.
 */

VOID
bassav()
{
	if (basep == NULL) {
		basep = (struct base *)
			new (sizeof (struct base));
		bsp = basep;
	} else {
		bsp->b_base = (struct base *)
				new (sizeof (struct base));
		bsp = bsp->b_base;
	}
	unget(getnb());
	bsp->b_strp = (char *) new (strlen(ip)+1);
	strcpy(bsp->b_strp, ip);
}

/*)Function	VOID	setbas()
 *
 *	The function setbas() scans the base address lines in the
 *	basep structure, evaluates the arguments, and sets beginning
 *	address of the specified areas.
 *
 *	local variables:
 *		int	v		expression value
 *		char	id[]		base id string
 *
 *	global variables:
 *		area	*ap		Pointer to the current
 *					area structure
 *		area	*areap		The pointer to the first
 *					area structure of a linked list
 *		base	*basep		The pointer to the first
 *					base structure
 *		base	*bsp		Pointer to the current
 *					base structure
 *		char	*ip		pointer into the REL file
 *					text line in ib[]
 *		int	lkerr		error flag
 *
 *	 functions called:
 *		a_uint	expr()		lkeval.c
 *		int	fprintf()	c_library
 *		VOID	getid()		lklex.c
 *		char	getnb()		lklex.c
 *		int	symeq()		lksym.c
 *
 *	side effects:
 *		The base address of an area is set.
 */

VOID
setbas()
{
	register int v;
	char id[NCPS];

	bsp = basep;
	while (bsp) {
		ip = bsp->b_strp;
		getid(id, -1);
		if (getnb() == '=') {
			v = expr(0);
			for (ap = areap; ap != NULL; ap = ap->a_ap) {
				if (symeq(id, ap->a_id, 0))
					break;
			}
			if (ap == NULL) {
				if (!TARGET_IS_GB) {
					fprintf(stderr,
					"ASlink-Warning-No definition of area %s\n", id);
					lkerr++;
				}
			} else {
				ap->a_addr = v;
				/* sdld specific */
				ap->a_type = 1; /* JLH: value set */
				/* end sdld specific */
			}
		} else {
			fprintf(stderr, "ASlink-Warning-No '=' in base expression");
			lkerr++;
		}
		bsp = bsp->b_base;
	}
}

/*)Function	VOID	gblsav()
 *
 *	The function gblsav() creates a linked structure containing
 *	the global variable strings input to the linker.
 *
 *	local variable:
 *		none
 *
 *	global variables:
 *		globl	*globlp		The pointer to the first
 *					globl structure
 *		globl	*gsp		Pointer to the current
 *					globl structure
 *		char	*ip		pointer into the REL file
 *					text line in ib[]
 *		int	lkerr		error flag
 *
 *	functions called:
 *		char	getnb()		lklex.c
 *		VOID *	new()		lksym.c
 *		int	strlen()	c_library
 *		char *	strcpy()	c_library
 *		VOID	unget()		lklex.c
 *
 *	side effects:
 *		The globlp structure is created.
 */

VOID
gblsav()
{
	if (globlp == NULL) {
		globlp = (struct globl *)
			new (sizeof (struct globl));
		gsp = globlp;
	} else {
		gsp->g_globl = (struct globl *)
				new (sizeof (struct globl));
		gsp = gsp->g_globl;
	}
	unget(getnb());
	gsp->g_strp = (char *) new (strlen(ip)+1);
	strcpy(gsp->g_strp, ip);
}

/*)Function	VOID	setgbl()
 *
 *	The function setgbl() scans the global variable lines in the
 *	globlp structure, evaluates the arguments, and sets a variable
 *	to this value.
 *
 *	local variables:
 *		int	v		expression value
 *		char	id[]		base id string
 *		sym *	sp		pointer to a symbol structure
 *
 *	global variables:
 *		char	*ip		pointer into the REL file
 *					text line in ib[]
 *		globl	*globlp		The pointer to the first
 *					globl structure
 *		globl	*gsp		Pointer to the current
 *					globl structure
 *		FILE *	stderr		c_library
 *		int	lkerr		error flag
 *
 *	 functions called:
 *		a_uint	expr()		lkeval.c
 *		int	fprintf()	c_library
 *		VOID	getid()		lklex.c
 *		char	getnb()		lklex.c
 *		sym *	lkpsym()	lksym.c
 *
 *	side effects:
 *		The value of a variable is set.
 */

VOID
setgbl()
{
	register int v;
	register struct sym *sp;
	char id[NCPS];

	gsp = globlp;
	while (gsp) {
		ip = gsp->g_strp;
		getid(id, -1);
		if (getnb() == '=') {
			v = expr(0);
			sp = lkpsym(id, 0);
			if (sp == NULL) {
				if (!TARGET_IS_GB) {
					fprintf(stderr,
					"No definition of symbol %s\n", id);
					lkerr++;
				}
			} else {

				if (sp->s_type & S_DEF) {
					fprintf(stderr,
					"Redefinition of symbol %s\n", id);
					lkerr++;
					sp->s_axp = NULL;
				}
				sp->s_addr = v;
				sp->s_type |= S_DEF;
			}
		} else {
			fprintf(stderr, "No '=' in global expression");
			lkerr++;
		}
		gsp = gsp->g_globl;
	}
}

/*)Function	FILE *	afile(fn, ft, wf)
 *
 *		char *	fn		file specification string
 *		char *	ft		file type string
 *		int	wf		0 ==>> read
 *					1 ==>> write
 *					2 ==>> binary write
 *
 *	The function afile() opens a file for reading or writing.
 *		(1)	If the file type specification string ft
 *			is not NULL then a file specification is
 *			constructed with the file path\name in fn
 *			and the extension in ft.
 *		(2)	If the file type specification string ft
 *			is NULL then the file specification is
 *			constructed from fn.  If fn does not have
 *			a file type then the default .rel file
 *			type is appended to the file specification.
 *
 *	afile() returns a file handle for the opened file or aborts
 *	the assembler on an open error.
 *
 *	local variables:
 *		int	c		character value
 *		FILE *	fp		filehandle for opened file
 *		char *	p1		pointer to filespec string fn
 *		char *	p2		pointer to filespec string fb
 *		char *	p3		pointer to filetype string ft
 *
 *	global variables:
 *		char	afspec[]	constructed file specification string
 *		int	lkerr		error flag
 *
 *	functions called:
 *		int	fndidx()	lkmain.c
 *		FILE *	fopen()		c_library
 *		int	fprintf()	c_library
 *
 *	side effects:
 *		File is opened for read or write.
 */

FILE *
afile(fn, ft, wf)
char *fn;
char *ft;
int wf;
{
	char *p1, *p2;
	int c;
	char * frmt;
	FILE *fp;

	if (strlen(fn) > (FILSPC-7)) {
		fprintf(stderr, "?ASlink-Error-<filspc to long> : \"%s\"\n", fn);
		lkerr++;
		return(NULL);
	}

	/*
	 * Skip The Path
	 */
	strcpy(afspec, fn);
	c = fndidx(afspec);

	/*
	 * Skip to File Extension separator
	 */
	p1 = strrchr(&afspec[c], FSEPX);

	/*
	 * Copy File Extension
	 */
	 p2 = ft;
	 if (*p2 == 0) {
		if (p1 == NULL) {
			p2 = LKOBJEXT;
		} else {
			p2 = strrchr(&fn[c], FSEPX) + 1;
		}
	}
	if (p1 == NULL) {
		p1 = &afspec[strlen(afspec)];
	}
	*p1++ = FSEPX;
	while ((c = *p2++) != 0) {
		if (p1 < &afspec[FILSPC-1])
			*p1++ = c;
	}
	*p1++ = 0;

	/*
	 * Select Read/Write/Binary Write
	 */
	switch(wf) {
	default:
	case 0:	frmt = "r";	break;
	case 1:	frmt = "w";	break;
#ifdef	DECUS
	case 2:	frmt = "wn";	break;
#else
	case 2:	frmt = "wb";	break;
#endif
	}
	if ((fp = fopen(afspec, frmt)) == NULL && strcmp(ft,"adb") != 0) { /* Do not complaint for optional adb files */
		fprintf(stderr, "?ASlink-Error-<cannot %s> : \"%s\"\n", wf?"create":"open", afspec);
		lkerr++;
	}
	return (fp);
}

/*)Function	int	fndidx(str)
 *
 *		char *	str		file specification string
 *
 *	The function fndidx() scans the file specification string
 *	to find the index to the file name.  If the file
 *	specification contains a 'path' then the index will
 *	be non zero.
 *
 *	fndidx() returns the index value.
 *
 *	local variables:
 *		char *	p1		temporary pointer
 *		char *	p2		temporary pointer
 *
 *	global variables:
 *		none
 *
 *	functions called:
 *		char *	strrchr()	c_library
 *
 *	side effects:
 *		none
 */

int
fndidx(str)
char *str;
{
	char *p1, *p2;

	/*
	 * Skip Path Delimiters
	 */
	p1 = str;
	if ((p2 = strrchr(p1,  ':')) != NULL) { p1 = p2 + 1; }
	if ((p2 = strrchr(p1,  '/')) != NULL) { p1 = p2 + 1; }
	if ((p2 = strrchr(p1, '\\')) != NULL) { p1 = p2 + 1; }

	return((int) (p1 - str));
}

/*)Function	int	fndext(str)
 *
 *		char *	str		file specification string
 *
 *	The function fndext() scans the file specification string
 *	to find the file.ext separater.
 *
 *	fndext() returns the index to FSEPX or the end of the string.
 *
 *	local variables:
 *		char *	p1		temporary pointer
 *		char *	p2		temporary pointer
 *
 *	global variables:
 *		none
 *
 *	functions called:
 *		char *	strrchr()	c_library
 *
 *	side effects:
 *		none
 */

int
fndext(str)
char * str;
{
	char *p1, *p2;

	/*
	 * Find the file separator
	 */
	p1 = str + strlen(str);
	if ((p2 = strrchr(str,  FSEPX)) != NULL) { p1 = p2; }

	return((int) (p1 - str));
}

/* sdld specific */
/*)Function	VOID	iramsav()
 *
 *	The function iramsav() stores the size of the chip's internal RAM.
 *	This is used after linking to check that variable assignment to this
 *	dataspace didn't overflow into adjoining segments.	Variables in the
 *	DSEG, OSEG, and ISEG are assigned to this dataspace.
 *
 *	local variables:
 *		none
 *
 *	global variables:
 *		char	*ip		pointer into the REL file
 *					text line in ib[]
 *		unsigned int		size of chip's internal
 *		iram_size		RAM segment
 *
 *	 functions called:
 *		char	getnb()		lklex.c
 *		VOID	unget()		lklex.c
 *		a_uint	expr()		lkeval.c
 *
 *	side effects:
 *		The iram_size may be modified.
 */

VOID
iramsav()
{
  unget(getnb());
  if (ip && *ip)
	iram_size = expr(0);	/* evaluate size expression */
  else
	iram_size = 128;		/* Default is 128 (0x80) bytes */
  if ((iram_size<=0) || (iram_size>256))
	iram_size = 128;		/* Default is 128 (0x80) bytes */
}

/*Similar to iramsav but for xram memory*/
VOID
xramsav()
{
  unget(getnb());
  if (ip && *ip)
	xram_size = expr(0);	/* evaluate size expression */
  else
	xram_size = rflag?0x1000000:0x10000;
}

/*Similar to iramsav but for code memory*/
VOID
codesav()
{
  unget(getnb());
  if (ip && *ip)
	code_size = expr(0);	/* evaluate size expression */
  else
	code_size = rflag?0x1000000:0x10000;
}


/*)Function	VOID	iramcheck()
 *
 *	The function iramcheck() is used at the end of linking to check that
 *	the internal RAM area wasn't overflowed by too many variable
 *	assignments.  Variables in the DSEG, ISEG, and OSEG are assigned to
 *	the chip's internal RAM.
 *
 *	local variables:
 *		none
 *
 *	global variables:
 *		unsigned int		size of chip's internal
 *		iram_size		RAM segment
 *		struct area		linked list of memory
 *		*areap			areas
 *
 *	 functions called:
 *
 *	side effects:
 */

VOID
iramcheck()
{
  register unsigned int last_addr;
  register struct area *ap;

  for (ap = areap; ap; ap=ap->a_ap) {
	if ((ap->a_size != 0) &&
		(!strcmp(ap->a_id, "DSEG") ||
		 !strcmp(ap->a_id, "OSEG") ||
		 !strcmp(ap->a_id, "ISEG")
		)
	   )
	{
	  last_addr = ap->a_addr + ap->a_size - 1;
	  if (last_addr >= iram_size)
		fprintf(stderr,
		  "\nWARNING! Segment %s extends past the end\n"
		  "         of internal RAM.  Check map file.\n",
		  ap->a_id);
	}
  }
}
/* end sdld specific */

char *usetxt[] = {
	"Usage: [-Options] [-Option with arg] file",
	"Usage: [-Options] [-Option with arg] outfile file1 [file2 ...]",
	"Startup:",
	"  -c                           Command line input",
	"  -f   file[lnk]               File input",
	"  -p   Prompt and echo of file[lnk] to stdout (default)",
	"  -n   No echo of file[lnk] to stdout",
	"Usage: [-Options] file [file ...]",
	"Libraries:",
	"  -k	Library path specification, one per -k",
	"  -l	Library file specification, one per -l",
	"Relocation:",
	"  -b   area base address = expression",
	"  -g   global symbol = expression",
	"Map format:",
	"  -m   Map output generated as file[map]",
	"  -x   Hexidecimal (default)",
	"  -d   Decimal",
	"  -q   Octal",
	"Output:",
	"  -i   Intel Hex as file[ihx]",
	"  -s   Motorola S19 as file[s19]",
	"List:",
	"  -u	Update listing file(s) with link data as file(s)[.rst]",
	"End:",
	"  -e   or null line terminates input",
	"",
	0
};

char *usetxt_8051[] = {
	"Usage: [-Options] [-Option with arg] file",
	"Usage: [-Options] [-Option with arg] outfile file1 [file2 ...]",
	"Startup:",
	"  -c                           Command line input",
	"  -f   file[lnk]               File input",
	"  -p   Prompt and echo of file[lnk] to stdout (default)",
	"  -n   No echo of file[LNK] to stdout",
	"Libraries:",
	"  -k   Library path specification, one per -k",
	"  -l   Library file specification, one per -l",
	"Relocation:",
	"  -b   area base address = expression",
	"  -g   global symbol = expression",
	"Map format:",
	"  -m   Map output generated as file[map]",
	"  -w	Wide listing format for map file",
	"  -x   Hexidecimal (default)",
	"  -d   Decimal",
	"  -q   Octal",
	"Output:",
	"  -i   Intel Hex as file[ihx]",
	"  -s   Motorola S19 as file[s19]",
	"  -j   Produce NoICE debug as file[noi]",
	"  -z   Produce SDCdb debug as file[cdb]",
/*	"List:", */
	"  -u   Update listing file(s) with link data as file(s)[.rst]",
	"Miscellaneous:\n"
	"  -a   [iram-size] Check for internal RAM overflow",
	"  -v   [xram-size] Check for external RAM overflow",
	"  -w   [code-size] Check for code overflow",
	"  -y   Generate memory usage summary file[mem]",
	"  -Y   Pack internal ram",
	"  -A   [stack-size] Allocate space for stack",
	"Case Sensitivity:",
	"  -z	Enable Case Sensitivity for Symbols",
	"End:",
	"  -e   or null line terminates input",
	"",
	0
};

char *usetxt_6808[] = {
	"Usage: [-Options] [-Option with arg] file",
	"Usage: [-Options] [-Option with arg] outfile file1 [file2 ...]",
	"Startup:",
	"  -c                           Command line input",
	"  -f   file[lnk]               File input",
	"  -p   Prompt and echo of file[lnk] to stdout (default)",
	"  -n   No echo of file[lnk] to stdout",
	"Libraries:",
	"  -k   Library path specification, one per -k",
	"  -l   Library file specification, one per -l",
	"Relocation:",
	"  -b   area base address = expression",
	"  -g   global symbol = expression",
	"Map format:",
	"  -m   Map output generated as file[map]",
	"  -x   Hexidecimal (default)",
	"  -d   Decimal",
	"  -q   Octal",
	"Output:",
	"  -i   Intel Hex as file[ihx]",
	"  -s   Motorola S19 as file[s19]",
	"  -t   ELF executable as file[elf]",
	"  -j   Produce NoICE debug as file[noi]",
	"  -z   Produce SDCdb debug as file[cdb]",
/*	"List:", */
	"  -u   Update listing file(s) with link data as file(s)[.rst]",
	"Miscellaneous:\n"
	"  -a   [iram-size] Check for internal RAM overflow",
	"  -v   [xram-size] Check for external RAM overflow",
	"  -w   [code-size] Check for code overflow",
	"  -y   Generate memory usage summary file[mem]",
	"End:",
	"  -e   or null line terminates input",
	"",
	0
};

char *usetxt_z80[] = {
	"Usage: [-Options] [-Option with arg] file",
	"Usage: [-Options] [-Option with arg] outfile file1 [file2 ...]",
	"Startup:",
	"  -c                           Command line input",
	"  -f   file[lnk]               File input",
	"  -p   Prompt and echo of file[lnk] to stdout (default)",
	"  -n   No echo of file[lnk] to stdout",
	"Libraries:",
	"  -k   Library path specification, one per -k",
	"  -l   Library file specification, one per -l",
	"Relocation:",
	"  -b   area base address = expression",
	"  -g   global symbol = expression",
	"Map format:",
	"  -m   Map output generated as file[map]",
	"  -j   no$gmb symbol file generated as file[sym]",
	"  -x   Hexidecimal (default)",
	"  -d   Decimal",
	"  -q   Octal",
	"Output:",
	"  -i   Intel Hex as file[ihx]",
	"  -s   Motorola S19 as file[s19]",
	"  -z   Produce SDCdb debug as file[cdb]",
	"List:",
	"  -u   Update listing file(s) with link data as file(s)[.rst]",
	"End:",
	"  -e   or null line terminates input",
	"",
	0
};

char *usetxt_gb[] = {
	"Usage: [-Options] [-Option with arg] file",
	"Usage: [-Options] [-Option with arg] outfile file1 [file2 ...]",
	"Startup:",
	"  -c                           Command line input",
	"  -f   file[lnk]               File input",
	"  -p   Prompt and echo of file[lnk] to stdout (default)",
	"  -n   No echo of file[lnk] to stdout",
	"Libraries:",
	"  -k   Library path specification, one per -k",
	"  -l   Library file specification, one per -l",
	"Relocation:",
	"  -b   area base address = expression",
	"  -g   global symbol = expression",
	"  -yo  Number of rom banks (default: 2)",
	"  -ya  Number of ram banks (default: 0)",
	"  -yt  MBC type (default: no MBC)",
	"  -yn  Name of program (default: name of output file)",
	"  -yp# Patch one byte in the output GB file (# is: addr=byte)",
	"Map format:",
	"  -m   Map output generated as file[map]",
	"  -j   no$gmb symbol file generated as file[sym]",
	"  -x   Hexidecimal (default)",
	"  -d   Decimal",
	"  -q   Octal",
	"Output:",
	"  -i   Intel Hex as file[ihx]",
	"  -s   Motorola S19 as file[s19]",
	"  -Z   Gameboy image as file[gb]",
	"List:",
	"  -u   Update listing file(s) with link data as file(s)[.rst]",
	"End:",
	"  -e   or null line terminates input",
	"",
	0
};

/*)Function	VOID	usage()
 *
 *	The function usage() outputs to the stderr device the
 *	linker name and version and a list of valid linker options.
 *
 *	local variables:
 *		char ** dp		pointer to an array of
 *					text string pointers.
 *
 *	global variables:
 *		FILE *	stderr		c_library
 *
 *	functions called:
 *		int	fprintf()	c_library
 *
 *	side effects:
 *		none
 */

VOID
usage()
{
	register char	**dp;
	/* sdld specific */
	enum sdld_target_e target = get_sdld_target();

	fprintf(stderr, "\n%s Linker %s\n\n", is_sdld() ? "sdld" : "ASxxxx", VERSION);
	for (dp = (target == TARGET_IS_8051) ? usetxt_8051 : ((target == TARGET_IS_6808) ? usetxt_6808 : ((target == TARGET_IS_Z80) ? usetxt_z80 : ((target == TARGET_IS_GB) ? usetxt_gb : usetxt))); *dp; dp++)
		fprintf(stderr, "%s\n", *dp);
	/* end sdld specific */
	lkexit(1);
}

/*)Function	VOID	copyfile()
 *
 *		FILE	*dest		destination file
 *		FILE	*src		source file
 *
 *		function will copy source file to destination file
 *
 *
 *	functions called:
 *		int	fgetc()			c_library
 *		int	fputc()			c_library
 *
 *	side effects:
 *		none
 */
VOID
copyfile (dest, src)
FILE *dest;
FILE *src;
{
	int ch;

	while ((ch = fgetc(src)) != EOF) {
		fputc(ch,dest);
	}
}
