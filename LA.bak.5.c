// Fonction principal

#include "LA.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#if Prints
  #define info(a) printf a
  #define pause getchar()
#else
  #define info(a) (void)0
  #define pause (void)0
#endif

#define C_SIZE (UCHAR_MAX+1)
#define C_CUM(c) C[c << 1]		//cumulative C array
#define C_CNT(c) C[(c << 1) + 1] 	//C array counting characters
#define GLINK PREV_			//alias
#define GENDLINK ISA_			//alias

unsigned int i,j;
unsigned int gstart, gend;
unsigned int s, p, sr;
unsigned int tmp, gstarttmp, gendtmp;
unsigned int C[2*C_SIZE]; //counts and cumulative counts
const unsigned char *S_;
unsigned int *LA_, *SA_, *ISA_, *PREV_, *new_PREV,*loc_PREV;
unsigned int n_;
void *GSIZE_;
unsigned int CONTEXTSIZE,prev_counter,new_PREV_size,n_jumps,loc,loc_GLINK,gend_prev_suffs,gstart_prev_suffs,prev_nothing,loc_shift_count,group_size;


void         gsize_set( void *g, unsigned int pos, unsigned int val );
unsigned int gsize_get( const void *g, unsigned int pos );
void         gsize_clear( void *g, unsigned int pos ); //sets gsize at pos to 0
unsigned int gsize_dec_get( void *g, unsigned int pos ); //first decrement, then read
void         gsize_inc( void *g, unsigned int pos ); //increment

#if Prints
unsigned int k,gset,nomark,change;
unsigned int get_value, count;
void print_state();
void print_order_suffs();
void print_order_suffs_prev();
void print_algo3();
void print_decrement_group_count();
void print_GENDLINK_suffs();
void print_order_suffs_rem();
void print_new_PREV();
#endif

void setup_GSIZE();
void setup_rest();
void process_groups();
void get_gstart();
void compute_prev();
void set_GENDLINK_suffs();
void order_suffs();
void rearrange_prev_suffs();
void algo3();
void algo3mk2();
void decrement_group_count();
void set_new_GLINK();
void setup_new_GSIZE();
unsigned int get_GLINK(unsigned int pos);
void shift_loc_new_PREV();
void isort2(unsigned int start, unsigned int end);
void set_GSIZE2();
void get_prev_GLINK();


int gsaca_phase_1(const unsigned char *S, unsigned int *LA, unsigned int *SA, unsigned int *ISA , unsigned int *PREV, void *GSIZE, unsigned int n) {

	S_ = S;
	LA_ = LA;
	SA_ = SA;
	ISA_ = ISA;
	GSIZE_ = GSIZE;
	PREV_ = PREV;
	n_ = n;

	if (n_ == 0) return 0;
	if (S_[n_-1] != '\0')	{
        printf("non null terminated\n");
        return -1;
    }

	//// PHASE 1: pre-sort suffixes ////
	//build initial group structure
	for (i = 0; i < C_SIZE; ++i)	C_CNT(i) = 0;
	for (i = 0; i < n_; ++i) ++C_CNT(S_[i]); //count characters

	if (C_CNT(0) != 1) { //more than 1 nullbyte
		//free(ISA); free(PREV); gsize_free(GSIZE);
        printf("C_CNT nullbyte\n");
		return -1;
	}

    j = 0;

		setup_GSIZE(); //build cumulative counts and set up GSIZE

		setup_rest();

    // printf("set up ISA, GLINK and SA\n");

    info(("\n"));

		#if Prints
			print_state();
		#endif

		info(("n = %u\n", n_));

    //Line 3 phase 1
	//process groups from highest to lowest
  #if Prits
    count = 1;
  #endif
    info(("process groups from highest to lowest\n\n"));

		process_groups();

    info(("end of programme\n\n\n"));
    return 0;
}

void gsize_set( void *g, unsigned int pos, unsigned int val ) {
	((unsigned int *)g)[pos] = val;
}

unsigned int gsize_get( const void *g, unsigned int pos ) {
	return ((const unsigned int *)g)[pos];
}

void gsize_clear( void *g, unsigned int pos ) {
	((unsigned int *)g)[pos] = 0;
}

unsigned int gsize_dec_get( void *g, unsigned int pos ) {
	return --((unsigned int *)g)[pos];
}

void gsize_inc( void *g, unsigned int pos ) {
	++((unsigned int *)g)[pos];
}

void setup_GSIZE(){
	for (i = 0; i < C_SIZE; ++i) { //build cumulative counts and set up GSIZE
		if (C_CNT(i) > 0) {
			C_CUM(i) = j;
			gsize_set(GSIZE_, j, C_CNT(i));
			j += C_CNT(i);
		}
	}
}

