
#include <stdio.h>  	/* printf */
#include <stdlib.h>		/* malloc, free */

#define MAX_NUM(n1, n2) (((n1) > (n2)) ? (n1) : (n2))

static int pub_trans_s_count = 0;

/*----------------------------------------------------------------------------*/
/*                              METHODS DECLARATIONS                          */
/*----------------------------------------------------------------------------*/
typedef struct PublicTransport pub_trans_t; 
typedef struct Minibus minibus_t;
typedef struct Taxi taxi_t;
typedef struct SpecialTaxi specialtaxi_t;
typedef struct PublicConvoy publicconvoy_t;

typedef struct PublicTranspotVtable pub_trans_vtable_t;
typedef struct Minibus_vtable minibus_vtable_t;
typedef struct Taxi_vtable taxi_vtable_t;
typedef struct SpecialTaxi_vtable specialtaxi_vtable_t;
typedef struct PublicConvoy_vtable publicconvoy_vtable_t;

void PublicTransportCtor(pub_trans_t *this);
void PublicTransportCCtor(pub_trans_t *this, pub_trans_t *other);
void PublicTransportDtor(void *this);
void PublicTransportDisplay(void *this);
void PublicTransportPrintCount(void);
int PublicTransportGetID(pub_trans_t *this);

void MinibusCtor(minibus_t *this);
void MinibusDtor(void *this);
void MinibusCCtor(minibus_t *this, minibus_t *src);
void MinibusDisplay(void *this);
void MinibusWash(void *this, int minutes);

void TaxiCtor(taxi_t *this);
void TaxiDtor(void *this);
void TaxiCCtor(taxi_t *this, taxi_t *src);
void TaxiDisplay(void *this);

void SpecialTaxiCtor(specialtaxi_t *this);
void SpecialTaxiDtor(void *this);
void SpecialTaxiCCtor(specialtaxi_t *this, specialtaxi_t *src);
void SpecialTaxiDisplay(void *this);

void PublicConvoyCtor(publicconvoy_t *this);
void PublicConvoyDtor(void * this);
void PublicConvoyCCtor(publicconvoy_t *this, publicconvoy_t *src);
void PublicConvoyDisplay(void *this);

/*----------------------------------------------------------------------------*/
/*                                    CLASSES                                 */
/*----------------------------------------------------------------------------*/	

struct PublicTransport 
{
	const pub_trans_vtable_t *vptr;
    int m_license_plate;
};

struct Minibus
{
    pub_trans_t m_base;
    int m_num_seats;
};

struct Taxi
{
    pub_trans_t m_base;
};

struct SpecialTaxi
{
    taxi_t m_base;
};

struct PublicConvoy
{
    pub_trans_t m_base;
    pub_trans_t *m_pt1;
    pub_trans_t *m_pt2;
    minibus_t m_minibus;
    taxi_t m_taxi;
};

/*----------------------------------------------------------------------------*/
/*                                    VTABLES                                 */
/*----------------------------------------------------------------------------*/

struct PublicTranspotVtable
{
	void (*Dtor)(void *this);
	void (*Display)(void *this);
};

struct Minibus_vtable
{
    void (*Dtor)(void *this);
    void (*Display)(void *this);
    void (*Wash)(void *this, int minutes);
};

struct Taxi_vtable
{
    void (*Dtor)(void *this);
    void (*Display)(void *this);
};

struct SpecialTaxi_vtable
{
    void (*Dtor)(void *this);
    void (*Display)(void *this);
};

struct PublicConvoy_vtable
{
    void (*Dtor)(void *this);
    void (*Display)(void *this);
};

/*------------------------------- Vtable Inits -------------------------------*/

static pub_trans_vtable_t pt_vtable = {&PublicTransportDtor, 
													   &PublicTransportDisplay};

static minibus_vtable_t minibus_vtable = {&MinibusDtor, &MinibusDisplay,
																  &MinibusWash};

static taxi_vtable_t taxi_vtable = {&TaxiDtor, &TaxiDisplay};

static specialtaxi_vtable_t specialtaxi_vtable = {&SpecialTaxiDtor, 
														   &SpecialTaxiDisplay};

static publicconvoy_vtable_t publicconvoy_vtable = {&PublicConvoyDtor, 
														  &PublicConvoyDisplay};


