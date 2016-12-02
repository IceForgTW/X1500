#include <os_api.h>
#include <em3000_cfg.h>
#include <nor_flash.h>
#include <MemMgr.h>

#include <math.h> 
#include <malloc.h> 
#undef printf
#include <stdio.h>

#define MAX 100  
#define LEN sizeof(struct slink)  
char encrytion_buf[1024] __attribute__((aligned (4)));
char key_buf[MAX] __attribute__((aligned (4)));

void sub(int a[MAX],int b[MAX] ,int c[MAX] ); 
struct slink 
{ 
    int bignum[MAX];  /*bignum[98]用来标记正负号，1正，0负bignum[99]来标记实际长度*/ 
    struct slink *next; 
}; 

  
/*/--------------------------------------自己建立的大数运算库-------------------------------------*/ 

void print( int a[MAX] ) 
{ 
    int i; 
    
    for(i=0;i<a[99];i++) 
        printf("%d",a[a[99]-i-1]); 
    printf("\n\n"); 

    return; 
} 

int cmp(int a1[MAX],int a2[MAX]) 
{ 
    int l1, l2; 
    int i; 

    l1=a1[99]; 
    l2=a2[99]; 

    if (l1>l2) 
        return 1; 
    if (l1<l2) 
        return -1;  

    for(i=(l1-1);i>=0;i--) 
    { 
        if (a1[i]>a2[i]) 
            return 1;  
        if (a1[i]<a2[i]) 
            return -1; 
    }  
    return 0; 
} 

  
void mov(int a[MAX],int *b) 
{ 
    int j; 
    for(j=0;j<MAX;j++) 
        b[j]=a[j]; 

    return;      
} 

void mul(int a1[MAX],int a2[MAX],int *c) 
{  
    int i,j; 
    int y; 
    int x; 
    int z; 
    int w; 
    int l1, l2; 
    
    l1=a1[MAX-1]; 
    l2=a2[MAX-1]; 
    if (a1[MAX-2]=='-'&& a2[MAX-2]=='-') 
        c[MAX-2]=0; 
    else if (a1[MAX-2]=='-') 
        c[MAX-2]='-'; 
    else if (a2[MAX-2]=='-') 
        c[MAX-2]='-'; 
    
    for(i=0;i<l1;i++) 
    {  
        for(j=0;j<l2;j++) 
        {  
            x=a1[i]*a2[j]; 
            y=x/10; 
            z=x%10; 
            w=i+j;  
            c[w]=c[w]+z;  
            c[w+1]=c[w+1]+y+c[w]/10; 
            c[w]=c[w]%10; 
        } 
    }  

    w=l1+l2;  
    if(c[w-1]==0)w=w-1; 
        c[MAX-1]=w; 
    return; 
} 

void add(int a1[MAX],int a2[MAX],int *c) 
{ 
    int i,l1,l2;  
    int len,temp[MAX]; 
    int k=0;  

    l1=a1[MAX-1]; 
    l2=a2[MAX-1];  

    if((a1[MAX-2]=='-')&&(a2[MAX-2]=='-')) 
    { 
        c[MAX-2]='-'; 
    }  
    else if (a1[MAX-2]=='-') 
    { 
        mov(a1,temp); 
        temp[MAX-2]=0; 
        sub(a2,temp,c); 
        return; 
    }  
    else if (a2[MAX-2]=='-') 
    { 
        mov(a2,temp); 
        temp[98]=0; 
        sub(a1,temp,c); 
        return; 
    } 

    if(l1<l2)
        len=l1; 
    else 
        len=l2;  
    for(i=0;i<len;i++) 
    {  
        c[i]=(a1[i]+a2[i]+k)%10; 
        k=(a1[i]+a2[i]+k)/10; 
    }  

    if(l1>len) 
    {  
        for(i=len;i<l1;i++) 
        {  
            c[i]=(a1[i]+k)%10; 
            k=(a1[i]+k)/10; 
        }  

        if(k!=0) 
        {  
            c[l1]=k; 
            len=l1+1; 
        }  
        else len=l1; 
    } 
    else 
    {  
        for(i=len;i<l2;i++) 
        {  
            c[i]=(a2[i]+k)%10; 
            k=(a2[i]+k)/10; 
        }  
        if(k!=0) 
        {  
            c[l2]=k; 
            len=l2+1; 
        }  
        else 
            len=l2; 
    }  
    c[99]=len; 

    return; 
} 
 