void setup_rest(){
	for (i = n_-1; i < n_; --i) { //set up ISA, GLINK and SA
		gstart = C_CUM(S_[i]);
		sr = gstart + --C_CNT(S_[i]);
		GLINK[i] = gstart;
		ISA_[i] = sr;
		SA_[sr] = i;
		LA_[i] = 1;
	}
}

void process_groups(){
	for (gend = n_-1; gend > 0; gend = gstarttmp-1) {

		get_gstart();

		gstarttmp = gstart;
    gendtmp = gend;

    // isort2(gstart,gend);

    new_PREV_size = gsize_get(GSIZE_,gstart);

    new_PREV = (unsigned int *)calloc( new_PREV_size, sizeof(unsigned int) );

    loc_PREV = (unsigned int *)calloc( new_PREV_size , sizeof(unsigned int));

    info(("new_PREV of %u slots\n\n",new_PREV_size));

    CONTEXTSIZE = LA_[SA_[gstart]];

			#if Prints
				info(("gend = %u\n\n", gend));

				info(("gstart = %u\n", gstart));
				info(("gstarttmp = %u\n", gstarttmp));
				info(("gendtmp = %u\n\n", gendtmp));
        info(("CONTEXTSIZE = %u\n\n",CONTEXTSIZE));
			#endif

		//clear GSIZE group size for marking
		gsize_clear(GSIZE_, gstart);

		info(("GSIZE[%u] = %u\n\n", gstart, ((unsigned int *)GSIZE_)[gstart]));

        //ALGO 4
		//compute prev - pointers and mark suffixes of own group that
		//have a prev-pointer of own group pointing to them

    info(("==============compute prev============\n\n"));
    info(("i <- gend = %u\n\n", gend));

    // r=0;

		compute_prev(); //###################################

    // r = --j;
    // rtmp = r;

		//set GENDLINK of all suffixes for phase 2 and move unmarked
		//suffixes to the front of the actual group

    info(("--- set GENDLINK of all suffixes for phase 2 and move unmarked\n"));
    info(("--- suffixes to the front of the actual group\n\n"));
		j = 0;

    info(("j = 0\n\n"));

    set_GENDLINK_suffs();

    n_jumps = j;

    info(("\n"));

    #if Prints
      getchar();
    #endif

		//order the suffixes according on how much suffixes of same
		//group are jumped by them
    info(("--- Order the suffixes according on how much suffixes of same\n"));
    info(("--- group are jumped by them\n\n"));

    //############################  ! ! !  ############################
		gend = gstart + j; //exclusive bound by now
    //############################  ! ! !  ############################

    gend_prev_suffs = gend;

    info(("gstart = %u\nj = %u\n\n",gstart,j));
		info(("gend <- gstart + j = %u (exclusive bound by now)\n", gend));
		j = 0;

    info(("j = 0\n\n"));
    info(("do while gstart < gend\n\n"));

    //reset prev_couter for order_suffs
    prev_counter = 1;

    prev_nothing = 0;

		//OPTIMIZE: ////////////////////////////////////////////////////////////////
		order_suffs();
		////////////////////////////////////////////////////////////////////////////

    gstart_prev_suffs = gstart;

    info(("\n"));

		// //print_state();
        //ALGO 6
		//rearrange previous suffixes stored in other groups
    info(("--- set GLINK for moved suffixes\n\n"));

    // if(gstarttmp == gendtmp){
    //   new_PREV[0] = ISA_[SA_[gstarttmp]];
    // }else{

    get_prev_GLINK();

    #if Prints
      print_new_PREV();
    #endif

    pause;

    info(("--- rearrange previous suffixes stored in other groups\n\n\n"));

    loc = 0;
    loc_shift_count = 0;

		rearrange_prev_suffs();

		/**
  		decrement_group_count();
  		set_new_GLINK();
  		setup_new_GSIZE();
		*/

    free(new_PREV);
    free(loc_PREV);
    new_PREV = loc_PREV = 0;

		//prepare current group for phase 2
		SA_[gendtmp] = gstarttmp; //counter where to place next entry

    info(("gendtmp = %u\n", gendtmp));
    info(("gstarttmp = %u\n", gstarttmp));

    info(("SA[gendtmp] <- gstarttmp = %u\n\n",gstarttmp));

    info(("################# End of group %u, gstart = %u ###################\n",count++,gstarttmp));

		info(("\n"));
    #if Prints
		  print_state();
    #endif

    info(("\n"));

    pause;
	}

}

void get_gstart(){
	for (unsigned int k = gend; k > 0; --k) {
			tmp = gsize_get(GSIZE_, k);
			//printf("i = %u, tmp = %u\n\n", i, tmp);
			if (tmp != 0) {
					gstart = gend - tmp + 1;
					break;
			}
	}
}