/*----------------------------------------------------------------------------*/
/*                                   METHODS                                  */
/*----------------------------------------------------------------------------*/
 
 /*----------------------- PublicTransport Methods ---------------------------*/

 void PublicTransportCtor(pub_trans_t *this)
{
	this->vptr = &pt_vtable;
	
	++pub_trans_s_count;
	this->m_license_plate = pub_trans_s_count;
	
	printf("PublicTransport::Ctor()%d\n", this->m_license_plate);	
}

void PublicTransportDtor(void *this)
{
	((pub_trans_t *)this)->vptr = &pt_vtable;
	--pub_trans_s_count;
    printf("PublicTransport::Dtor()%d\n", 
										((pub_trans_t *)this)->m_license_plate);
}

void PublicTransportCCtor(pub_trans_t *this, pub_trans_t *other)
{
	(void)other;
	
	this->vptr = &pt_vtable;
	++pub_trans_s_count;
	this->m_license_plate = pub_trans_s_count;
	printf("PublicTransport::CCtor() %d\n", this->m_license_plate);
}

void PublicTransportDisplay(void *this)
{
	printf("PublicTransport::display(): %d\n", 
									    ((pub_trans_t *)this)->m_license_plate);
}

void PublicTransportPrintCount(void)
{
	printf("s_count: %d\n", pub_trans_s_count);
}

int PublicTransportGetID(pub_trans_t *this)
{
	return this->m_license_plate;
}

/*------------------------------- Minibus Methods ----------------------------*/

void MinibusCtor(minibus_t *this)
{
    PublicTransportCtor((pub_trans_t *)this);
    
    ((pub_trans_t *)this)->vptr = (pub_trans_vtable_t *)&minibus_vtable;
    this->m_num_seats = 20;
    
    printf("Minibus::Ctor()\n");
}

void MinibusDtor(void *this)
{
    ((pub_trans_t *)this)->vptr = (pub_trans_vtable_t *)&pt_vtable;
    printf("Minibus::Dtor()\n");
    
    PublicTransportDtor((pub_trans_t *)this);
}

void MinibusCCtor(minibus_t *this, minibus_t *src)
{
    PublicTransportCCtor(((pub_trans_t *)this), ((pub_trans_t *)src));
    
    ((pub_trans_t *)this)->vptr = (pub_trans_vtable_t *)&minibus_vtable;
    this->m_num_seats = src->m_num_seats;

    printf("Minibus::CCtor()\n");
}

void MinibusDisplay(void *this)
{
    printf("Minibus::display() ID:%d ", 
                                     PublicTransportGetID((pub_trans_t *)this));
    printf("num seats:%d\n", ((minibus_t *)this)->m_num_seats);
}

void MinibusWash(void *this, int minutes)
{
    printf("Minibus::wash(%d) ID:%d\n", minutes, 
									 PublicTransportGetID((pub_trans_t *)this));
}

/*---------------------------- TaxiCtor Methods ------------------------------*/

void TaxiCtor(taxi_t *this)
{
    PublicTransportCtor((pub_trans_t *)this);
    
    ((pub_trans_t *)this)->vptr = (pub_trans_vtable_t *)&taxi_vtable;
    
    printf("Taxi::Ctor()\n");
}

void TaxiDtor(void *this)
{
    ((pub_trans_t *)this)->vptr = (pub_trans_vtable_t *)&taxi_vtable;
    printf("Taxi::Dtor()\n");
    
    PublicTransportDtor((pub_trans_t *)this);
}

void TaxiCCtor(taxi_t *this, taxi_t *src)
{
    PublicTransportCCtor(((pub_trans_t *)this), ((pub_trans_t *)src));
    
    ((pub_trans_t *)this)->vptr = (pub_trans_vtable_t *)&taxi_vtable;
    
    printf("Taxi::CCtor()\n");
}

void TaxiDisplay(void *this)
{
    printf("Taxi::display() ID:%d\n", PublicTransportGetID((pub_trans_t *)this));
}

/*---------------------------- SpecialTaxi Methods ---------------------------*/

void SpecialTaxiCtor(specialtaxi_t *this)
{
    TaxiCtor((taxi_t *)this);
    
    ((pub_trans_t *)this)->vptr = (pub_trans_vtable_t *)&specialtaxi_vtable;

    printf("SpecialTaxi::Ctor()\n");
}

