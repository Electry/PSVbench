#include <psp2/kernel/processmgr.h>
#include <math.h>

#pragma GCC push_options
#pragma GCC optimize ("Ofast")

#define REAL float
#define ZERO 0.0f
#define ONE  1.0f

#define NTIMES 500

void matgen(REAL a[], int lda, int n, REAL b[], REAL *norma);
void dgefa(REAL a[], int lda, int n, int ipvt[], int *info);
void dgesl(REAL a[], int lda, int n, int ipvt[], REAL b[], int job);
void daxpy(int n, REAL da, REAL dx[], int incx, REAL dy[], int incy);
REAL ddot(int n, REAL dx[], int incx, REAL dy[], int incy);
void dscal(int n, REAL da, REAL dx[], int incx);
int idamax(int n, REAL dx[], int incx);
REAL epslon(REAL x);
void dmxpy(int n1, REAL y[], int n2, int ldm, REAL x[], REAL m[]);

REAL second() {
    return sceKernelGetProcessTimeLow() / 1000000.0f;
}

int run_linpack() {
    REAL timearray[9][9];
	REAL aa[200][200],a[200][201],b[200],x[200];
	REAL cray,ops,total,norma,normx;
	REAL resid, __attribute__((unused)) eps,t1,tm,tm2;
	REAL kf;
	int ipvt[200],n,i,ntimes,info,lda,ldaa,kflops;

	lda = 201;
	ldaa = 200;
	cray = .056;
	n = 100;

    ops = (2.0e0*(n*n*n))/3.0 + 2.0*(n*n);

    matgen((REAL *)a,lda,n,b,&norma);
    t1 = second();
    dgefa((REAL *)a,lda,n,ipvt,&info);
    timearray[0][0] = second() - t1;
    t1 = second();
    dgesl((REAL *)a,lda,n,ipvt,b,0);
    timearray[1][0] = second() - t1;
    total = timearray[0][0] + timearray[1][0];

    for (i = 0; i < n; i++) {
        x[i] = b[i];
    }
    matgen((REAL *)a,lda,n,b,&norma);
    for (i = 0; i < n; i++) {
            b[i] = -b[i];
    }
    dmxpy(n,b,n,lda,x,(REAL *)a);
    resid = 0.0;
    normx = 0.0;
    for (i = 0; i < n; i++) {
        resid = (resid > fabs((double)b[i]))
            ? resid : fabs((double)b[i]);
        normx = (normx > fabs((double)x[i]))
            ? normx : fabs((double)x[i]);
	}
    eps = epslon((REAL)ONE);

    timearray[2][0] = total;
    timearray[3][0] = ops/(1.0e3*total);
    timearray[4][0] = 2.0e3/timearray[3][0];
    timearray[5][0] = total/cray;

    matgen((REAL *)a,lda,n,b,&norma);
    t1 = second();
    dgefa((REAL *)a,lda,n,ipvt,&info);
    timearray[0][1] = second() - t1;
    t1 = second();
    dgesl((REAL *)a,lda,n,ipvt,b,0);
    timearray[1][1] = second() - t1;
    total = timearray[0][1] + timearray[1][1];
    timearray[2][1] = total;
    timearray[3][1] = ops/(1.0e3*total);
    timearray[4][1] = 2.0e3/timearray[3][1];
    timearray[5][1] = total/cray;

    matgen((REAL *)a,lda,n,b,&norma);
    t1 = second();
    dgefa((REAL *)a,lda,n,ipvt,&info);
    timearray[0][2] = second() - t1;
    t1 = second();
    dgesl((REAL *)a,lda,n,ipvt,b,0);
    timearray[1][2] = second() - t1;
    total = timearray[0][2] + timearray[1][2];
    timearray[2][2] = total;
    timearray[3][2] = ops/(1.0e3*total);
    timearray[4][2] = 2.0e3/timearray[3][2];
    timearray[5][2] = total/cray;

    ntimes = NTIMES;
    tm2 = 0.0;
    t1 = second();

	for (i = 0; i < ntimes; i++) {
        tm = second();
		matgen((REAL *)a,lda,n,b,&norma);
		tm2 = tm2 + second() - tm;
		dgefa((REAL *)a,lda,n,ipvt,&info);
	}

    timearray[0][3] = (second() - t1 - tm2)/ntimes;
    t1 = second();

	for (i = 0; i < ntimes; i++) {
        dgesl((REAL *)a,lda,n,ipvt,b,0);
	}

    timearray[1][3] = (second() - t1)/ntimes;
    total = timearray[0][3] + timearray[1][3];
    timearray[2][3] = total;
    timearray[3][3] = ops/(1.0e3*total);
    timearray[4][3] = 2.0e3/timearray[3][3];
    timearray[5][3] = total/cray;

    matgen((REAL *)aa,ldaa,n,b,&norma);
    t1 = second();
    dgefa((REAL *)aa,ldaa,n,ipvt,&info);
    timearray[0][4] = second() - t1;
    t1 = second();
    dgesl((REAL *)aa,ldaa,n,ipvt,b,0);
    timearray[1][4] = second() - t1;
    total = timearray[0][4] + timearray[1][4];
    timearray[2][4] = total;
    timearray[3][4] = ops/(1.0e3*total);
    timearray[4][4] = 2.0e3/timearray[3][4];
    timearray[5][4] = total/cray;

    matgen((REAL *)aa,ldaa,n,b,&norma);
    t1 = second();
    dgefa((REAL *)aa,ldaa,n,ipvt,&info);
    timearray[0][5] = second() - t1;
    t1 = second();
    dgesl((REAL *)aa,ldaa,n,ipvt,b,0);
    timearray[1][5] = second() - t1;
    total = timearray[0][5] + timearray[1][5];
    timearray[2][5] = total;
    timearray[3][5] = ops/(1.0e3*total);
    timearray[4][5] = 2.0e3/timearray[3][5];
    timearray[5][5] = total/cray;

	matgen((REAL *)aa,ldaa,n,b,&norma);
	t1 = second();
	dgefa((REAL *)aa,ldaa,n,ipvt,&info);
	timearray[0][6] = second() - t1;
	t1 = second();
	dgesl((REAL *)aa,ldaa,n,ipvt,b,0);
	timearray[1][6] = second() - t1;
	total = timearray[0][6] + timearray[1][6];
	timearray[2][6] = total;
	timearray[3][6] = ops/(1.0e3*total);
	timearray[4][6] = 2.0e3/timearray[3][6];
	timearray[5][6] = total/cray;

	ntimes = NTIMES;
	tm2 = 0;
	t1 = second();
	for (i = 0; i < ntimes; i++) {
		tm = second();
		matgen((REAL *)aa,ldaa,n,b,&norma);
		tm2 = tm2 + second() - tm;
		dgefa((REAL *)aa,ldaa,n,ipvt,&info);
	}
	timearray[0][7] = (second() - t1 - tm2)/ntimes;
	t1 = second();
	for (i = 0; i < ntimes; i++) {
		dgesl((REAL *)aa,ldaa,n,ipvt,b,0);
	}
	timearray[1][7] = (second() - t1)/ntimes;
	total = timearray[0][7] + timearray[1][7];
	timearray[2][7] = total;
	timearray[3][7] = ops/(1.0e3*total);
	timearray[4][7] = 2.0e3/timearray[3][7];
	timearray[5][7] = total/cray;

	kf = (timearray[3][3] < timearray[3][7]) ? timearray[3][3] : timearray[3][7];
	kf = (kf > ZERO) ? (kf + .5) : (kf - .5);
	if (fabs((double)kf) < ONE)
		kflops = 0;
	else {
		kflops = fabs((double)kf);
		if (kf < ZERO) kflops = -kflops;
	}

	return kflops / 1000;
}