// Trie par insertion
// void isort2(unsigned int start, unsigned int end){
//   unsigned int i, j;
//   unsigned int t,t_ISA;
//   for (i = start; i < end+1; i++){
//     for (j = i; j > start && SA_[j-1] > SA_[j]; j--) {
//       t = SA_[j]; t_ISA = ISA_[j];
//       ISA_[j] = ISA_[j-1];ISA_[j-1] = t_ISA;
//       SA_[j] = SA_[j-1];SA_[j-1] = t;
//     }
//   }
// }

void compute_prev(){
	for (i = gend; i >= gstart; --i) {
		info(("i = %d\n\n", i));
		s = SA_[i]; //use prev - pointers from already used groups
		info(("s = SA[i] = %u\n", SA_[i]));

		//////////////////////////////////////////

		//ALGO 3
		info(("p <- s-1 = %d\n\n", s-1));

    #if Prints
		  gset = n_+1;
    #endif

		//OPTIMIZE: //////////////////////////////////////////////////////////
		algo3();
		//////////////////////////////////////////////////////////////////////

		info(("\n"));

		PREV_[s] = p; //line 14
    new_PREV[i-gstart] = p; //line 14

		info(("PREV[s] <- p = %u\n\n", p));
    info(("new_PREV[i-gstart] = %u\n\n",new_PREV[i-gstart]));

    #if Prints
		  print_algo3();
    #endif

	}
}

///////////////////////////////////////////////////////////////////////////////////
void algo3(){
  #if Prints
    printf(" *** algo3 ***\n\n");
  #endif

  // for (p = s-1; p < n_; p = PREV_[p]) {
	for (p = s-1; p < n_; p--) {

		info((" ISA[p] = %u\n", ISA_[p]));
		info((" ISA[p] <= gend is %s\n\n", ISA_[p] <= gend ? "true" : "false"));

		if (ISA_[p] <= gend) {
			info(("  ISA[p] >= gstart is %s\n\n", ISA_[p] >= gstart ? "true" : "false"));
			if (ISA_[p] >= gstart) {
				gsize_set(GSIZE_, ISA_[p], 1); //mark ISA[p]  //comment

        #if Prints
          gset = ISA_[p];
        #endif

				info(("   GSIZE[ISA[p]] <- 1\n\n"));
        break;
			}

      //prev_suffs++;

      //info(("  prev_suffs = %u\n",prev_suffs));

			break;
		}
		// info((" p <- PREV[p] = %u\n\n",PREV_[p]));
    info(("p-- = %u\n\n",p));
	}

  #if Prints
    printf(" ***       ***\n\n");
  #endif
}
///////////////////////////////////////////////////////////////////////////////////

void set_GENDLINK_suffs(){
	for (i = gstart; i <= gend; ++i) {

    #if Prints
      nomark = 0;
    #endif

		info(("i = %d\n\n", i));
		s = SA_[i];
		info((" s <- SA[i] = %u\n",SA_[i]));
		//GENDLINK[s] = gend;
		info((" GENDLINK[s] <- gend = %u\n", GENDLINK[s]));
		info((" GSIZE[i] == 0 is %s\n\n", gsize_get(GSIZE_, i) == 0 ? "true" : "false"));

		if (gsize_get(GSIZE_, i) == 0) { //i is not marked

      #if Prints
        nomark = 1;
      #endif

      loc_PREV[j] = i-gstart-j;

      SA_[gstart+(j++)] = s;

			info(("  j = %d\n", j));
			info(("  gstart = %u\n", gstart));
			info(("  SA[gstart+j-1] <- s = %u\n",s));

      // if(i < gend){
      //   new_PREV[j] = new_PREV[i-gstart+1];
      // }else{
      //   if(j < new_PREV_size){
      //     new_PREV[j] = n_;
      //   }
      // }

		}

    gsize_set(GSIZE_,i,0);

		info(("\n"));
		#if Prints
			print_GENDLINK_suffs();
		#endif

    #if Prints
      print_new_PREV();
    #endif
	}
}