void sub(int a1[MAX],int a2[MAX],int *c) 
{  
    int i,l1,l2;  
    int len=0,t1[MAX],t2[MAX]; 
    int k=0;  

    l1=a1[MAX-1]; 
    l2=a2[MAX-1];  
    if ((a1[MAX-2]=='-') && (a2[MAX-2]=='-')) 
    { 
        mov(a1,t1); 
        mov(a2,t2); 
        t1[MAX-2]=0; 
        t2[MAX-2]=0; 
        sub(t2,t1,c); 
        return; 
    }  
    else if( a2[MAX-2]=='-') 
    { 
        mov(a2,t2); 
        t2[MAX-2]=0; 
        add(a1,t2,c); 
        return; 
    }  
    else if (a1[MAX-2]=='-') 
    {   
        mov(a2,t2); 
        t2[MAX-2]='-'; 
        add(a1,t2,c); 
        return; 
    } 

    if(cmp(a1,a2)==1) 
    { 
        len=l2;  
        for(i=0;i<len;i++) 
        { 
            if ((a1[i]-k-a2[i])<0) 
            { 
                c[i]=(a1[i]-a2[i]-k+10)%10; 
                k=1; 
            }  
            else 
            { 
                c[i]=(a1[i]-a2[i]-k)%10; 
                k=0; 
            } 
        } 
        for(i=len;i<l1;i++) 
        { 
            if ((a1[i]-k)<0) 
            { 
                c[i]=(a1[i]-k+10)%10; 
                k=1; 
            }  
            else 
            { 
                c[i]=(a1[i]-k)%10; 
                k=0; 
            } 
        }  

        if(c[l1-1]==0)/*使得数组C中的前面所以0字符不显示了，如1000-20=0980--->显示为980了*/ 
        { 
            len=l1-1; 
            i=2; 
            while (c[l1-i]==0)/*111456-111450=00006，消除0后变成了6；*/ 
            { 
                len=l1-i; 
                i++; 
            } 
        } 
        else 
        { 
            len=l1; 
        } 
    } 
    else if(cmp(a1,a2)==(-1)) 
    { 
        c[MAX-2]='-'; 
        len=l1; 
        for(i=0;i<len;i++) 
        { 
            if ((a2[i]-k-a1[i])<0) 
            { 
                c[i]=(a2[i]-a1[i]-k+10)%10; 
                k=1; 
            }  
            else 
            { 
                c[i]=(a2[i]-a1[i]-k)%10; 
                k=0; 
            } 
        }  

        for(i=len;i<l2;i++) 
        {  
            if ((a2[i]-k)<0) 
            { 
                c[i]=(a2[i]-k+10)%10; 
                k=1; 
            }  
            else 
            { 
                c[i]=(a2[i]-k)%10; 
                k=0; 
            } 
        }  

        if(c[l2-1]==0) 
        { 
            len=l2-1; 
            i=2;  
            while (c[l1-i]==0) 
            { 
                len=l1-i; 
                i++; 
            } 
        } 
        else len=l2; 
    } 
    else if(cmp(a1,a2)==0) 
    { 
        len=1; 
        c[len-1]=0; 
    } 
    c[MAX-1]=len; 

    return; 
} 

  
void mod(int a[MAX],int b[MAX],int *c)/*/c=a mod b//注意：经检验知道此处A和C的数组都改变了。*/ 
{ 
    int d[MAX]; 
    mov (a,d); 

    while (cmp(d,b)!=(-1))/*/c=a-b-b-b-b-b.......until(c<b)*/ 
    { 
        sub(d,b,c); 
        mov(c,d);/*/c复制给a*/ 
    } 
    return;  
}  