void matgen(REAL a[], int lda, int n, REAL b[], REAL *norma) {
	int init, i, j;

	init = 1325;
	*norma = 0.0;
	for (j = 0; j < n; j++) {
		for (i = 0; i < n; i++) {
			init = 3125*init % 65536;
			a[lda*j+i] = (init - 32768.0)/16384.0;
			*norma = (a[lda*j+i] > *norma) ? a[lda*j+i] : *norma;
		}
	}
	for (i = 0; i < n; i++) {
          b[i] = 0.0;
	}
	for (j = 0; j < n; j++) {
		for (i = 0; i < n; i++) {
			b[i] = b[i] + a[lda*j+i];
		}
	}
}

void dgefa(REAL a[], int lda, int n, int ipvt[], int *info) {
    REAL t;
    int j, k, kp1, l, nm1;

	*info = 0;
	nm1 = n - 1;
	if (nm1 >=  0) {
		for (k = 0; k < nm1; k++) {
			kp1 = k + 1;
			l = idamax(n-k,&a[lda*k+k],1) + k;
			ipvt[k] = l;

			if (a[lda*k+l] != ZERO) {
				if (l != k) {
					t = a[lda*k+l];
					a[lda*k+l] = a[lda*k+k];
					a[lda*k+k] = t;
				}

				t = -ONE/a[lda*k+k];
				dscal(n-(k+1),t,&a[lda*k+k+1],1);

				for (j = kp1; j < n; j++) {
					t = a[lda*j+l];
					if (l != k) {
						a[lda*j+l] = a[lda*j+k];
						a[lda*j+k] = t;
					}
					daxpy(n-(k+1),t,&a[lda*k+k+1],1,
					      &a[lda*j+k+1],1);
  				}
  			}
			else {
            	*info = k;
			}
		}
	}
	ipvt[n-1] = n-1;
	if (a[lda*(n-1)+(n-1)] == ZERO) *info = n-1;
}