void order_suffs(){
	do {
		info(("\n"));
		i = gend-1; sr = gend;
		info((" gstart = %u\n", gstart));
		info((" gend = %u\n", gend));
		info((" sr <- gend = %d\n", sr));
		info((" i <- gend - 1 = %u\n\n", i));
		info((" while i >= gstart\n\n"));

    info((" prev_counter = %u\n\n",prev_counter));

		while (i >= gstart) {

      #if Prints
        printf(" ");
        print_new_PREV();
      #endif

      info(("  gstart = %u\n",gstart));
			info(("  gend = %u\n", gend));
			info(("  i = %u\n", i));
			info(("  sr = %u\n\n", sr));
      // info(("  r = %u\n\n",r));

			s = SA_[i];
			info(("  s <- SA[i] = %u\n", s));

      loc = loc_PREV[i-gstart];
      info(("  loc <- loc_PREV[i-gstart] = %u\n", loc_PREV[i-gstart]));

      p = new_PREV[i-gstart+loc];
      info(("  p <- new_PREV[i-gstart+loc] = %u\n\n",p));

      // p = PREV_[s];
			// info(("  p <- PREV[s] = %u\n", p));

      pause;

			info(("  p < n is %s\n\n", p < n_ ? "true" : "false"));

			if (p < n_) {

			info(("   ISA[p] = %u\n", ISA_[p]));
			info(("   ISA[p] < gstarttmp is %s\n\n", ISA_[p] < gstarttmp ? GREEN"true"RESET : RED"false"RESET));

				if (ISA_[p] < gstarttmp) { //p is in a lex. smaller group

          #if Prints
            get_value = SA_[gend-1];
            change = p;
          #endif

          LA_[p] += prev_counter * CONTEXTSIZE;

          if(i-gstart < new_PREV_size && i-gstart+1 < new_PREV_size){
            // printf("asdf %u\n",i-gstart-1);
            loc_PREV[i-gstart] = loc_PREV[i-gstart+1] + 1;
          }

          new_PREV[i-gstart+loc] = new_PREV[gend-gstart-1];

          loc_PREV[i-gstart+loc] = loc_PREV[gend-gstart-1] + (gend-1-i);

          // info(("     SA[gend] <- new_PREV[i-gstart+loc] = %u\n\n",new_PREV[i-gstart+1]));

					SA_[i--] = SA_[--gend];

          // shift_loc_new_PREV();

          // r -= n_jumps;

					info(("     SA[i] <- SA[gend-1] = %u\n",get_value));
					info(("     gend <- %u\n", gend));
					SA_[gend] = p; //push prev to back
					info(("     SA[gend] <- p = %u (i--)\n\n", p));
					info(("     i <- %u\n", i));
					info(("     gend = %u (--gend)\n\n", gend));

					#if Prints
						print_order_suffs();
            change = n_;
					#endif

          #if Prints
            print_new_PREV();
          #endif



          pause;

				} else { //p is in same group

          #if Prints
            get_value = PREV_[p];
          #endif

					PREV_[s] = PREV_[p];

          // if(i-gstart-1+loc >= 0){
          //   new_PREV[i-gstart+loc] = new_PREV[i-gstart-1+loc];
          // }

          if(loc_PREV[i-gstart] > 0){
            loc_PREV[i-gstart]--;
          }else{
            // loc_PREV[i-gstart] = new_PREV_size-1;  //test 1
            // loc_PREV[i-gstart] = i-gend;           //test 2
            loc_PREV[i-gstart] = new_PREV_size-1-(i-gstart);           //test 3
          }

          info(("     loc_PREV[i-gstart] <- new_PREV_size - i = %u\n\n",loc_PREV[i-gstart]));

          info(("     PREV[s] <- PREV[p] = %u\n",get_value));

          // info(("     ISA[s] = %u\n",ISA_[s]));
          // info(("     ISA[p] = %u\n",ISA_[p]));
          // info(("     r = %u\n\n",r));
          // info(("     s = %u\n     p = %u\n     gstart = %u\n\n",s,p,gstart));

					PREV_[p] = n_; //clear prev pointer, is not used in phase 2

          // new_PREV[i-gstart+loc-1] = n_;

          // shift_loc_new_PREV();

					info(("     PREV[p] <- n = %u\n\n",n_));
					--i;
					info(("     i <- %u\n\n", i));

					#if Prints
						print_order_suffs_prev();
					#endif

          #if Prints
            print_new_PREV();
          #endif

          pause;

				}
			} else { //prev points to nothing

        // prev_nothing++;
        info(("   prev points to nothing: REMOVE\n"));

        #if Prints
				    get_value = SA_[gstart];
        #endif

        shift_loc_new_PREV();

				SA_[i] = SA_[gstart++]; //remove entry


        info(("   SA[i] <- SA[gstart] = %u\n\n", get_value));

        // new_PREV[i-gstart] = new_PREV[gstart-gstarttmp];


				#if Prints
					print_order_suffs_rem();
				#endif

        #if Prints
          print_new_PREV();
        #endif

			}

		}
		info((" end while i>= gstart\n\n"));
		//write number of suffixes written to end on stack using GSIZE
		info((" gend < sr is %s\n\n", gend < sr ? "true" : "false"));

		if (gend < sr) {
			gsize_set(GSIZE_, gend, sr-gend);
			info(("  GSIZE[gend] <- sr - gend = %u\n\n", sr-gend));

			++j; //also, count number of splitted groups
			info(("  j = %u\n\n", j));
		}

    prev_counter++;
    // r = rtmp;

		info(("\n"));
		info(("while: gstart < gend is %s\n", gstart < gend ? "true" : "false"));
		info(("\n"));
	} while (gstart < gend);

}