void divt(int t[MAX],int b[MAX],int *c ,int *w)/*//试商法//调用以后w为a mod b, C为a div b;*/ 
{ 
    int a1,b1,i,j,m;/*w用于暂时保存数据*/ 

    int d[MAX],e[MAX],f[MAX],g[MAX],a[MAX]; 
    mov(t,a); 
    for(i=0;i<MAX;i++) 
        e[i]=0; 
    for(i=0;i<MAX;i++) 
        d[i]=0; 
    for(i=0;i<MAX;i++) 
        g[i]=0; 
    a1=a[MAX-1]; 
    b1=b[MAX-1]; 

    if (cmp(a,b)==(-1)) 
    { 
        c[0]=0; 
        c[MAX-1]=1; 
        mov(t,w); 
        return; 
    } 
    else if (cmp(a,b)==0) 
    { 
        c[0]=1;  
        c[MAX-1]=1; 
        w[0]=0; 
        w[MAX-1]=1; 
        return; 
    } 
    m=(a1-b1); 

    for(i=m;i>=0;i--)/*341245/3=341245-300000*1--->41245-30000*1--->11245-3000*3--->2245-300*7--->145-30*4=25--->25-3*8=1*/ 
    {  
        for(j=0;j<MAX;j++) 
        d[j]=0; 
        d[i]=1; 
        d[MAX-1]=i+1; 
        mov(b,g); 
        mul(g,d,e); 
                                 
        while (cmp(a,e)!=(-1))   
        {    
            c[i]++;    
            sub(a,e,f);       
            mov(f,a);/*f复制给g*/   
        }  
        for(j=i;j<MAX;j++)/*高位清零*/         
            e[j]=0;    
     }  
     mov(a,w);  
     if (c[m]==0) 
        c[MAX-1]=m;  
     else 
        c[MAX-1]=m+1;   
     return; 
}  

void mulmod(int a[MAX] ,int b[MAX] ,int n[MAX],int *m)/*解决 了 m=a*b mod n;*/ 
{  
    int c[MAX],d[MAX];  
    int i;  
    
    for(i=0;i<MAX;i++)   
        d[i]=c[i]=0;  
    mul(a,b,c);    
    divt(c,n, d,m); 
    for(i=0;i<m[MAX-1];i++)   
        printf("%d",m[m[MAX-1]-i-1]); 
    printf("\nm  length is :  %d \n",m[MAX-1]); 
}  

/*接下来的重点任务是要着手解决 m=a^p  mod n的函数问题。*/  
void expmod(int a[MAX] ,int p[MAX] ,int n[MAX],int *m) 
{  
    int t[MAX],l[MAX],temp[MAX]; /*/t放入2，l放入1；*/  
    int w[MAX],s[MAX],c[MAX],b[MAX],i;  
    for(i=0;i<MAX-1;i++)   
        b[i]=l[i]=t[i]=w[i]=0;  
    t[0]=2;t[MAX-1]=1;  
    l[0]=1;l[MAX-1]=1;    
    mov(l,temp);  
    mov(a,m);                                                                 
    mov(p,b); 

    while(cmp(b,l)!=0) 
    {  
        for(i=0;i<MAX;i++) 
            w[i]=c[i]=0; 
        divt(b,t,w,c);/*// c=p mod 2 w= p /2*/ 
        mov(w,b);/*//p=p/2*/ 
        if(cmp(c,l)==0) /*/余数c==1*/ 
        { 
            for(i=0;i<MAX;i++) 
                w[i]=0; 
            mul(temp,m,w); 
            mov(w,temp); 
            for(i=0;i<MAX;i++) 
                w[i]=c[i]=0; 
            divt(temp,n,w,c);/* /c为余c=temp % n，w为商w=temp/n */ 
            mov(c,temp); 
        } 
        
        for(i=0;i<MAX;i++) 
        s[i]=0; 
        mul(m,m,s);//s=a*a 
        for(i=0;i<MAX;i++) 
            c[i]=0; 
        divt(s,n,w,c);/*/w=s/n;c=s mod n*/ 
        mov (c,m); 
    } 

    for(i=0;i<MAX;i++) 
        s[i]=0;  
    mul(m,temp,s); 

    for(i=0;i<MAX;i++) 
        c[i]=0; 
    divt(s,n,w,c); 
    mov (c,m);/*余数s给m*/   
    m[MAX-2]=a[MAX-2];/*为后面的汉字显示需要，用第99位做为标记*/ 

    return;  
    /*/k=temp*k%n;*/ 
} 
  