void dgesl(REAL a[], int lda, int n, int ipvt[], REAL b[], int job) {
	REAL t;
	int k, kb, l, nm1;

	nm1 = n - 1;
	if (job == 0) {
		if (nm1 >= 1) {
			for (k = 0; k < nm1; k++) {
				l = ipvt[k];
				t = b[l];
				if (l != k){
					b[l] = b[k];
					b[k] = t;
				}
				daxpy(n-(k+1),t,&a[lda*k+k+1],1,&b[k+1],1);
			}
		}

		for (kb = 0; kb < n; kb++) {
		    k = n - (kb + 1);
		    b[k] = b[k]/a[lda*k+k];
		    t = -b[k];
		    daxpy(k,t,&a[lda*k+0],1,&b[0],1);
		}
	}
	else {
		for (k = 0; k < n; k++) {
			t = ddot(k,&a[lda*k+0],1,&b[0],1);
			b[k] = (b[k] - t)/a[lda*k+k];
		}

		if (nm1 >= 1) {
			for (kb = 1; kb < nm1; kb++) {
				k = n - (kb+1);
				b[k] = b[k] + ddot(n-(k+1),&a[lda*k+k+1],1,&b[k+1],1);
				l = ipvt[k];
				if (l != k) {
					t = b[l];
					b[l] = b[k];
					b[k] = t;
				}
			}
		}
	}
}

void daxpy(int n, REAL da, REAL dx[], int incx, REAL dy[], int incy) {
	int i,ix,iy;

	if(n <= 0) return;
	if (da == ZERO) return;

	if(incx != 1 || incy != 1) {
		ix = 0;
		iy = 0;
		if(incx < 0) ix = (-n+1)*incx;
		if(incy < 0)iy = (-n+1)*incy;
		for (i = 0;i < n; i++) {
			dy[iy] = dy[iy] + da*dx[ix];
			ix = ix + incx;
			iy = iy + incy;
		}
      		return;
	}

	for (i = 0;i < n; i++) {
		dy[i] = dy[i] + da*dx[i];
	}
}

REAL ddot(int n, REAL dx[], int incx, REAL dy[], int incy) {
	REAL dtemp;
	int i,ix,iy;

	dtemp = ZERO;

	if(n <= 0) return(ZERO);

	if(incx != 1 || incy != 1) {
		ix = 0;
		iy = 0;
		if (incx < 0) ix = (-n+1)*incx;
		if (incy < 0) iy = (-n+1)*incy;
		for (i = 0;i < n; i++) {
			dtemp = dtemp + dx[ix]*dy[iy];
			ix = ix + incx;
			iy = iy + incy;
		}
		return(dtemp);
	}
	for (i=0;i < n; i++)
		dtemp = dtemp + dx[i]*dy[i];
	return(dtemp);
}

void dscal(int n, REAL da, REAL dx[], int incx) {
	int i,nincx;

	if(n <= 0)return;
	if(incx != 1) {
		nincx = n*incx;
		for (i = 0; i < nincx; i = i + incx)
			dx[i] = da*dx[i];
		return;
	}
	for (i = 0; i < n; i++)
		dx[i] = da*dx[i];
}