void shift_loc_new_PREV(){
  for (size_t k = 0; k < gend-gstart; k++) {
    if(k >= i-gstart){
      if(loc_PREV[k] >= 0 && k < new_PREV_size - 1 ){
        loc_PREV[k] = loc_PREV[k+1] + 1;
      }else{
        loc_PREV[k] = 0;
      }
    }
  }

  // if(loc_PREV[0] )

// unsigned int tmp_shift;
//
//   for (unsigned int k = i-gstart+1; k < gend-gstart; k++) {
//
//     tmp_shift = loc_PREV[k];
//     tmp_shift++;
//
//     if(tmp_shift == gend-gstart){
//       loc_PREV[k] = 0;
//     }else{
//       loc_PREV[k] = tmp_shift;
//     }
//   }

}

void get_prev_GLINK(){
  info((" gstart = %u\n",gstart));

  for (i = gstart; i < gend_prev_suffs; ++i) {
    info((" i = %u\n\n", i));

    p = SA_[i];
    info((" p <- SA[i] = %u\n", SA_[gstart]));

    sr = get_GLINK(ISA_[p]);
    info((" sr <- get_GLINK(ISA_[p]) = %u\n",get_GLINK(ISA_[p])));
    // sr = GLINK[p];
    // info((" sr <- GLINK[p] = %u\n", GLINK[p]));
    // sr += gsize_get(GSIZE_, sr) - 1;
            // info((" sr += GSIZE[sr] = %u\n\n", sr));
    new_PREV[i-gstart] = sr;
    info((" new_PREV[i-gstart] <- sr = %u\n\n",new_PREV[i-gstart]));
    // gsize_inc(GSIZE_, sr);
    // GLINK[p] = sr; //not needed
    // new_PREV[i-gstart] = sr;
            // info((" GLINK[p] <- sr = %u\n\n", sr));
  }
}

