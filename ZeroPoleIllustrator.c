#include <formatio.h>
#include "toolbox.h"
#include <cvirte.h>		
#include <userint.h>
#include "ZeroPoleIllustrator.h"

#define MAX_ARRAY 15
#define MAX_ZEROPOLE 15
/////////////////////////////////////////////added functions
double* GET_ZEROPOLE (char* string,int len);
void CheckProperties();
void DrawMap(); 
void FindGain();
void UpdateLists();
void Stable();
void Causal_MinPhase();
void Real();
void FIR();
void DoubleZeroPole();



////////////////////////////////////////////

///////////////////////////////////////////  for Canvas and Handles
static int panelHandle;
static int Tab_1,Tab_2,Tab_3,pActiveTabPanel;

static int Height,Width;
static int Center_X,Center_Y;
//////////////////////////////////////////
static 	char s_input_nominator[MAX_ARRAY],s_input_denominator[MAX_ARRAY];
static int num_zero,num_pole;

static double mat_zero[MAX_ZEROPOLE][3],mat_pole[MAX_ZEROPOLE][3];


static double Gain;


//////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

int main (int argc, char *argv[])
{
int activeTab;

	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "ZeroPoleIllustrator.uir", PANEL)) < 0)
		return -1;
	/* get the panel handles for the tab pages */
	GetPanelHandleFromTabPage(panelHandle, PANEL_TAB, 0, &Tab_1);
	GetPanelHandleFromTabPage(panelHandle, PANEL_TAB, 1, &Tab_2);
	//GetPanelHandleFromTabPage(panelHandle, PANEL_TAB, 2, &Tab_3);
	

	
	//////
	
	
	/////
	
	GetActiveTabPage(panelHandle, PANEL_TAB, &activeTab);
	GetPanelHandleFromTabPage(panelHandle, PANEL_TAB, activeTab, &pActiveTabPanel);
	SetCtrlAttribute(Tab_1, TAB1_CANVAS, ATTR_PEN_COLOR, VAL_DK_RED);
	SetCtrlAttribute(Tab_1, TAB1_CANVAS, ATTR_PEN_FILL_COLOR, VAL_TRANSPARENT);
	SetCtrlAttribute (Tab_1, TAB1_CANVAS, ATTR_PICT_BGCOLOR, VAL_TRANSPARENT);
	

	
	GetCtrlAttribute (Tab_1, TAB1_CANVAS, ATTR_HEIGHT, &Height);
    GetCtrlAttribute (Tab_1, TAB1_CANVAS, ATTR_WIDTH, &Width);
	Center_X =	Height/2;	
	Center_Y =	Width/2;


	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK QuitCallback (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface (0);
			break;
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
int CVICALLBACK addzero (int panel, int control, int event,
	 		 void *callbackData, int eventData1, int eventData2)
{
 double *p_zeropole;  //*(zeropole+0)=gain of zero, *(zeropole+1)=coe of Re num, *(zeropole+2)=coe of Im num
 
	  

	 
	 int i,l_nominator;
	
	 
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal (panelHandle, PANEL_input_nominator, &s_input_nominator[0]);

			
			l_nominator = StringLength (s_input_nominator);
			
			
		p_zeropole = GET_ZEROPOLE(s_input_nominator,l_nominator);
		double	zero[3]={*(p_zeropole+0),*(p_zeropole+1),*(p_zeropole+2)};
		
		
		for(i=0 ; i<MAX_ZEROPOLE-1 ; i++)
		{
			if (mat_zero[i][0]==0)//free space
			{
				mat_zero[i][0]=zero[0];	//the gain of the zero
				mat_zero[i][1]=zero[1];	//the X(Re) value of the zero
				mat_zero[i][2]=zero[2];	//the Y(Im) value of the zero
				break;
						   
			

			}
		}
		

		num_zero++;
		CheckProperties();
		break;
	}
	
	return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
int CVICALLBACK addpole (int panel, int control, int event,
					 void *callbackData, int eventData1, int eventData2)
{
 double *p_zeropole;  //*(zeropole+0)=gain of pole, *(zeropole+1)=coe of Re num, *(zeropole+2)=coe of Im num
 
	 

	 
	 int i,l_denominator;
	 
	switch (event)
	{
		case EVENT_COMMIT:
			
			
			GetCtrlVal (panelHandle, PANEL_input_denominator, &s_input_denominator[0]);
			
			l_denominator = StringLength (s_input_denominator);	
		
			
			p_zeropole = GET_ZEROPOLE(s_input_denominator,l_denominator); 
		double	pole[3]={*(p_zeropole+0),*(p_zeropole+1),*(p_zeropole+2)};
		
			for(i=0 ; i<MAX_ZEROPOLE-1 ; i++)
		{								   
			if (mat_pole[i][0]==0)//free space
			{
				mat_pole[num_pole][0]=pole[0];	//the gain of the pole
				mat_pole[num_pole][1]=pole[1];	//the X(Re) value of the pole
				mat_pole[num_pole][2]=pole[2];  //the Y(Im) value of the pole
				break;
			}
		}
		

		num_pole++;
		CheckProperties();
		break;
	}
	
	return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////the function can get both j and i as complex numbers
double* GET_ZEROPOLE (char* string,int len)
{
	int i,j=0,k;
	double d_coe_z=0;
	double d_coe_num_Re=0;
	double d_coe_num_Im=0;
	int flag=0;
	int flag_coe_num_Re=0,flag_coe_num_Im=0,flag_coe_z=0;
	
	char temp[MAX_ARRAY]={0};
	
	
	///////////////////////finding coefficients
	if(string[0]!='-')
		flag=1;
	
	for (i=0;i<len;i++)
	{
		if(string[i]=='+' || string[i]=='-')
			flag++;
		
		
		if(flag!=2 && string[i]!='z' && string[i]!='j' && string[i]!='i')//loading a coefficient into buffer
		{
			
			temp[j] = string[i];
			j++;
			
		}
		//////
		else
		{
		if(flag_coe_num_Re==0 && string[i]!='z' && string[i]!='j' && string[i]!='i') // for num_Re coefficient
		//for 2+1z+3j num_Re is at the beginning of the array 						  || for 1z+2+3j  num_Re is at the middle of the array ||  for 1z+3j+2  num_Re is at the end of the array || 	
		if((flag_coe_z==0 && flag_coe_num_Im==0 && i<len-1 && (string[i+1]=='+' ||string[i+1]=='-')) || (flag==2)  || (i==len-1 && string[i]!='z' && string[i]!='j' && string[i]!='i' ))
		{
			
			flag_coe_num_Re=1;	
			d_coe_num_Re=atof(temp);
			
			for(k=0;k<j;k++)//clearing the buffer
			{
				temp[k]=0;	
			}
				j=0;	  
		
		
			if(flag==2)				  //the next sign
			{
			temp[0]=string[i];
			j++;
			}
			flag=0;
		}
		/////
		  if((string[i] =='j'||string[i] =='i')&&flag_coe_num_Im==0)
	  	{		
			d_coe_num_Im = atof(temp);
			flag=0;
			flag_coe_num_Im=1;
			for(k=0;k<j;k++)
			{
			temp[k]=0;	
			}
			j=0;
			
	  	}
		/////
		if(string[i] =='z'&&flag_coe_z==0)
	  {
		d_coe_z = atof (temp);
		flag=0,flag_coe_z=1;
		for(k=0;k<j;k++)
		{
		temp[k]=0;	
		}
		j=0;
		
	  }
		/////
		
		}
	}

	if(flag_coe_num_Re==0)
	{
		d_coe_num_Re=atof(temp);	
		for(k=0;k<j;k++)
			{
			temp[k]=0;	
			}
			j=0;	
	}	
		

	 	//normalization
	d_coe_num_Re /= d_coe_z;
	d_coe_num_Im /= d_coe_z;
	
	double zp[3] = { d_coe_z, d_coe_num_Re, d_coe_num_Im}; //when d_coe_z is the gain of the zero or the pole
	return zp;
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void CheckProperties()
{
	CanvasClear (Tab_1, TAB1_CANVAS, VAL_ENTIRE_OBJECT); //delete canvas
	DrawMap();
	FindGain();
	
	Stable();
	Causal_MinPhase();
	Real();
	FIR();
	DoubleZeroPole();
	 
	UpdateLists();
	
	return;	
}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////finds the gain of the system
void FindGain()
{
	double gain_nominator=1,gain_denominator=1;
	int i;
	
	for(i=0 ; i<MAX_ZEROPOLE-1 ; i++)	
	{
	if(mat_zero[i][0]!=0)
		gain_nominator*=mat_zero[i][0];	
	
	if(mat_pole[i][0]!=0)
		gain_denominator*=mat_pole[i][0];
	
	}
	Gain=gain_nominator/gain_denominator;
	SetCtrlVal (panelHandle, PANEL_gain, Gain);

	return ;
	
}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////checks if the system is Stable
void Stable()
{
	int i,Is_Stable=1;//flag=1 stable
	double r;
	
	for(i=0 ; i<num_pole ; i++)
	{
		r=sqrt(pow(mat_pole[i][1],2)+pow(mat_pole[i][2],2));
		if(r<1)
		{
			Is_Stable=1;
			
		}
		else
		{
			Is_Stable=0;
			break;
		}
		
	}
	   if(num_pole==0)//if there are no poles
		 Is_Stable=1;
	   
		SetCtrlVal (panelHandle, PANEL_stable, Is_Stable);
		
	return;		
}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////checks if the system is Causal, checks if the system is Minimum phase
void Causal_MinPhase()
{
	 //Is_Causal is global parameter used in MinPhase
	 int i,Is_Causal=0; // Is_causal=1 system is Causal
	 	int Is_MinPhase=0;//Is_MinPhase=1 system is minimum phase
	double r;
	
	
	if(num_pole>=num_zero)
		Is_Causal=1;
	
  if(Is_Causal==1)
	for(i=0 ; i<num_zero ; i++)
	{
		r=sqrt(pow(mat_zero[i][1],2)+pow(mat_zero[i][2],2));
		if(r<1)
		{
			Is_MinPhase=1;
			
		}
		else
		{
			Is_MinPhase=0;
			break;
		}
	}
	
		if(num_zero==0)//if there are no zeros
		 Is_MinPhase=1;
	
	SetCtrlVal (panelHandle, PANEL_causal, Is_Causal);
	SetCtrlVal (panelHandle, PANEL_minphase, Is_MinPhase);
	
	
	return;		
}	
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void UpdateLists()
{
	int i;

	char s_temp_zero[MAX_ZEROPOLE][MAX_ARRAY]={0};
	char s_temp_pole[MAX_ZEROPOLE][MAX_ARRAY]={0};
	char s_nominator[MAX_ZEROPOLE*MAX_ARRAY]={0};
	char s_denominator[MAX_ZEROPOLE*MAX_ARRAY]={0};
	/////////////////////  clearing lists
	 ClearListCtrl (panelHandle, PANEL_list_zero);
	 ClearListCtrl (panelHandle, PANEL_list_pole);
	 SetCtrlVal (panelHandle, PANEL_show_nominator, "");
	 SetCtrlVal (panelHandle, PANEL_show_denominator, "");

	 /////////////
	
	if(num_zero==0)
		SetCtrlVal (panelHandle, PANEL_show_nominator, "1");	
	
	for(i=0 ; i<num_zero ; i++)// for zeros
	{
		if(mat_zero[i][0]!=0) //if zero not deleted
		{
			if(mat_zero[i][2]>0)	
				Fmt (s_temp_zero[i], "(z-(%f%fj))", -mat_zero[i][1], -mat_zero[i][2]);
			else
				Fmt (s_temp_zero[i], "(z-(%f+%fj))", -mat_zero[i][1], -mat_zero[i][2]);
			
			 
				InsertListItem (panelHandle, PANEL_list_zero, i, s_temp_zero[i], i);
		
			 
			strcat (s_nominator, s_temp_zero[i]);
		
			SetCtrlVal (panelHandle, PANEL_show_nominator, s_nominator);
		}
	}
	 
	for(i=0 ; i<num_pole ; i++) //for poles
	{
		if(mat_pole[i][0]!=0) //if zero not deleted
		{
			if(mat_pole[i][2]>0)	
				Fmt (s_temp_pole[i], "(z-(%f%fj))", -mat_pole[i][1], -mat_pole[i][2]);
			else
				Fmt (s_temp_pole[i], "(z-(%f+%fj))", -mat_pole[i][1], -mat_pole[i][2]);
			
		   
				InsertListItem (panelHandle, PANEL_list_pole, i, s_temp_pole[i], i);
			
			strcat (s_denominator, s_temp_pole[i]);
			SetCtrlVal (panelHandle, PANEL_show_denominator, s_denominator);
		}
	}
	
	
	
	
	return ;
}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void Real()
{
	int i,j,Is_Real;
	int have_pair_zero[MAX_ZEROPOLE]={0};
	int have_pair_pole[MAX_ZEROPOLE]={0};
	int all_paired_zero=1;
	int all_paired_pole=1;
	
	////////for zeros
	for(i=0 ; i<num_zero ; i++)
	{
		if(mat_zero[i][2]==0)
			have_pair_zero[i]=1;
			
		for(j=0 ; j<num_zero ; j++)
		{
		if((i !=j)&&(have_pair_zero[i]==0)&&( have_pair_zero[j]==0) && (mat_zero[i][1] == mat_zero[j][1])&&(mat_zero[i][2] == - mat_zero[j][2])) // if Re is eaqual between 2 zeros	
			{
			have_pair_zero[i]=1;
			have_pair_zero[j]=1;
			}
		
		}
		 if(num_zero==1 && mat_zero[i][2] !=0)
			have_pair_pole[i]=0;
		all_paired_zero*=have_pair_zero[i];
	}
	
	
	////////for poles
	for(i=0 ; i<num_pole ; i++)
	{
		if(mat_pole[i][2]==0)
			have_pair_pole[i]=1;
		for(j=0 ; j<num_pole ; j++)
		{
			if((i !=j)&&(have_pair_pole[i]==0)&&( have_pair_pole[j]==0) && (mat_pole[i][1] == mat_pole[j][1])&&(mat_pole[i][2] == - mat_pole[j][2])) // if Re is eaqual between 2 zeros	
			{
			have_pair_pole[i]=1;
			have_pair_pole[j]=1;
			}
		}	
		if(num_pole==1 && mat_pole[i][2] !=0)
			have_pair_pole[i]=0;
		all_paired_pole*=have_pair_pole[i];
	}
	
	
	
	Is_Real=all_paired_zero*all_paired_pole;
	
	SetCtrlVal (panelHandle, PANEL_real, Is_Real);
	
	return;	
}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void FIR()
{
	int i,Is_FIR=1; //Is_FIR=1 -> system is FIR
	for(i=0 ; i<MAX_ZEROPOLE-1 ; i++)
	{
		if(mat_pole[i][1]!=0 || mat_pole[i][2]!=0)
			{
				Is_FIR=0;
				break;
			}
	} 
	   SetCtrlVal (panelHandle, PANEL_fir, Is_FIR);
	   
	return;	
}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
int CVICALLBACK Delete_Zero (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	int i,j,index=-1;
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlIndex (panelHandle, PANEL_list_zero, &index);

			if(index==-1)
				break;
			mat_zero[index][0]=0;
			mat_zero[index][1]=0;
			mat_zero[index][2]=0;
			
			for(i=0 ; i<num_zero ; i++) //shortering the lists
				{
					for(j=0 ; j<num_zero ; j++)
					{
						if( j>i && mat_zero[i][0]==0 && mat_zero[j][0] !=0)
						{
						mat_zero[i][0]=mat_zero[j][0];
						mat_zero[i][1]=mat_zero[j][1];
						mat_zero[i][2]=mat_zero[j][2];
						
						mat_zero[j][0]=0;
						mat_zero[j][1]=0;
						mat_zero[j][2]=0;
						
						j=0;
						}
						
					}
				}
			num_zero--;
			CheckProperties();
			break;
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
int CVICALLBACK Delete_Pole (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	 int i,j,index=-1;
	 
	switch (event)
	{
		case EVENT_COMMIT:
			 GetCtrlIndex (panelHandle, PANEL_list_pole, &index);
			 
			 if(index==-1)
				break;
			 
				mat_pole[index][0]=0;
				mat_pole[index][1]=0;
				mat_pole[index][2]=0;
				
				for(i=0 ; i<num_pole ; i++) //shortering the lists
				{
					for(j=0 ; j<num_pole ; j++)
					{
						if(j>i && mat_pole[i][0]==0 && mat_pole[j][0] !=0)
						{
						mat_pole[i][0]=mat_pole[j][0];
						mat_pole[i][1]=mat_pole[j][1];
						mat_pole[i][2]=mat_pole[j][2];
						
						mat_pole[j][0]=0;
						mat_pole[j][1]=0;
						mat_pole[j][2]=0;
						
						j=0;
						}
						
					}
				}
				
				num_pole--;
				CheckProperties();	
			break;
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void DrawMap()
{
	int i,xz,yz,xp,yp;
	
	



	for(i=0 ; i<num_zero ; i++)
	{
		xz=Center_X-100*mat_zero[i][1];
		yz=Center_Y+100*mat_zero[i][2];  
		CanvasDrawTextAtPoint (Tab_1, TAB1_CANVAS, "O", VAL_APP_META_FONT,MakePoint(xz,yz), VAL_CENTER);	
		
	}
	
	for(i=0 ; i<num_pole ; i++)
	{
		xp=Center_X-100*mat_pole[i][1];
		yp=Center_Y+100*mat_pole[i][2];  
		CanvasDrawTextAtPoint (Tab_1, TAB1_CANVAS, "X", VAL_APP_META_FONT,MakePoint(xp,yp), VAL_CENTER);	
	
	}
	
	
	
	return;	
}
//////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////// 
void DoubleZeroPole()
{
 	int i,j,k,xz,yz,xp,yp;
	double numzero[MAX_ZEROPOLE][3]={0},numpole[MAX_ZEROPOLE][3]={0};
	char s_temp_zero[MAX_ARRAY]={0},s_temp_pole[MAX_ARRAY]={0};
	
	  //where numzero[i][0] is the number of the times numzero[i][1],numzero[i][2] 
	
	  if(num_zero>0)
	  {
		numzero[0][0]=1;
		numzero[0][1]= mat_zero[0][1];
		numzero[0][2]= mat_zero[0][2];	  
	  }
	  
	for(j=0 ; j<num_zero ; j++)
	{
		
		for(i=0 ; i<j ; i++)		
		{
			
		if( mat_zero[j][1]==mat_zero[i][1] && mat_zero[j][2]==mat_zero[i][2])// the zero is reapeted
		{
			  
				numzero[i][0]++;
				break;
				
				
		
		}	
		else//adding new zero into the numzero matrix
		{
			for(k=0 ; k<num_zero ; k++)
			{
		      if(numzero[k][0]==0)
		  	  {
			  
		  		numzero[k][0]=1;
		 		numzero[k][1]= mat_zero[j][1];
		 		numzero[k][2]= mat_zero[j][2];
				
				break;
		 	  }
			}
		}
		}
		
	}
	/////////////////////
	/// for poles
	if(num_pole>0)
	  {
		numpole[0][0]=1;
		numpole[0][1]= mat_pole[0][1];
		numpole[0][2]= mat_pole[0][2];	  
	  }
	  
	for(j=0 ; j<num_pole ; j++)
	{
		
		for(i=0 ; i<j ; i++)		
		{
			
		if( mat_pole[j][1]==mat_pole[i][1] && mat_pole[j][2]==mat_pole[i][2])
		{
			  
				numpole[i][0]++;
				break;
				
				
		
		}	
		else//the zero is not in numzero
		{
			for(k=0 ; k<num_pole ; k++)
			{
		      if(numpole[k][0]==0)
		  	  {
			  
		  		numpole[k][0]=1;
		 		numpole[k][1]= mat_pole[j][1];
		 		numpole[k][2]= mat_pole[j][2];
				
				break;
		 	  }
			}
		}
		}
		
	}
	
	for(i=0 ; i<num_zero-1 ; i++)
	{
		if(numzero[i][0]!=0)
		{
		xz=Center_X-100*numzero[i][1];
		yz=Center_Y+100*numzero[i][2];
		Fmt (s_temp_zero, "x%f",numzero[i][0]);
		CanvasDrawTextAtPoint (Tab_1, TAB1_CANVAS, s_temp_zero, VAL_APP_META_FONT,MakePoint(xz,yz), VAL_LOWER_LEFT);	
		}	
	}
	
	for(i=0 ; i<num_pole-1 ; i++)
	{
		if(numpole[i][0]!=0)
		{
		xp=Center_X-100*numpole[i][1];
		yp=Center_Y+100*numpole[i][2];
		Fmt (s_temp_pole, "x%f",numpole[i][0]);
		CanvasDrawTextAtPoint (Tab_1, TAB1_CANVAS, s_temp_pole, VAL_APP_META_FONT,MakePoint(xp,yp), VAL_LOWER_LEFT);	
		}
	}

			
			
		
	
	return;	
}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////