void SpecialTaxiDtor(void *this)
{
    ((pub_trans_t *)this)->vptr = (pub_trans_vtable_t *)&specialtaxi_vtable;
    printf("SpecialTaxi::Dtor()\n");    
    
    TaxiDtor((taxi_t *)this);
}

void SpecialTaxiCCtor(specialtaxi_t *this, specialtaxi_t *src)
{
    TaxiCCtor(((taxi_t *)this), ((taxi_t *)src));
    ((pub_trans_t *)this)->vptr = (pub_trans_vtable_t *)&specialtaxi_vtable;

    printf("SpecialTaxi::CCtor()\n");
}

void SpecialTaxiDisplay(void *this)
{
    printf("SpecialTaxi::display() ID: %d", 
    								 PublicTransportGetID((pub_trans_t *)this));    
}

/*--------------------------- PublicConvoy Methods ---------------------------*/

void PublicConvoyCtor(publicconvoy_t *this)
{
    PublicTransportCtor((pub_trans_t *)this);
    ((pub_trans_t *)this)->vptr = (pub_trans_vtable_t *)&publicconvoy_vtable;
    
    this->m_pt1 = (pub_trans_t *)malloc(sizeof(minibus_t));
    MinibusCtor((minibus_t *)this->m_pt1);
    
    this->m_pt2 = (pub_trans_t *)malloc(sizeof(taxi_t));
    TaxiCtor((taxi_t *)this->m_pt2);
    
    MinibusCtor(&(this->m_minibus));
    TaxiCtor(&(this->m_taxi));
}

void PublicConvoyDtor(void *this)
{
    MinibusDtor(((publicconvoy_t *)this)->m_pt1);
    free(((publicconvoy_t *)this)->m_pt1);
    
    TaxiDtor(((publicconvoy_t *)this)->m_pt2);
    free(((publicconvoy_t *)this)->m_pt2);
    
    TaxiDtor(&(((publicconvoy_t *)this)->m_taxi));
    
    MinibusDtor(&(((publicconvoy_t *)this)->m_minibus));

    PublicTransportDtor((pub_trans_t *)this);
}

void PublicConvoyCCtor(publicconvoy_t *this, publicconvoy_t *src)
{
    PublicTransportCCtor(((pub_trans_t *)this),((pub_trans_t *)src));
    ((pub_trans_t *)this)->vptr = (pub_trans_vtable_t *)&publicconvoy_vtable;
    
    this->m_pt1 = (pub_trans_t *)malloc(sizeof(minibus_t));
    MinibusCCtor((minibus_t *)this->m_pt1, (minibus_t *)src->m_pt1);

    this->m_pt2 = (pub_trans_t *)malloc(sizeof(taxi_t));
    TaxiCCtor((taxi_t *)this->m_pt2, (taxi_t *)src->m_pt2);

    MinibusCCtor(&(this->m_minibus), &(src->m_minibus));
    TaxiCCtor(&(this->m_taxi), &(src->m_taxi));
}

void PublicConvoyDisplay(void *this)
{
    ((publicconvoy_t *)this)-> 
						  m_pt1->vptr->Display(((publicconvoy_t *)this)->m_pt1);

    ((publicconvoy_t *)this)->
                		  m_pt2->vptr->Display(((publicconvoy_t *)this)->m_pt2);

    MinibusDisplay(&(((publicconvoy_t *)this)->m_minibus));
    TaxiDisplay(&(((publicconvoy_t *)this)->m_taxi));
}

/*----------------------------------------------------------------------------*/
/*                                 PRINT INFO                                 */
/*----------------------------------------------------------------------------*/

void PT1_Print_Info(pub_trans_t *tr)
{
    tr->vptr->Display(tr);
}

void Print_Info(void)
{
    PublicTransportPrintCount();
}

void MinibusPrint_Info(minibus_t *minibus)
{
    ((minibus_vtable_t *)((pub_trans_t *)minibus)->vptr)->Wash(minibus, 3);
}

pub_trans_t PT2_Print_Info(int i)
{
    minibus_t ret;
    pub_trans_t tmp_ret;
	(void)i;

    MinibusCtor(&ret);
    printf("print_info(int i)\n");
    MinibusDisplay(&ret);
    
    PublicTransportCCtor(&tmp_ret, (pub_trans_t *)&ret);
    MinibusDtor(&ret);
    
    return tmp_ret;
}

