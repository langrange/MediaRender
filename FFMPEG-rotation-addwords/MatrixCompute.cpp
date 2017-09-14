//
// Created by yfei on 17-9-6.
//

#include "MatrixCompute.h"

#include <math.h>
#include <string.h>

char* file_Full_Path(std::string str){
    std::string str_return = "../Media/" + str ;
    char* full_path ;
    full_path = (char *)malloc(sizeof(char)*49) ;
    memcpy(full_path,str_return.c_str(),50);
    return  full_path ;
}

int rinv(double *a,int n)
{
    int *is,*js,i,j,k,l,u,v;
    double d,p;
    is=(int *)malloc(n*sizeof(int));
    js=(int *)malloc(n*sizeof(int));
    for (k=0; k<=n-1; k++)
    { d=0.0;
        for (i=k; i<=n-1; i++)
            for (j=k; j<=n-1; j++)
            { l=i*n+j; p=fabs(a[l]);
                if (p>d) { d=p; is[k]=i; js[k]=j;}
            }
        if (d+1.0==1.0)
        { free(is); free(js); printf("err**not inv\n");
            return(0);
        }
        if (is[k]!=k)
            for (j=0; j<=n-1; j++)
            { u=k*n+j; v=is[k]*n+j;
                p=a[u]; a[u]=a[v]; a[v]=p;
            }
        if (js[k]!=k)
            for (i=0; i<=n-1; i++)
            { u=i*n+k; v=i*n+js[k];
                p=a[u]; a[u]=a[v]; a[v]=p;
            }
        l=k*n+k;
        a[l]=1.0/a[l];
        for (j=0; j<=n-1; j++)
            if (j!=k)
            { u=k*n+j; a[u]=a[u]*a[l];}
        for (i=0; i<=n-1; i++)
            if (i!=k)
                for (j=0; j<=n-1; j++)
                    if (j!=k)
                    { u=i*n+j;
                        a[u]=a[u]-a[i*n+k]*a[k*n+j];
                    }
        for (i=0; i<=n-1; i++)
            if (i!=k)
            { u=i*n+k; a[u]=-a[u]*a[l];}
    }
    for (k=n-1; k>=0; k--)
    { if (js[k]!=k)
            for (j=0; j<=n-1; j++)
            { u=k*n+j; v=js[k]*n+j;
                p=a[u]; a[u]=a[v]; a[v]=p;
            }
        if (is[k]!=k)
            for (i=0; i<=n-1; i++)
            { u=i*n+k; v=i*n+is[k];
                p=a[u]; a[u]=a[v]; a[v]=p;
            }
    }
    free(is); free(js);
    return(1);
}


void trmul(double *a,double *b, double m,int n,int k,double *c)
{
    int i,j,l,u;
    for (i=0; i<=m-1; i++)
        for (j=0; j<=k-1; j++)
        {
            u=i*k+j; c[u]=0.0;
            for (l=0; l<=n-1; l++)
                c[u]=c[u]+a[i*n+l]*b[l*k+j];
        }
}