int idamax(int n, REAL dx[], int incx) {
	REAL dmax;
	int i, ix, itemp = 0;

	if( n < 1 ) return(-1);
	if(n ==1 ) return(0);
	if(incx != 1) {
		ix = 0;
		dmax = fabs((double)dx[0]);
		ix = ix + incx;
		for (i = 1; i < n; i++) {
			if(fabs((double)dx[ix]) > dmax)  {
				itemp = i;
				dmax = fabs((double)dx[ix]);
			}
			ix = ix + incx;
		}
	}
	else {
		itemp = 0;
		dmax = fabs((double)dx[0]);
		for (i = 1; i < n; i++) {
			if(fabs((double)dx[i]) > dmax) {
				itemp = i;
				dmax = fabs((double)dx[i]);
			}
		}
	}
	return (itemp);
}

REAL epslon(REAL x) {
	REAL a,b,c,eps;

	a = 4.0e0/3.0e0;
	eps = ZERO;
	while (eps == ZERO) {
		b = a - ONE;
		c = b + b + b;
		eps = fabs((double)(c-ONE));
	}
	return(eps*fabs((double)x));
}

void dmxpy(int n1, REAL y[], int n2, int ldm, REAL x[], REAL m[]) {
	int j,i,jmin;

	j = n2 % 2;
	if (j >= 1) {
		j = j - 1;
		for (i = 0; i < n1; i++)
            		y[i] = (y[i]) + x[j]*m[ldm*j+i];
	}

	j = n2 % 4;
	if (j >= 2) {
		j = j - 1;
		for (i = 0; i < n1; i++)
            		y[i] = ( (y[i])
                  	       + x[j-1]*m[ldm*(j-1)+i]) + x[j]*m[ldm*j+i];
	}

	j = n2 % 8;
	if (j >= 4) {
		j = j - 1;
		for (i = 0; i < n1; i++)
			y[i] = ((( (y[i])
			       + x[j-3]*m[ldm*(j-3)+i])
			       + x[j-2]*m[ldm*(j-2)+i])
			       + x[j-1]*m[ldm*(j-1)+i]) + x[j]*m[ldm*j+i];
	}

	j = n2 % 16;
	if (j >= 8) {
		j = j - 1;
		for (i = 0; i < n1; i++)
			y[i] = ((((((( (y[i])
			       + x[j-7]*m[ldm*(j-7)+i]) + x[j-6]*m[ldm*(j-6)+i])
		  	       + x[j-5]*m[ldm*(j-5)+i]) + x[j-4]*m[ldm*(j-4)+i])
			       + x[j-3]*m[ldm*(j-3)+i]) + x[j-2]*m[ldm*(j-2)+i])
			       + x[j-1]*m[ldm*(j-1)+i]) + x[j]  *m[ldm*j+i];
	}

	jmin = (n2%16)+16;
	for (j = jmin-1; j < n2; j = j + 16) {
		for (i = 0; i < n1; i++)
			y[i] = ((((((((((((((( (y[i])
			       	+ x[j-15]*m[ldm*(j-15)+i])
				+ x[j-14]*m[ldm*(j-14)+i])
			        + x[j-13]*m[ldm*(j-13)+i])
				+ x[j-12]*m[ldm*(j-12)+i])
			        + x[j-11]*m[ldm*(j-11)+i])
				+ x[j-10]*m[ldm*(j-10)+i])
			        + x[j- 9]*m[ldm*(j- 9)+i])
				+ x[j- 8]*m[ldm*(j- 8)+i])
			        + x[j- 7]*m[ldm*(j- 7)+i])
				+ x[j- 6]*m[ldm*(j- 6)+i])
			        + x[j- 5]*m[ldm*(j- 5)+i])
				+ x[j- 4]*m[ldm*(j- 4)+i])
			        + x[j- 3]*m[ldm*(j- 3)+i])
				+ x[j- 2]*m[ldm*(j- 2)+i])
			        + x[j- 1]*m[ldm*(j- 1)+i])
				+ x[j]   *m[ldm*j+i];
	}
}

#pragma GCC pop_options