int loadskey(int d[MAX],int n[MAX]) //导入私钥
{ 
    char str[MAX],ch; 
    int i,j,k; 
 //jz_nor_erase(PUBLIC_KEYS_ADD, MAX);//debug
    jz_nor_read(PUBLIC_KEYS_ADD, MAX, (unsigned int)key_buf);

    for(i=0;i<MAX;i++) 
        d[i]=n[i]=0; 

    j=k=0; 
    while(((unsigned char)(ch=key_buf[j++]) != 0xff) && (ch != 0)) 
    { 
        if(ch!=' ')  
        { 
            str[k]=ch; 
            k++; 
        } 
        else  
        { 
            for(i=0;i<k;i++) 
            { 
                d[i]=str[k-i-1]-48; 
            } 
            d[MAX-1]=k; 
            k=0; 
        } 
    } 

    k = (k<MAX)?k:MAX;

    for(i=0; i <k; i++) 
        n[i]=str[k-i-1]-48; 
    n[MAX-1]=k; 
    /*
    printf("dencrypted key d: "); 
    for(i=0;i<d[MAX-1];i++) 
        printf("%d",d[d[MAX-1]-i-1]); 
    printf("\n"); 
    printf("public key n : "); 

    for(i=0;i<n[MAX-1];i++) 
        printf("%d",n[n[MAX-1]-i-1]); 
    printf("\n"); 

    printf("get key(d,n) success!\n\n"); 
    */
    return k;
} 

  

void tdecrypto(int d[MAX], int n[MAX], char *plaintext) 
{ 
    struct slink *h,*p1,*p2,*tmp; 
    struct slink stP2;
    char ch;
    int i,j,k,c,count,temp=0;  
    int position=0;
   // jz_nor_erase(ENCRTION_TEXT_ADD, 1024);//debug
    jz_nor_read(ENCRTION_TEXT_ADD, 1024, (unsigned int)encrytion_buf); 

    printf("encryption context is\n"); 
    i=0; 
    j=3; 
    count=0;  
    h=p1=p2=(struct slink * )alignAlloc(4, LEN); 
    while(((unsigned char)(ch = encrytion_buf[position++]) != 0xff) && (ch !=0))  
    { 
        c=ch;
        if((c<0x30) || (c>0x39))
            return;
        if(j==3) 
        { 
            p1->bignum[MAX-2]=c; 
            j--; 
        } 
        else if(j==2) 
        { 
            temp=c-48; 
            j--; 
        } 
        else if(j==1) 
        { 
            p1->bignum[MAX-1]=temp*10+c-48; 
            j--; 
        } 
        else if (j==0) 
        { 
            p1->bignum[i]=c-48; 
            i++; 
            if(i==p1->bignum[MAX-1]) 
            { 
                i=0; 
                j=3; 
                count++; 
                if (count==1) 
                    h=p1; 
                else 
                    p2->next=p1; 
                p2=p1;  
                p1=(struct slink * )alignAlloc(4, LEN); 
            } 
        } 
    } 
    p2->next=NULL; 

	p2= &stP2; 
    p1=h; 
    k=0; 

    if(h!=NULL)/*/temp为暂存ASIIC码的int值*/ 
    do 
    { 
        for(i=0;i<MAX;i++) 
            p2->bignum[i]=0; 
        expmod( p1->bignum , d ,n ,p2->bignum); 
        temp=p2->bignum[0]+p2->bignum[1]*10+p2->bignum[2]*100; 
        if (( p2->bignum[MAX-2])=='0') 
        { 
            temp=0-temp; 
        }/*/转化为正确的ASIIC码，如-78-96形成汉字 */ 
        ch=temp;/* str[k]--->ch */ 
        plaintext[k++] = ch; 
		tmp = p1->next;
		deAlloc((MEM_ULONG)p1);
        p1=tmp; 
    }while ((p1!=NULL) && (k<CPUID_LEN)); 
    return; 
} 


void rsa_get_cpuid(char *plaintext_buf)
{ 
    int i; 
    int n[MAX],d[MAX]; 

    for(i=0; i<MAX; i++)  
        n[i]=d[i]=0;/*/简单初始化一下*/ 

    /* 公钥的长度一定超过10，否则认为公钥为错误，无需解密 */
    if(loadskey(d,n)>10)
    {
        tdecrypto(d, n, plaintext_buf); 
    }
}  
 

unsigned int crc_cal_by_bit(unsigned char* ptr, unsigned int len)
{
    unsigned int crc = 0;
    unsigned char i;
    
    while(len-- != 0)
    {
        for(i = 0x80; i != 0; i /= 2)
        {
            crc *= 2;
            if((crc&0x10000) !=0) //上一位CRC乘 2后，若首位是1，则除以 0x11021
                crc ^= 0x11021;
            if((*ptr&i) != 0)    //如果本位是1，那么CRC = 上一位的CRC + 本位/CRC_CCITT
                crc ^= 0x1021;
        }
        ptr++;
    }
    return crc;
}