void rearrange_prev_suffs(){

	while (j--) {

		info(("j = %u\n",j));
		info(("gstart = %u\n", gstart));
		info(("GSIZE[gstart] = %u\n", gsize_get(GSIZE_, gstart)));

    gend = gstart + gsize_get(GSIZE_, gstart);

    info(("gend <- gend + GSIZE[gstart] = %u\n\n", gstart + gsize_get(GSIZE_, gstart)));

    //decrement group count of previous group suffixes, and move them to back
		info(("i <- gend - 1 = %u\n\n", gend-1));

    loc += loc_shift_count;

    info(("-## Decrement group count of previous group suffixes, and move them to back\n\n"));

		decrement_group_count();

		//set new GLINK for moved suffixes
    info(("-## set new GLINK for moved suffixes\n\n"));
    set_new_GLINK();

		//set up GSIZE for newly created groups
					info(("-## set up GSIZE for newly created groups\n\n"));
    set_GSIZE2();
		// setup_new_GSIZE();

		gstart = gend;
					info(("gstart <- gend = %u\n\n", gstart));

    // if(prev_nothing != 0){
    //   prev_nothing--;
    // }

    // loc--;

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
void decrement_group_count(){
	for (i = gend-1; i >= gstart; --i) {
						info(("i = %u\n", i));
		p = SA_[i];
						info((" p <- SA[i] = %u\n", SA_[i]));
            info((" i-gstart = %u\n",i-gstart));

    // sr = get_GLINK(ISA_[p]);
    sr = new_PREV[i-gstart+loc];

    info((" sr <-  new_PREV[i-gstart+loc] = %u\n", new_PREV[i-gstart+loc]));
						// info((" sr <- GLINK[p] = %u\n", sr));
		sr += gsize_dec_get(GSIZE_, sr);
    group_size++;

						info((" sr += gsize_dec_get(GSIZE, sr) = %u\n\n", sr));
		//move p to back by exchanging it with last suffix s of group
		s = SA_[sr];
						info((" s <- SA[sr] = %u\n", SA_[sr]));
		tmp = ISA_[p];
						info((" tmp <- ISA[p] = %u\n\n", ISA_[p]));
		SA_[tmp] = s;	ISA_[s] = tmp;
						info((" SA[tmp] <- s = %u\n", s));
						info((" ISA[s] <- tmp = %u\n\n", tmp));
		SA_[sr] = p;	ISA_[p] = sr;
						info((" SA[sr] <- p = %u\n", p));
						info((" ISA[p] <- sr = %u\n\n\n", sr));

    // sr += gsize_get(GSIZE_, sr);
		info((" sr += GSIZE[sr] = %u\n\n", sr));
    // gsize_inc(GSIZE_, sr_tmp);

		#if Prints
			print_decrement_group_count();
		#endif

    loc_shift_count++;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned int get_GLINK(unsigned int pos) {
  unsigned int gsize = 0;
  do{
    gsize = gsize_get(GSIZE_,pos);
    pos--;
  }while (!gsize);

  return pos+1;
}


void set_new_GLINK(){
	for (i = gstart; i < gend; ++i) {
						info((" i = %u\n\n", i));
		p = SA_[i];
						info((" p <- SA[i] = %u\n", SA_[i]));
    // sr = get_GLINK(ISA_[p]);
    sr = new_PREV[i-gstart+loc];
    info((" sr <- new_PREV[i-gstart+loc] = %u\n",new_PREV[i-gstart+loc]));
    // sr = GLINK[p];
						// info((" sr <- GLINK[p] = %u\n", GLINK[p]));
		sr += gsize_get(GSIZE_, sr);
						info((" sr += GSIZE[sr] = %u\n\n", sr));
    new_PREV[i-gstart+loc] = sr;
            // gsize_inc(GSIZE_, sr);
		// GLINK[p] = sr; //not needed
    // new_PREV[i-gstart] = sr;
						// info((" GLINK[p] <- sr = %u\n\n", sr));
            info((" new_PREV[i-gstart+loc] <- sr = %u\n\n",new_PREV[i-gstart+loc]));
	}
}

// void setup_new_GSIZE(){
// 	for (i = gstart; i < gend; ++i) {
// 						info((" i = %u\n", i));
// 		// p = SA_[i];
// 						info((" p <- SA[i] = %u \n", SA_[i]));
// 		sr = GLINK[p];
//     // sr = get_GLINK(ISA_[p]);
//     // sr = new_PREV[i-gstart];
//     info((" sr <- new_PREV[i-gstart] = %u\n\n", new_PREV[i-gstart]));
//     info((" sr <- GLINK[p] = %u\n", GLINK[p]));
//             //
// 		// gsize_inc(GSIZE_, sr);
// 						printf(" GSIZE[sr] <- %u\n\n", gsize_get(GSIZE_, sr));
// 	}
// }

void set_GSIZE2(){
  // loc_GLINK = get_GLINK(ISA_[p]); //GLINK of a previous pointer (p) from the process before
	for (i = gstart; i < gend; ++i) {
						info((" i = %u\n\n", i));
		p = SA_[i];
						info((" p <- SA[i] = %u\n", SA_[i]));
    // sr = loc_GLINK;
    // sr = GLINK[p];
    sr = new_PREV[i-gstart+loc];
    // info((" sr <- get_GLINK(ISA_[p]) = %u\n",loc_GLINK));
		info((" sr <- new_PREV[i-gstart+loc] = %u\n", new_PREV[i-gstart+loc]));
		// sr += gsize_get(GSIZE_, sr);
						// info((" sr += GSIZE[sr] = %u\n\n", sr));
    gsize_inc(GSIZE_, sr);
    info((" GSIZE[sr] <- GSIZE[sr] + 1 = %u\n\n", gsize_get(GSIZE_, sr)));
		// GLINK[p] = sr; //not needed
    // new_PREV[i-gstart] = sr;
						// info((" GLINK[p] <- sr = %u\n\n", sr));
            // info((" new_PREV[i-gstart] <- sr = %u\n\n",new_PREV[i-gstart]));
	}
}

/////////////// Print functions //////////////////////

#if Prints
void print_state(){
	printf("%9s","i  ");
	for (k = 0; k < n_; k++) {
			printf(" %3u",k);
	}
	printf("\n");

	printf("%9s","SA[i] ");
	for (k = 0; k < n_; k++) {
			printf(" %3u", SA_[k]);
	}
			printf("\n");
      printf("\n");
	printf("%9s","LA[i] ");
	for (k = 0; k < n_; k++) {
			printf(" %3u", LA_[k]);
	}
			printf("\n");
			printf("\n");
	printf("%9s","ISA[i] ");
	for (k = 0; k < n_; k++) {
			printf(" %3u", ISA_[k]);
	}
			printf("\n");
			printf("\n");
	printf("%9s","PREV[i] ");
	for (k = 0; k < n_; k++) {
			if (PREV_[k] < n_) {
				printf(" %3u", PREV_[k]);
				/* code */
			}else{
				printf(" %3s","inf");
			}
	}
			printf("\n");
			printf("\n");
	printf("%9s","GSIZE[i] ");
	for (k = 0; k < n_; k++) {
			printf(" %3u", ((unsigned int *)GSIZE_)[k]);
	}
	printf("\n");
	printf("\n");
}

void print_algo3(){
	printf("%9s","i  ");
	for (k = 0; k < n_; k++) {
			printf(" %3u",k);
	}
	printf("\n");

	printf("%9s","SA[i] ");
	for (k = 0; k < n_; k++) {
			printf(" %3u", SA_[k]);
	}
			printf("\n");
	printf("%9s","LA[i] ");
	for (k = 0; k < n_; k++) {
			printf(" %3u", LA_[k]);
	}
			printf("\n");
			printf("\n");
	printf("%9s","ISA[i] ");
	for (k = 0; k < n_; k++) {
			printf(" %3u", ISA_[k]);
	}
			printf("\n");
			printf("\n");
	printf("%9s","PREV[i] ");
	for (k = 0; k < n_; k++) {
			if (PREV_[k] < n_) {
				if (k == s) {
					printf(BLUE" %3u"RESET, PREV_[k]);
				} else {
					printf(" %3u", PREV_[k]);
				}
			}else{
				if (k == s) {
					printf(CYAN" %3s"RESET, "inf");
				} else {
					printf(" %3s","inf");
				}

			}
	}
			printf("\n");
			printf("\n");
	printf("%9s","GSIZE[i] ");
	for (k = 0; k < n_; k++) {
		if (k == gset) {
			printf(RED" %3u"RESET, ((unsigned int *)GSIZE_)[k]);
		} else {
			printf(" %3u", ((unsigned int *)GSIZE_)[k]);
		}
	}
	printf("\n");
	printf("\n");
}

void print_order_suffs(){
	printf("%9s","i  ");
	for (k = 0; k < n_; k++) {
		printf(" %3u",k);
	}
	printf("\n");
	printf("%9s","SA[i] ");
	for (k = 0; k < n_; k++) {
		if (k == gend || k == i+1) {
			printf(" \x1b[32;1m%3u\x1b[m", SA_[k]);
		}else{
			printf(" %3u", SA_[k]);
		}
	}
	printf("\n");
	printf("%9s","LA[i] ");
	for (k = 0; k < n_; k++) {

    if (k == change) {
			printf(" \x1b[32;1m%3u\x1b[m", LA_[k]);
		}else{
			printf(" %3u", LA_[k]);
		}
	}
	printf("\n");
	printf("\n");
	printf("%9s","ISA[i] ");
	for (k = 0; k < n_; k++) {
		printf(" %3u", ISA_[k]);
	}
	printf("\n");
	printf("\n");
	printf("%9s","PREV[i] ");
	for (k = 0; k < n_; k++) {
		if (PREV_[k] < n_) {
			printf(" %3u", PREV_[k]);
		}else{
			printf(" %3s","inf");
		}
	}
	printf("\n");
	printf("\n");
	printf("%9s","GSIZE[i] ");
	for (k = 0; k < n_; k++) {
		printf(" %3u", ((unsigned int *)GSIZE_)[k]);
	}
	printf("\n\n");
}

void print_order_suffs_rem(){
	printf("%9s","i  ");
	for (k = 0; k < n_; k++) {
		printf(" %3u",k);
	}
	printf("\n");
	printf("%9s","SA[i] ");
	for (k = 0; k < n_; k++) {
		if (k == i) {
			printf(" \x1b[32;1m%3u\x1b[m", SA_[k]);
		}else{
			printf(" %3u", SA_[k]);
		}
	}
	printf("\n");
	printf("%9s","LA[i] ");
	for (k = 0; k < n_; k++) {
    if (k == change) {
      printf(" \x1b[32;1m%3u\x1b[m", LA_[k]);
    }else{
      printf(" %3u", LA_[k]);
    }
	}
	printf("\n");
	printf("\n");
	printf("%9s","ISA[i] ");
	for (k = 0; k < n_; k++) {
		printf(" %3u", ISA_[k]);
	}
	printf("\n");
	printf("\n");
	printf("%9s","PREV[i] ");
	for (k = 0; k < n_; k++) {
		if (PREV_[k] < n_) {
			printf(" %3u", PREV_[k]);
			/* code */
		}else{
			printf(" %3s","inf");
		}
	}
	printf("\n");
	printf("\n");
	printf("%9s","GSIZE[i] ");
	for (k = 0; k < n_; k++) {
		printf(" %3u", ((unsigned int *)GSIZE_)[k]);
	}
	printf("\n");
	printf("\n");
}

void print_order_suffs_prev(){
	printf("%9s","i  ");
	for (k = 0; k < n_; k++) {
			printf(" %3u",k);
	}
	printf("\n");
	printf("%9s","SA[i] ");
	for (k = 0; k < n_; k++) {
			printf(" %3u", SA_[k]);
	}
			printf("\n");
	printf("%9s","LA[i] ");
	for (k = 0; k < n_; k++) {
    if (k == change) {
      printf(" \x1b[32;1m%3u\x1b[m", LA_[k]);
    }else{
      printf(" %3u", LA_[k]);
    }
	}
			printf("\n");
			printf("\n");
	printf("%9s","ISA[i] ");
	for (k = 0; k < n_; k++) {
			printf(" %3u", ISA_[k]);
	}
			printf("\n");
			printf("\n");
	printf("%9s","PREV[i] ");
	for (k = 0; k < n_; k++) {
		if (k == s ||k == p) {
			if (PREV_[k] < n_) {
				printf(" \x1b[32;1m%3u\x1b[m", PREV_[k]);
			}else{
				printf(" \x1b[32;1m%3s\x1b[m","inf");
			}
			// printf(" \x1b[32;1m%3u\x1b[m", SA_[k]);
		}else{
			if (PREV_[k] < n_) {
				printf(" %3u", PREV_[k]);
			}else{
				printf(" %3s","inf");
			}
		}
	}
			printf("\n");
			printf("\n");
	printf("%9s","GSIZE[i] ");
	for (k = 0; k < n_; k++) {
			printf(" %3u", ((unsigned int *)GSIZE_)[k]);
	}
	printf("\n");
	printf("\n");
}

void print_GENDLINK_suffs(){
	printf("%9s","i  ");
	for (k = 0; k < n_; k++) {
			printf(" %3u",k);
	}
	printf("\n");

	printf("%9s","SA[i] ");
	for (k = 0; k < n_; k++) {
		if(nomark == 1){
			if (k == gstart+j-1) {
				printf(MAGENTA" %3u"RESET, SA_[k]);
			} else {
				printf(" %3u", SA_[k]);
			}
		}else{
			printf(" %3u", SA_[k]);
		}
	}
			printf("\n");
	printf("%9s","LA[i] ");
	for (k = 0; k < n_; k++) {
			printf(" %3u", LA_[k]);
	}
			printf("\n");
			printf("\n");
	printf("%9s","ISA[i] ");
	for (k = 0; k < n_; k++) {
		if (k == s) {
			printf(YELLOW" %3u"RESET, ISA_[k]);
		} else {
			printf(" %3u", ISA_[k]);
		}
	}
			printf("\n");
			printf("\n");
	printf("%9s","PREV[i] ");
	for (k = 0; k < n_; k++) {
			if (PREV_[k] < n_) {
				printf(" %3u", PREV_[k]);
				/* code */
			}else{
				printf(" %3s","inf");
			}
	}
			printf("\n");
			printf("\n");
	printf("%9s","GSIZE[i] ");
	for (k = 0; k < n_; k++) {
			printf(" %3u", ((unsigned int *)GSIZE_)[k]);
	}
	printf("\n");
	printf("\n");
}

void print_decrement_group_count(){
	printf("%9s","i  ");
	for (k = 0; k < n_; k++) {
			printf(" %3u",k);
	}
	printf("\n");

	printf("%9s","SA[i] ");
	for (k = 0; k < n_; k++) {
		if(k == sr){
			printf(GREEN" %3u"RESET, SA_[k]);
		}else if (k == tmp) {
			printf(YELLOW" %3u"RESET, SA_[k]);
		}else{
			printf(" %3u", SA_[k]);
		}
	}
			printf("\n");
	printf("%9s","LA[i] ");
	for (k = 0; k < n_; k++) {
			printf(" %3u", LA_[k]);
	}
			printf("\n");
			printf("\n");
	printf("%9s","ISA[i] ");
	for (k = 0; k < n_; k++) {
		if(k == p){
			printf(MAGENTA" %3u"RESET, ISA_[k]);
		}else if (k == s) {
			printf(BLUE" %3u"RESET, ISA_[k]);
		}else{
			printf(" %3u", ISA_[k]);
		}
	}
			printf("\n");
			printf("\n");
	printf("%9s","PREV[i] ");
	for (k = 0; k < n_; k++) {
			if (PREV_[k] < n_) {
				printf(" %3u", PREV_[k]);
			}else{
				printf(" %3s","inf");
			}
	}
			printf("\n");
			printf("\n");
	printf("%9s","GSIZE[i] ");
	for (k = 0; k < n_; k++) {
			printf(" %3u", ((unsigned int *)GSIZE_)[k]);
	}
	printf("\n");
	printf("\n");
}

void print_new_PREV(){
  printf("%9s","nPREV[i] ");
  for (k = 0; k < new_PREV_size; k++) {
    if (new_PREV[k] < n_) {
      printf(" %3u", new_PREV[k]);
    }else{
      printf(" %3s","inf");
    }
  }
  printf("\n\n");
  printf("%9s","nPloc[i] ");
  for (k = 0; k < new_PREV_size; k++) {
    if (loc_PREV[k] < n_) {
      printf(" %3u", loc_PREV[k]);
    }else{
      printf(" %3s","inf");
    }
  }
  printf("\n\n");
}

#endif