void Taxi_Display(taxi_t taxi)
{
    TaxiDisplay(&taxi);
}

/*----------------------------------------------------------------------------*/
/*                        			 main		                              */
/*----------------------------------------------------------------------------*/

int main(int argc, char **argv, char **envp)
{
	minibus_t m;
    pub_trans_t p_tmp;
    pub_trans_t *array[3];
    int i;
    pub_trans_t arr2[3];
    minibus_t m_tmp;
    taxi_t t_tmp;
    minibus_t m2;
    minibus_t arr3[4];
    taxi_t *arr4;
    specialtaxi_t st;
    taxi_t st_tmp;
    publicconvoy_t *ts1;
    publicconvoy_t *ts2;
	
	(void)argc;
	(void)argv;
	(void)envp;

    MinibusCtor(&m);
    MinibusPrint_Info(&m);
    p_tmp = PT2_Print_Info(3);
    PublicTransportDisplay(&p_tmp);
    PublicTransportDtor(&p_tmp);

    array[0] = (pub_trans_t *)malloc(sizeof(minibus_t));
    MinibusCtor((minibus_t *)array[0]);
    
    array[1] = (pub_trans_t *)malloc(sizeof(taxi_t));
    TaxiCtor((taxi_t *)array[1]);
    
    array[2] = (pub_trans_t *)malloc(sizeof(minibus_t));
    MinibusCtor((minibus_t *)array[2]);

    for (i = 0; i < 3; ++i)
    {
        (array[i])->vptr->Display(array[i]);
    }

    for (i = 0; i < 3; ++i)
    {
        (array[i])->vptr->Dtor(array[i]);
        free(array[i]);
    }

    MinibusCtor(&m_tmp);
    PublicTransportCCtor(&arr2[0],(pub_trans_t *)&m_tmp);
    MinibusDtor(&m_tmp);

    TaxiCtor(&t_tmp);
    PublicTransportCCtor(&arr2[1], (pub_trans_t *)&t_tmp);
    TaxiDtor(&t_tmp);

    PublicTransportCtor(&arr2[2]);

    for (i = 0; i < 3; ++i)
    {
        PublicTransportDisplay(&arr2[i]);
    }

    PT1_Print_Info(&arr2[0]);

    PublicTransportPrintCount();
    MinibusCtor(&m2);
    PublicTransportPrintCount();

    for (i = 0; i < 4; ++i)
    {
        MinibusCtor(&arr3[i]);
    }

    arr4 = (taxi_t *)malloc(4 * sizeof(taxi_t));

    for (i = 0; i < 4; ++i)
    {
        TaxiCtor(&arr4[i]);
    }

    for (i = 3; i >= 0; --i)
    {
        ((taxi_vtable_t *)((pub_trans_t *)(arr4 + i))->vptr)->Dtor(&arr4[i]);
    }    

    free(arr4);

   	printf("%d\n", (MAX_NUM(1, 2)));
    printf("%d\n", (MAX_NUM(1, (int)2.0)));
 
    SpecialTaxiCtor(&st);
    TaxiCCtor(&st_tmp, (taxi_t *)&st);
    Taxi_Display(st_tmp);
    TaxiDtor(&st_tmp);

    ts1 = (publicconvoy_t *)malloc(sizeof(publicconvoy_t));
    PublicConvoyCtor(ts1);
    
    ts2 = (publicconvoy_t *)malloc(sizeof(publicconvoy_t));
    PublicConvoyCCtor(ts2, ts1);
    
    ((pub_trans_t *)ts1)->vptr->Display(ts1);
    ((pub_trans_t *)ts2)->vptr->Display(ts2);

    ((pub_trans_t *)ts1)->vptr->Dtor(ts1);
    free(ts1);
    
    ((pub_trans_t *)ts2)->vptr->Display(ts2);
    ((pub_trans_t *)ts2)->vptr->Dtor(ts2);
    free(ts2);

    SpecialTaxiDtor(&st);
    
    for (i = 3; i >= 0; --i)
    {
        MinibusDtor(&arr3[i]);
    }

    MinibusDtor(&m2);

    for (i = 2; i >= 0; --i)
    {
        PublicTransportDtor(&arr2[i]);
    }

    MinibusDtor(&m); 

	return 0;
}

